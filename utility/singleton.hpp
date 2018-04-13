/**
 *
 * File singleton.hpp
 * 线程安全的单例模板类
 *
 * @author  :   yandaren1220@126.com
 * @date    :   2017-03-22
 */

#ifndef __ydk_utility_singleton_hpp__
#define __ydk_utility_singleton_hpp__

#include <mutex>

namespace utility
{
template<typename Type>
class singleton
{
public:
    static Type* get()
    {
        if (m_instance == nullptr)
        {
            std::lock_guard<std::mutex> lock(m_mtx);

            if (m_instance == nullptr)
            {
                Type* newval = new Type();

                m_instance = reinterpret_cast<void*>(newval);
            }
        }

        return reinterpret_cast<Type*>(m_instance);
    }

    static Type& instance()
    {
        return *get();
    }

    Type& operator * ()
    {
        return *get();
    }

    Type* operator -> ()
    {
        return get();
    }

protected:
    static void* m_instance;
    static std::mutex m_mtx;
};

template<typename Type>
void* singleton<Type>::m_instance = nullptr;

template<typename Type>
std::mutex singleton<Type>::m_mtx;

}

#endif