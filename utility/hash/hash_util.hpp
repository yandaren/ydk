/**
 *
 * hash_util.hpp
 * 
 * some hash algorithm
 *
 * @author  :   yandaren1220@126.com
 * @date    :   2017-06-20
 */

#ifndef __ydk_utility_hash_hash_util_hpp__
#define __ydk_utility_hash_hash_util_hpp__

#include <cstdint>

namespace utility
{
namespace hash_util
{
    static uint32_t sdbm_hash(const char *str)
    {
        uint32_t hash = 0;

        while (*str)
        {
            // equivalent to: hash = 65599*hash + (*str++);
            hash = (*str++) + (hash << 6) + (hash << 16) - hash;
        }

        return (hash & 0x7FFFFFFF);
    }

    // RS Hash Function
    static uint32_t rs_hash(const char *str)
    {
        uint32_t b = 378551;
        uint32_t a = 63689;
        uint32_t hash = 0;

        while (*str)
        {
            hash = hash * a + (*str++);
            a *= b;
        }

        return (hash & 0x7FFFFFFF);
    }

    // JS Hash Function
    static uint32_t js_hash(const char *str)
    {
        uint32_t hash = 1315423911;

        while (*str)
        {
            hash ^= ((hash << 5) + (*str++) + (hash >> 2));
        }

        return (hash & 0x7FFFFFFF);
    }

    // ELF Hash Function
    static uint32_t elf_hash(const char *str)
    {
        uint32_t hash = 0;
        uint32_t x = 0;

        while (*str)
        {
            hash = (hash << 4) + (*str++);
            if ((x = hash & 0xF0000000L) != 0)
            {
                hash ^= (x >> 24);
                hash &= ~x;
            }
        }

        return (hash & 0x7FFFFFFF);
    }

    // BKDR Hash Function
    static uint32_t bkdr_hash(const char *str)
    {
        uint32_t seed = 131; // 31 131 1313 13131 131313 etc..
        uint32_t hash = 0;

        while (*str)
        {
            hash = hash * seed + (*str++);
        }

        return (hash & 0x7FFFFFFF);
    }

    // DJB Hash Function
    static uint32_t djb_hash(const char *str)
    {
        uint32_t hash = 5381;

        while (*str)
        {
            hash += (hash << 5) + (*str++);
        }

        return (hash & 0x7FFFFFFF);
    }

    // AP Hash Function
    static uint32_t ap_hash(const char *str)
    {
        uint32_t hash = 0;
        int32_t i;

        for (i = 0; *str; i++)
        {
            if ((i & 1) == 0)
            {
                hash ^= ((hash << 7) ^ (*str++) ^ (hash >> 3));
            }
            else
            {
                hash ^= (~((hash << 11) ^ (*str++) ^ (hash >> 5)));
            }
        }

        return (hash & 0x7FFFFFFF);
    }
}
}

#endif