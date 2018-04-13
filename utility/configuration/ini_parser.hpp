/**
 *
 * a simeple ini parser
 *
 *
 * @author  :   yandaren1220@126.com
 * @date    :   2017-06-25
 */

#ifndef __ydk_utility_configuration_ini_parser_hpp__
#define __ydk_utility_configuration_ini_parser_hpp__

#include <unordered_map>
#include <string>
#include <cstdint>
#include <cstdio>
#include <mutex>
#include <sstream>
#include <utility/sync/null_mutex.hpp>

namespace utility
{
template<class Mutex = utility::sync::null_mutex >
class ini_parser
{
protected:
    std::unordered_map<std::string, std::string> key_value_map_;
    Mutex                                        mtx_;
public:
    /** 
     * @brief parser from file
     */
    bool parse(const char* file_name)
    {
        std::lock_guard<Mutex> locker(mtx_);

        FILE* file = fopen(file_name, "r");
        if (!file){
            return false;
        }

        bool result = parse(file);
        fclose(file);
        return result;
    }

    /** 
     * @brief printf all the result
     */
    void print(){
        std::lock_guard<Mutex> locker(mtx_);

        printf("ini_config result\n");
        for (auto kv : key_value_map_){
            printf("[%s]=%s\n", kv.first.c_str(), kv.second.c_str());
        }
        printf("\n");
    }

    /** 
     * @brief get the config value
     */
    int32_t get_int32(const char* value, int32_t defualt = 0)
    {
        std::lock_guard<Mutex> locker(mtx_);

        int32_t ret_v = defualt;

        std::string* v = get(value);
        if (v){
            std::istringstream is(*v);
            is >> ret_v;
        }

        return ret_v;
    }

    int64_t get_int64(const char* value, int64_t default = 0)
    {
        std::lock_guard<Mutex> locker(mtx_);

        int64_t ret_v = default;

        std::string* v = get(value);
        if (v){
            std::istringstream is(*v);
            is >> ret_v;
        }

        return ret_v;
    }

    float  get_float(const char* value, float default = 0)
    {
        std::lock_guard<Mutex> locker(mtx_);

        float ret_v = default;

        std::string* v = get(value);
        if (v){
            std::istringstream is(*v);
            is >> ret_v;
        }

        return ret_v;
    }

    std::string get_string(const char* value, std::string default = "")
    {
        std::lock_guard<Mutex> locker(mtx_);

        std::string ret_v = default;

        std::string* v = get(value);
        if (v){
            ret_v = *v;
        }

        return std::move(ret_v);
    }

protected:
    std::string* get(const char* value)
    {
        auto iter = key_value_map_.find(value);
        if (iter != key_value_map_.end()){
            return &iter->second;
        }
        return nullptr;
    }

    bool        parse(FILE* file){
        std::string app_name;
        std::string item_name;
        std::string item_value;

        char buffer[512] = { 0 };
        bool is_first_line = true;
        while (fgets(buffer, 512, file)){
            char* buf = (char*)buffer;
            int32_t len = (int32_t)strlen(buf);
            if (len == 0){
                continue;
            }

            if (is_first_line){
                // skip bom header
                if (len >= 3 &&
                    buf[0] == '\xEF' &&
                    buf[1] == '\xBB' &&
                    buf[2] == '\xBF'){
                    buf = buf + 3;
                    len -= 3;
                }

                is_first_line = false;
            }

            if (buf[0] == '['){
                int32_t j = len - 1;
                while (j > 0 && buf[j] != ']') j--;

                if (buf[j] == ']'){
                    app_name = std::move(std::string(buf + 1, j - 1));
                }
                else{
                    // parser failed
                    return false;
                }
            }
            else{
                for (int32_t i = 0; i < len; ++i){
                    if (buf[i] == '='){
                        std::string item_name_tmp 
                            = std::move(std::string(buf, i));

                        int32_t k = (int32_t)(item_name_tmp.size() - 1);
                        while (k >= 0 && item_name_tmp[k] == ' ') k--;
                        item_name = item_name_tmp.substr(0, k + 1);

                        std::string value_tmp = i < len - 1 ?
                            std::move(std::string(buf + i + 1, len - i - 1)) :
                            std::move(std::string(""));

                        // skip /r/n
                        k = (int32_t)(value_tmp.size() - 1);
                        while (k >= 0 && (value_tmp[k] == '\r' || value_tmp[k] == '\n')) k--;
                        item_value = value_tmp.substr(0, k + 1);

                        // save the result
                        std::string key = app_name + std::string(".") + item_name;
                        key_value_map_[key] = item_value;
                        break;
                    }
                }
            }
        }

        return true;
    }
};
}

#endif