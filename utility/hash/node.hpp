/**
 *
 * node.hpp
 *
 * the node of the consist hash( mainly the machine)
 *
 * @author  :   yandaren1220@126.com
 * @date    :   2017-06-20
 */

#ifndef __ydk_utility_hash_node_hpp__
#define __ydk_utility_hash_node_hpp__

#include <cstdint>
#include <memory>
#include <string>

namespace utility
{
namespace consistent_hashing
{
class node;
typedef std::shared_ptr<node> node_ptr;
class node
{
protected:
    std::string ip_;
    uint32_t    port_;
    std::string identifier_;

public:
    node(const char* ip, uint32_t port)
        : ip_(ip)
        , port_(port)
    {
        char buf[64] = { 0 };
        sprintf(buf, "%s:%d", ip_.c_str(), port_);
        identifier_ = buf;
    }

    static node_ptr create(const char* ip, uint32_t port)
    {
        return std::make_shared<node>(ip, port);
    }

    const std::string& ip(){
        return ip_;
    }

    uint32_t port(){
        return port_;
    }

    const std::string& identifer(){
        return identifier_;
    }
};
}
}

#endif