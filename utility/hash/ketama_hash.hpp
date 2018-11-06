/**
 *
 * ketama_hash.hpp
 *
 * a consistent hashing algorithm
 *
 * @author  :   yandaren1220@126.com
 * @date    :   2017-06-20
 */

#ifndef __ydk_utility_hash_ketama_hash_hpp__
#define __ydk_utility_hash_ketama_hash_hpp__

#include "hash_util.hpp"
#include "node.hpp"
#include <cassert>
#include <cstdint>
#include <map>
#include <mutex>
#include <functional>

namespace utility
{
namespace consistent_hashing
{

/* 默认的虚拟节点数 */
static const uint32_t default_virtual_node_count = 160;

typedef std::function<uint32_t(const char*)> hash_func_type;

class ketama_hash
{
protected:
    hash_func_type                      hash_func_;
    int32_t                             virtual_node_count_;

    /* <identifier, node> */
    std::map<std::string, node_ptr>     node_map_;

    /* <hash_value, identifier> */
    std::map<uint32_t, std::string>     hash_map_;

    std::mutex                          map_mtx_;

public:
    ketama_hash()
        : virtual_node_count_(default_virtual_node_count)
    {
        hash_func_ = hash_util::rs_hash;
    }

    ketama_hash(uint32_t virtual_node_cout)
        : virtual_node_count_(virtual_node_cout)
    {
        hash_func_ = hash_util::rs_hash;
    }

    ketama_hash(uint32_t virtual_node_cout, const hash_func_type& hash_func)
        : virtual_node_count_(virtual_node_cout)
        , hash_func_(hash_func)
    {
    }

    void    set_hash_func(const hash_func_type& hash_func)
    {
        hash_func_ = hash_func;
    }

public:
    void    add_node(node_ptr nd)
    {
        std::lock_guard<std::mutex> locker(map_mtx_);
        char buf[128] = { 0 };
        for (int32_t i = 0; i < virtual_node_count_; ++i){
            /* virtual node key */
            sprintf(buf, "%s_%d", nd->identifer().c_str(), i);
            uint32_t key = hash_func_(buf);
            hash_map_[key] = nd->identifer();
        }

        node_map_[nd->identifer()] = nd;
    }

    void    remove_node(node_ptr nd){
        remove_node(nd->identifer().c_str());
    }

    void    remove_node(const char* node_identifer){
        std::lock_guard<std::mutex> locker(map_mtx_);
        auto iter = node_map_.find(node_identifer);
        if (iter != node_map_.end()){
            node_ptr nd = iter->second;
            char buf[128] = { 0 };
            for (int32_t i = 0; i < virtual_node_count_; ++i){
                /* virtual node key */
                sprintf(buf, "%s_%d", nd->identifer().c_str(), i);
                uint32_t key = hash_func_(buf);
                hash_map_.erase(key);
            }

            node_map_.erase(iter);
        }
    }

    uint32_t node_count(){
        std::lock_guard<std::mutex> locker(map_mtx_);

        return (uint32_t)node_map_.size();
    }

    uint32_t hash_map_size(){
        std::lock_guard<std::mutex> locker(map_mtx_);

        return (uint32_t)hash_map_.size();
    }

    uint32_t node_replicas(){
        return virtual_node_count_;
    }

    /** 
     * @brief get the node_ptr for the object
     */
    node_ptr get_node_for_key(const char* key){
        uint32_t hash_key = hash_func_(key);
        std::lock_guard<std::mutex> locker(map_mtx_);
        auto iter = hash_map_.lower_bound(hash_key);
        if (iter != hash_map_.end()){
            auto n_iter = node_map_.find(iter->second);
            if (n_iter != node_map_.end()){
                return n_iter->second;
            }
            else{
                // never happen
                assert(false);
            }
        }
        else{
            if (!node_map_.empty()){
                return node_map_.begin()->second;
            }
        }

        return node_ptr();
    }
};
}
}

#endif