/**
 *
 * condition_variable_once.hpp
 *
 *
 * @author  :   yandaren1220@126.com
 * @date    :   2017-04-19
 */

#ifndef __ydk_utility_sync_condition_variable_once_hpp__
#define __ydk_utility_sync_condition_variable_once_hpp__

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <chrono>
#include <mutex>

namespace utility
{
namespace sync
{
class condition_variable_once
{
protected:
    std::condition_variable cv_;
    std::mutex              mtx_;
    std::atomic_bool        has_notifyed_;

public:
    condition_variable_once& operator=(const condition_variable_once& that) = delete;
    condition_variable_once(const condition_variable_once& that) = delete;

    condition_variable_once()
    {
        has_notifyed_ = false;
    }

    void notify()
    {
        if (!has_notifyed_.exchange(true))
        {
            cv_.notify_all();
        }
    }

    /** 
     * @brief wait for the specific milliseconds
     * @param milli = 0, wait forever
     *              > 0, wait for specific milliseconds
     *              < 0, illegal, return
     */
    void wait_for(int32_t milli_seconds)
    {
        if (has_notifyed_)
            return;

        if (milli_seconds < 0)
            return;

        std::unique_lock<std::mutex> locker(mtx_);

        if (milli_seconds == 0)
        {
            cv_.wait(locker);
            return;
        }

        cv_.wait_for(locker, std::chrono::milliseconds(milli_seconds));
    }
};
}
}

#endif