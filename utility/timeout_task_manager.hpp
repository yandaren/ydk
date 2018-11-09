/**
* @brief timeout_task_manager.hpp
*
* @author  :   yandaren1220@126.com
* @date    :   2018-08-29
*/

#ifndef __ydk_utility_timeout_task_manager_hpp__
#define __ydk_utility_timeout_task_manager_hpp__

#include <stdint.h>
#include <stdarg.h>
#include <map>
#include <memory>
#include <chrono>
#include <thread>
#include <mutex>
#include <atomic>
#include <string>
#include <vector>
#include <unordered_map>

#ifdef _WIN32
#define snprintf _snprintf
#endif

namespace utility
{
    namespace details {
        static uint64_t gettime64_since_epoch()
        {
            return std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        }
    }

    struct timeout_task {
        typedef std::shared_ptr<timeout_task> ptr;

        timeout_task() : expire_time(0), task_id(0), task_type(0), time_out(0){
        }

        virtual ~timeout_task() {
        }

        /** 
         * @brief time_out in  milliseconds
         */
        void        set_time_out(int32_t time_out_in_milli) {
            uint64_t now = details::gettime64_since_epoch();
            start_time = now;
            time_out = time_out_in_milli;
            expire_time = (uint64_t)(now + time_out_in_milli);
        }

        int32_t     get_time_out() const {
            return time_out;
        }

        uint64_t    get_start_time() const {
            return  start_time;
        }

        uint64_t    get_expire_time() const {
            return expire_time;
        }

        void        set_task_id(uint64_t id) {
            task_id = id;
        }

        uint64_t    get_task_id() const {
            return task_id;
        }

        /** 
         * @brief set task_type
         */
        void        set_task_type(int32_t type) {
            task_type = type;
        }

        int32_t     get_task_type() const {
            return task_type;
        }

        void        set_desc(const std::string& info) {
            desc = info;
        }

        const std::string& get_desc() const {
            return desc;
        }

        virtual void do_affect() = 0;
        virtual void calc_desc() {};

    protected:
        uint64_t start_time;        // 开始时间
        uint64_t expire_time;       // 过期时间
        uint64_t task_id;           // task id
        int32_t  time_out;          // 多久过期(ms)
        int32_t  task_type;         // task 类型
        std::string desc;           // 描述信息
    };

    class timeout_task_manager
    {
    public:
        enum log_lvl {
            log_lvl_info = 0,
            log_lvl_error = 1,
        };

        enum {
            max_log_len = 1023,
        };

        typedef std::function<void(log_lvl log_level, const char* data, int32_t len)> log_handler_func;

    protected:
        /** <expired_time, task_id> */
        typedef std::multimap<uint64_t, uint64_t> expire_map_type;

        /** <task_id, expired_time> */
        typedef std::unordered_map<uint64_t, timeout_task::ptr> time_out_task_map_type;

        expire_map_type             expire_map_;
        time_out_task_map_type      time_out_task_map_;
        std::string                 name_;
        std::thread*                thread_;
        std::mutex                  mtx_;
        uint64_t                    next_task_id_;
        std::atomic<bool>           started_;
        volatile bool               stopped_;
        log_handler_func*           internal_logger_;
        uint32_t                    check_interval_;            // 检测间隔(ms)

    public:
        timeout_task_manager():thread_(nullptr), stopped_(true), next_task_id_(0), internal_logger_(nullptr), check_interval_(10){
            started_ = false;
        }

        ~timeout_task_manager() {
            stop();
            wait_for_stop();

            if (thread_) {
                delete thread_;
                thread_ = nullptr;
            }

            if (internal_logger_) {
                delete internal_logger_;
                internal_logger_ = nullptr;
            }
        }

        void set_name(const char* name) {
            name_ = name;
        }

        const std::string& get_name() const {
            return name_;
        }

        /** 
         * @brief set check interval in milliseconds
         */
        void set_check_interval(uint32_t  interval) {
            check_interval_ = interval;
        }

        void start() {
            if (!started_.exchange(true)) {
                stopped_ = false;
                thread_ = new std::thread(std::bind(&timeout_task_manager::run, this));
            }
        }

        void stop() {
            stopped_ = true;
        }

        void wait_for_stop() {
            if (thread_ && thread_->joinable()) {
                thread_->join();
            }
        }

        void    set_log_handler(const log_handler_func& func){
            if (internal_logger_) {
                *internal_logger_ = func;
            }
            else {
                internal_logger_ = new log_handler_func(func);
            }
        }

        void    clear_log_handler() {
            internal_logger_ = nullptr;
        }

    public:
        uint64_t    add_task(timeout_task::ptr task, int32_t task_type, int32_t time_out_in_milli) {
            std::lock_guard<std::mutex> locker(mtx_);

            task->set_task_id(++next_task_id_);
            task->set_task_type(task_type);
            task->set_time_out(time_out_in_milli);
            task->calc_desc();

            expire_map_.insert(std::make_pair(task->get_expire_time(), task->get_task_id()));
            time_out_task_map_[task->get_task_id()] = task;

            log_msg(log_lvl_info, "timeout_task_mgr[%s] add_task{id: %llu, type: %d, time_out: %dms, expired_time: %llu, desc:{%s}}, cur expire_map_size[%zd], timeout_task_map_size[%zd].",
                name_.c_str(), task->get_task_id(), task->get_task_type(), task->get_time_out(), task->get_expire_time(), task->get_desc().c_str(), expire_map_.size(), time_out_task_map_.size());

            return task->get_task_id();
        }

        timeout_task::ptr remove_task(uint64_t task_id) {
            std::lock_guard<std::mutex> locker(mtx_);

            timeout_task::ptr task;
            auto iter = time_out_task_map_.find(task_id);
            if (iter == time_out_task_map_.end()) {
                return task;
            }

            task = iter->second;
            time_out_task_map_.erase(iter);
            auto t_pair = expire_map_.equal_range(task->get_expire_time());
            for (auto t_iter = t_pair.first; t_iter != t_pair.second; ++t_iter) {
                if (t_iter->second == task_id) {
                    expire_map_.erase(t_iter);
                    break;
                }
            }

            log_msg(log_lvl_info, "timeout_task_mgr[%s] remove_task{id: %llu, type: %d, time_out: %dms, expired_time: %llu, desc:{%s}}, remain expire_map_size[%zd], timeout_task_map_size[%zd].",
                name_.c_str(), task->get_task_id(), task->get_task_type(), task->get_time_out(), task->get_expire_time(), task->get_desc().c_str(), expire_map_.size(), time_out_task_map_.size());

            return task;
        }

    protected:
        void    log_msg(log_lvl lvl, const char* format, ...) {
            if (!internal_logger_) {
                return;
            }

            char buffer[max_log_len + 1];
            char* log_buff = buffer;
            va_list ap;
            va_start(ap, format);
            int32_t len = vsnprintf(0, 0, format, ap);
            va_end(ap);

            if (len > max_log_len) {
                log_buff = (char*)malloc(len + 1);
            }
            va_start(ap, format);
            len = vsprintf(log_buff, format, ap);
            va_end(ap);

            (*internal_logger_)(lvl, log_buff, len);

            if (log_buff != buffer) {
                free(log_buff);
            }
        }

        void    tick() {
            std::vector<std::uint64_t> time_out_task_list;
            {
                std::lock_guard<std::mutex> locker(mtx_);
                uint64_t cur_time = details::gettime64_since_epoch();

                auto iter = expire_map_.begin();
                for (; iter != expire_map_.end(); ++iter) {
                    if (iter->first > cur_time) {
                        break;
                    }
                    time_out_task_list.push_back(iter->second);
                }
            }

            // process time out task
            for (auto task_id : time_out_task_list) {
                timeout_task::ptr task = remove_task(task_id);
                if (!task) {
                    log_msg(log_lvl_error, "timeout_task_mgr[%s] can't find time_out task[%llu]",
                        name_.c_str(), task_id);

                    continue;
                }

                log_msg(log_lvl_info, "timeout_task_mgr[%s] process time_out_task{id: %llu, type: %d, time_out: %dms, expired_time: %llu, desc:{%s}}.",
                    name_.c_str(), task->get_task_id(), task->get_task_type(), task->get_time_out(), task->get_expire_time(), task->get_desc().c_str());

                task->do_affect();
            }
        }

        void run() {
            while (!stopped_) {
                tick();

                std::this_thread::sleep_for(std::chrono::milliseconds(check_interval_));
            }
        }
    };
}

#endif