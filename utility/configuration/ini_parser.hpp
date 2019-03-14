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
#include <set>
#include <map>
#include <string.h>
#include <string>
#include <cstdint>
#include <cstdio>
#include <mutex>
#include <sstream>
#include <vector>
#include <utility/str.hpp>
#include <utility/sync/null_mutex.hpp>

namespace utility
{
template<class Mutex = utility::sync::null_mutex >
class ini_parser
{
protected:
    /** <app_name.item_key, value > */
    std::unordered_map<std::string, std::string> key_value_map_;

    /** <app_name, item_key> */
    std::map<std::string, std::set<std::string>> app_name_item_key_set_map_;

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
    int32_t get_int32(const char* key, int32_t defualt = 0)
    {
        std::lock_guard<Mutex> locker(mtx_);

        int32_t ret_v = defualt;

        std::string* v = get(key);
        if (v){
            std::istringstream is(*v);
            is >> ret_v;
        }

        return ret_v;
    }

    void    set_int32(const char* key, int32_t value) {
        std::lock_guard<Mutex> locker(mtx_);

        std::string value_str(std::move(std::to_string(value)));
        std::string* v = get_or_create(key);
        if (v) {
            *v = value_str;
        }
    }

    int64_t get_int64(const char* key, int64_t dfault = 0)
    {
        std::lock_guard<Mutex> locker(mtx_);

        int64_t ret_v = dfault;

        std::string* v = get(key);
        if (v){
            std::istringstream is(*v);
            is >> ret_v;
        }

        return ret_v;
    }

    void    set_int64(const char* key, int64_t value) {
        std::lock_guard<Mutex> locker(mtx_);

        std::string value_str(std::move(std::to_string(value)));
        std::string* v = get_or_create(key);
        if (v) {
            *v = value_str;
        }
    }

    float  get_float(const char* key, float dfault = 0)
    {
        std::lock_guard<Mutex> locker(mtx_);

        float ret_v = dfault;

        std::string* v = get(key);
        if (v){
            std::istringstream is(*v);
            is >> ret_v;
        }

        return ret_v;
    }

    void    set_float(const char* key, float value) {
        std::lock_guard<Mutex> locker(mtx_);

        std::string value_str(std::move(std::to_string(value)));
        std::string* v = get_or_create(key);
        if (v) {
            *v = value_str;
        }
    }

    std::string get_string(const char* key, std::string dfault = "")
    {
        std::lock_guard<Mutex> locker(mtx_);

        std::string ret_v = dfault;

        std::string* v = get(key);
        if (v){
            ret_v = *v;
        }

        return std::move(ret_v);
    }

    void    set_string(const char* key, const char* value) {
        std::lock_guard<Mutex> locker(mtx_);

        std::string* v = get_or_create(key);
        if (v) {
            *v = value;
        }
    }

    template<typename T>
    std::vector<T> get_list(const char* key, const char* separator) {
        std::lock_guard<Mutex> locker(mtx_);

        std::vector<T> ret_arr;
        std::vector<std::string> v_arr = get_list_inner(key, separator);
        for (auto& e : v_arr) {
            auto e1 = utility::str::trim_space(e);
            T v;
            std::istringstream is(e1);
            is >> v;
            ret_arr.push_back(v);
        }

        return ret_arr;
    }

    bool    save(const char* file_name) {

        std::lock_guard<Mutex> locker(mtx_);

        FILE* file = fopen(file_name, "w+");
        if (!file) {
            return false;
        }

        for (auto& app_item_key_set_kv : app_name_item_key_set_map_) {
            const std::string& app_name = app_item_key_set_kv.first;
            fprintf(file, "[%s]\n", app_name.c_str());

            for (auto& item_name : app_item_key_set_kv.second) {
                std::string item_real_key = app_name;
                item_real_key.append(".").append(item_name);
                const std::string& value = key_value_map_[item_real_key];
                fprintf(file, "%s = %s\n", item_name.c_str(), value.c_str());
            }

            fprintf(file, "\n");
        }

        fflush(file);
        fclose(file);

        return true;
    }

protected:
    std::string* get(const char* key)
    {
        auto iter = key_value_map_.find(key);
        if (iter != key_value_map_.end()){
            return &iter->second;
        }
        return nullptr;
    }

    std::vector<std::string> get_list_inner(const char* key, const char* separator) {
        std::vector<std::string> ret;
        std::string* v = get(key);
        if (v) {
            utility::str::string_splits(v->c_str(), separator, ret);
        }

        return std::move(ret);
    }

    std::string* get_or_create(const char* key) {
        std::string in_key(key);
        std::string::size_type pos = in_key.find(".");
        if (pos == std::string::npos) {
            return nullptr;
        }

        std::string app_name = in_key.substr(0, pos);
        std::string item_name = in_key.substr(pos + 1, in_key.size() - pos - 1);
        if (item_name.empty()) {
            return nullptr;
        }

        if (!app_name_item_key_set_map_[app_name].count(item_name)) {
            app_name_item_key_set_map_[app_name].insert(item_name);
        }

        std::string& ret = key_value_map_[in_key];

        return &ret;
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

            if (buf[0] == '#') {
                continue;
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
                        // trim ' '
                        // start skip ' '
                        int32_t ks = 0;
                        while (ks < i && buf[ks] == ' ') ks++;
                        // end skip ' '
                        int32_t ke = i - 1;
                        if (ke >= 0 && buf[ke] == ' ') ke--;
                        if (ks > ke) break;
                        std::string item_name = std::move(std::string(buf + ks, ke - ks + 1));

                        // start skip ' '
                        ks = i + 1;
                        while (ks < len && buf[ks] == ' ') ks++;

                        // end skip /r/n
                        ke = len - 1;
                        while (ke >= 0 && (buf[ke] == '\r' || buf[ke] == '\n' || buf[ke] == ' ')) ke--;
                        if (ks > ke) break;
                        std::string item_value = std::move(std::string(buf + ks, ke - ks + 1));

                        // save the result
                        std::string key = app_name + std::string(".") + item_name;
                        key_value_map_[key] = item_value;

                        app_name_item_key_set_map_[app_name].insert(item_name);

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
