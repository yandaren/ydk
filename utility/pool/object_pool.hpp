/**
 *
 * a object pool.hpp
 *
 *
 * @author  :   yandaren1220@126.com
 * @date    :   2017-06-20
 */

#ifndef __ydk_utility_pool_object_pool_hpp__
#define __ydk_utility_pool_object_pool_hpp__

#include "object_allocator.hpp"
#include "utility/sync/null_mutex.hpp"

namespace utility
{
    /**
     * @brief 
     * object pool, try to decrease the cost that allocate memory and free memory
     * try to reuse the object memory as far as possible
     */

    template< typename T, typename Alloctor = object_allocator<T, utility::sync::null_mutex> >  
    class object_pool
    {
    public:
        typedef uint32_t    size_type;
        
        /** 
         * @brief 
         * @param init_count:  the initial object count to allocate
         * @param grow_count:  the grow size when object not enough
         */
        object_pool(size_type init_count, size_type grow_size)
            :m_allocator(init_count, grow_size)
        {

        }

        /** 
         * @brief 
         * allocate a object, the construct has no param
         * @return the object pointer, return null when failed
         */
        inline T* allocate()
        {
            return new ( m_allocator.allocate())T;
        }

        /** 
         * @brief 
         * allocate a object, the construct has 1 param
         * @return the object pointer, return null when failed
         */
        template<typename P1>
        inline T* allocate(P1 p1)
        {
            return new ( m_allocator.allocate())T(p1);
        }

        /** 
         * @brief 
         * allocate a object, the construct has 2 param
         * @return the object pointer, return null when failed
         */
        template<typename P1, typename P2>
        inline T* allocate(P1 p1, P2 p2)
        {
            return new ( m_allocator.allocate())T(p1, p2);
        }

        /** 
         * @brief 
         * allocate a object, the construct has 3 param
         * @return the object pointer, return null when failed
         */
        template<typename P1, typename P2, typename P3>
        inline T* allocate(P1 p1, P2 p2, P3 p3)
        {
            return new ( m_allocator.allocate())T(p1, p2, p3);
        }

        /** 
         * @brief 
         * allocate a object, the construct has 4 param
         * @return the object pointer, return null when failed
         */
        template<typename P1, typename P2, typename P3, typename P4>
        inline T* allocate(P1 p1, P2 p2, P3 p3, P4 p4)
        {
            return new ( m_allocator.allocate())T(p1, p2, p3, p4);
        }

        /** 
         * @brief 
         * allocate a object, the construct has 5 param
         * @return the object pointer, return null when failed
         */
        template<typename P1, typename P2, typename P3, typename P4, typename P5>
        inline T* allocate(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
        {
            return new ( m_allocator.allocate())T(p1, p2, p3, p4, p5);
        }

        /** 
         * @brief 
         * reclaim the object to the pool
         * @param p object pointer
         */
        void       free(T* p)
        {
            p->~T();
            m_allocator.reclaim(p);
        }

        /**
         * @brief 
         * return the allocator
         */
        Alloctor& get_allocator()
        {
            return m_allocator;
        }

        /** 
         * @brief 
         * return the free object count
         */
        inline size_type free_object_count()
        {
            return m_allocator.free_object_count();
        }

        inline size_type free_memory_size()
        {
            return m_allocator.free_memory_size();
        }

        inline size_type total_object_count()
        {
            return m_allocator.total_object_count();
        }

        inline size_type total_memory_size()
        {
            return m_allocator.total_memory_size();
        }


    private:
        Alloctor m_allocator;    
    };

} // end namespace utility

#endif