/**
 *
 * a object allocator.hpp
 *
 *
 * @author  :   yandaren1220@126.com
 * @date    :   2017-06-20
 */

#ifndef __ydk_utility_pool_object_allocator_hpp__
#define __ydk_utility_pool_object_allocator_hpp__

#include <utility/noncopyable.hpp>
#include <cstdint>
#include <list>
#include <mutex>

namespace utility
{
    template<typename T, class Mutex>
    class object_allocator : public utility::noncopyable           // not allow copy and assign
    {
        enum 
        {
            POOL_INFLATE_SIZE = 50,
        };

    public:
        typedef uint32_t        size_type;
        typedef T*              pointer;
        typedef T&              reference;
        typedef const T*        const_pointer;
        typedef const T&        const_reference;
        typedef T               value_type;


        typedef std::list<pointer>                object_queue;
        typedef typename object_queue::iterator   object_queue_iter;

        object_allocator(size_type init_size = 0, size_type grow_size = 1) throw()
            :m_grow_size(grow_size)
        {
            inflate(init_size);
        }

        ~object_allocator()
        {
            clear();
        }

        // reset
        void clear()
        {
            std::lock_guard<Mutex> locker(m_mtx);

            auto iter = m_total_objects.begin();
            for( iter; iter != m_total_objects.end(); ++ iter )
            {
                ::operator delete (*iter);
            }
            m_total_objects.clear();
            m_free_objects.clear();
        }

        // allocate a object
        T* allocate()
        {
            std::lock_guard<Mutex> locker(m_mtx);

            if( m_free_objects.empty() )
            {
                inflate(m_grow_size);
            }

            T* ret = m_free_objects.front();
            m_free_objects.pop_front();

            return ret;
        }

        // reclain a object
        void reclaim(pointer p)
        {
            std::lock_guard<Mutex> locker(m_mtx);

            m_free_objects.push_back(p);
        }

        inline size_type free_object_count()
        {
            std::lock_guard<Mutex> locker(m_mtx);

            return m_free_objects.size();
        }

        inline size_type free_memory_size()
        {
            std::lock_guard<Mutex> locker(m_mtx);

             return m_free_objects.size() * sizeof(value_type);
        }

        inline size_type total_object_count()
        {
            std::lock_guard<Mutex> locker(m_mtx);

            return m_total_objects.size();
        }

        inline size_type total_memory_size()
        {
            std::lock_guard<Mutex> locker(m_mtx);

            return m_total_objects.size() * sizeof(value_type);
        }

    public:
        pointer address(reference x) const
        {
            return &x;
        }

        const_pointer address(const_reference x) const
        {
            return &x;
        }

    private:
        void    inflate(size_type count = POOL_INFLATE_SIZE)
        {
            for (size_type i = 0; i < count; ++i)
            {
                pointer p = static_cast<pointer>(::operator new (sizeof(value_type)));
                m_free_objects.push_back(p);
                m_total_objects.push_back(p);
            }
        }

    private:
        object_queue            m_free_objects;
        object_queue            m_total_objects;
        size_type               m_grow_size;
        Mutex                   m_mtx;
    };
}   // end namespace Utility

#endif