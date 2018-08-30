/**
* @brief timer_manager.hpp
*
* @author  :   yandaren1220@126.com
* @date    :   2018-08-29
*/

#ifndef __ydk_utility_timer_manager_hpp__
#define __ydk_utility_timer_manager_hpp__

#include <stdint.h>
#include <thread>
#include <chrono>
#include <atomic>
#include <functional>
#include <utility/time_wheel.hpp>

namespace utility
{
    class timer_manger : public time_wheel<std::mutex>
    {
    protected:
        std::thread*           thread_;
        std::atomic<bool>      started_;
        volatile    bool       stopped_;

    public:
        timer_manger() :thread_(nullptr), stopped_(true){
            started_ = false;
        }

        ~timer_manger() {
            stop();
            wait_for_stop();

            if (thread_) {
                delete thread_;
                thread_ = nullptr;
            }
        }

        void start() {
            if (!started_.exchange(true)) {
                stopped_ = false;
                thread_ = new std::thread(std::bind(&timer_manger::run, this));
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

    protected:
        void run() {
            while (!stopped_) {
                tick();

                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        }
    };
}

#endif