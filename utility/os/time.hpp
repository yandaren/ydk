/**
 *
 * File time.hpp
 * 一些时间相关的接口
 *
 * @author  :   yandaren1220@126.com
 * @date    :   2017-03-16
 */

#ifndef __ydk_utility_os_time_hpp__
#define __ydk_utility_os_time_hpp__

#include <stdint.h>
#include <chrono>

#ifdef _WIN32
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#endif

namespace ydk
{
namespace os
{
namespace time
{
    /** 
     * @brief 设置时间函数精度（针对windows系统下的timeGetTime函数，其他系统无效)
     */
    static bool time_begin_period(uint32_t p)
    {
#ifdef _WIN32
        return timeBeginPeriod(p) == TIMERR_NOERROR;
#else
        return true;
#endif
    }

    /** 
     * @brief 结束之前设定的时间函数精度（针对windows系统下的timeGetTime函数，其他系统无效)
     */
    static bool time_end_period(uint32_t p)
    {
#ifdef _WIN32
        return timeEndPeriod(p) == TIMERR_NOERROR;
#else
        return true;
#endif
    }

    /** 
     * @brief 返回系统开机到现在的毫秒数
     */
    static uint32_t gettime()
    {
#ifdef _WIN32
        /** 
         * 配合timeBeginPeriod(1), 可以使得精度达到1ms;之后通过timeEndPeriod(1)恢复默认精度;
         * 之所以不适用chrono,是因为在一些机器上的精度只有15ms
         */
        return timeGetTime();
#else
        return std::chrono::duration_cast<std::chrono::duration<uint32_t, std::milli>>(
            std::chrono::high_resolution_clock::now().time_since_epoch()).count();
#endif
    }

    /** 
     * @brief 返回1970年1月1日到到现在的毫秒数(跟time()函数效果差不多)
     */
    static int64_t gettime64_since_epoch()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    }
}

} // end namespace os
} // end namespace ydk

#endif