/** 
 * @brief time_wheel.hpp
 *
 * @author  :   yandaren1220@126.com
 * @date    :   2018-08-29
 */

#ifndef __ydk_utility_time_wheel_hpp__
#define __ydk_utility_time_wheel_hpp__

#include <stdint.h>
#include <functional>
#include <mutex>
#include <chrono>
#include <utility/noncopyable.hpp>
#include <utility/sync/null_mutex.hpp>

namespace utility
{
    typedef void*   timer_handle;
    typedef std::function<void(timer_handle* handle)> timer_handler;

    template<class Mutex = utility::sync::null_mutex>
    class time_wheel : public noncopyable
    {
    protected:
        /** 时间粒度(精度) */
        static const uint64_t time_granularity = 10;

        /*
         |<-----------------------------32 bits ------------------------------>|
         |<- tvn_bits->|<- tvn_bits->|<- tvn_bits->|<- tvn_bits->|<- tvr_bits->|
         |<-  6 bits ->|<-  6 bits ->|<-  6 bits ->|<-  6 bits ->|<-  8 bits ->|
         | idxs of tv5 | idxs of tv4 | idxs of tv3 | idxs of tv2 | idxs of tv1 |
         */
        static const int32_t tvn_bits = 6;
        static const int32_t tvr_bits = 8;
        static const int32_t tvn_size = (1 << tvn_bits);    // 64
        static const int32_t tvr_size = (1 << tvr_bits);    // 256
        static const int32_t tvn_mask = (tvn_size - 1);     // 63
        static const int32_t tvr_mark = (tvr_size - 1);     // 255
        static const int32_t max_wheel_lvl = 5;             // 5级

    protected:
        struct timer_node_link;
        struct timer_node {
            timer_node_link*    link;
            timer_node*         prev;
            timer_node*         next;
            timer_handler       fn;
            uint64_t            expired_time;

            timer_node() : link(nullptr), expired_time(0) { prev = next = this; }
        };

        struct timer_node_link {
            timer_node* head;

            timer_node_link() : head(nullptr) {
            }

            ~timer_node_link() {
            }

            bool empty() {
                return head == nullptr;
            }

            void remove(timer_node* nd) {
                if (nd->link != this) {
                    return;
                }

                if (head == nd) {
                    // only one node
                    if (head->next == head && head->prev == head) {
                        head = nullptr;
                    }
                    else {
                        head = head->next;
                    }
                }

                if (nd->prev) {
                    nd->prev->next = nd->next;
                }
                if (nd->next) {
                    nd->next->prev = nd->prev;
                }
                nd->prev = nd->next = nullptr;

                nd->link = nullptr;
            }

            timer_node* first() {
                return head;
            }

            void    add_to_tail(timer_node* node) {
                node->link = this;
                if (!head) {
                    head = node;
                    node->prev = node->next = node;
                }
                else {
                    timer_node* tail = head->prev;
                    node->prev = tail;
                    tail->next = node;
                    node->next = head;
                    head->prev = node;
                }
            }
        };

        template<int32_t Size>
        struct timer_node_link_arr {
            timer_node_link arr[Size];
        };

        typedef timer_node_link_arr<tvr_size> wheel_root;
        typedef timer_node_link_arr<tvr_size> wheel;

    protected:

        wheel_root                  tv1_;                   // 第一级时间轮
        wheel                       tv2_;                   // 第二级时间轮
        wheel                       tv3_;                   // 第三级时间轮
        wheel                       tv4_;                   // 第四级时间轮
        wheel                       tv5_;                   // 第五级时间轮
        uint64_t                    base_time_;             // 基准时间
        Mutex                       mtx_;

    public:
        time_wheel() {
            base_time_ = gettime64_since_epoch() / time_granularity;
        }

        ~time_wheel() {
        }

    public:
        /** 
         * @brief 
         *
         * @param handler       : the timer call back function
         * @param time_in_milli : the timer will expired in time_in_milli milliseconds
         */
        timer_handle*     make_timer(const timer_handler& handler, uint64_t time_in_milli) {
            timer_node* node = new timer_node();
            node->fn = handler;
            node->expired_time = (uint64_t)(gettime64_since_epoch() + time_in_milli) / time_granularity;

            return (timer_handle*)node;
        }

        /** 
         * @brief add timer
         */
        void            add_timer(timer_handle* handle) {
            timer_node* nd = (timer_node*)handle;
            if (!nd) {
                return;
            }

            // locker
            std::lock_guard<Mutex> locker(mtx_);

            add_timer_internal(nd);
        }

        /** 
         * @brief try remove the timer
         */
        void            remove_timer(timer_handle* handle) {

            timer_node* nd = (timer_node*)handle;
            if (!nd) {
                return;
            }

            // locker
            {
                std::lock_guard<Mutex> locker(mtx_);
                if (nd->link) {
                    nd->link->remove(nd);
                }
            }

            delete nd;
        }


        /** 
         * @brief free timer resource
         */
        void            free_timer(timer_handle* handle) {

            timer_node* nd = (timer_node*)handle;
            if (!nd) {
                return;
            }

            delete nd;
        }

        /** 
         * @brief tick
         */
        void            tick() {
            uint64_t cur_time = gettime64_since_epoch() / time_granularity;

            // lock
            mtx_.lock();
            while (cur_time >= base_time_) {
                int32_t idx = (int32_t)(base_time_ & tvr_mark);

                if (!idx &&
                    !cascade(tv2_, index_n(0)) &&
                    !cascade(tv3_, index_n(1)) &&
                    !cascade(tv4_, index_n(2))) {
                    cascade(tv5_, index_n(3));
                }

                ++base_time_;
                timer_node_link& expired_link = tv1_.arr[idx];

                // call the timer call back
                while (!expired_link.empty()) {
                    timer_node* n = expired_link.first();

                    // remove the timer
                    expired_link.remove(n);

                    // unlock
                    mtx_.unlock();

                    // call timer handler function
                    n->fn((timer_handle*)n);

                    // lock
                    mtx_.lock();
                }
            }

            // unlock
            mtx_.unlock();
        }

    protected:

        int32_t         cascade(wheel& wl, int32_t index) {

            timer_node_link link = wl.arr[index];
            wl.arr[index].head = nullptr;

            // re add the nodes of the link to the time wheel
            timer_node* n = link.head;
            timer_node* next = nullptr;
            while (n) {
                next = n->next;

                // readd to the time wheel
                add_timer_internal(n);

                n = next;

                if (n == link.head) {
                    break;
                }
            }

            return index;
        }

        int32_t         index_n(int32_t n) {
            return (int32_t)((base_time_ >> (tvr_bits + n * tvn_bits)) & tvn_mask);
        }

        void     add_timer_internal(timer_node *node) {

            uint64_t expired_time = node->expired_time;

            timer_node_link* link = nullptr;
            uint64_t count = expired_time - base_time_;
            if (count < tvr_size) {
                uint32_t idx = expired_time & tvr_mark;
                link = &tv1_.arr[idx];
            }
            else if (count < 1 << (tvr_bits + tvn_bits)) {
                uint32_t idx = (expired_time >> tvr_bits) & tvn_mask;
                link = &tv2_.arr[idx];
            }
            else if (count < 1 << (tvr_bits + 2 * tvn_bits)) {
                uint32_t idx = (expired_time >> (tvr_bits + tvn_bits)) & tvn_mask;
                link = &tv3_.arr[idx];
            }
            else if (count < 1 << ((tvr_bits + 3 * tvn_bits))) {
                uint32_t idx = (expired_time >> (tvr_bits + 2 * tvn_bits)) & tvn_mask;
                link = &tv4_.arr[idx];
            }
            else if ((int32_t)count < 0) {
                // if the timer expired when insert
                /*
                * Can happen if you add a timer with expires == jiffies,
                * or you set a timer to go off in the past
                */
                link = &tv1_.arr[base_time_ & tvr_mark];
            }
            else {
                /* If the timeout is larger than 0xffffffff on 64-bit
                * architectures then we use the maximum timeout:
                */
                if (count > 0xffffffffUL) {
                    count = 0xffffffffUL;
                    expired_time = count + base_time_;
                }
                uint32_t idx = (expired_time >> (tvr_bits + 3 * tvn_bits)) & tvn_mask;
                link = &tv5_.arr[idx];
            }

            // add to tail
            link->add_to_tail(node);
        }

        static uint64_t gettime64_since_epoch()
        {
            return std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        }
    };
}

#endif