/**
 * @brief cmdline parser
 *
 * @author  :   yandaren1220@126.com
 * @date    :   2018-07-27
 */

#ifndef __ydk_utility_cmdline_parser_hpp__
#define __ydk_utility_cmdline_parser_hpp__

#include <stdint.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

namespace utility
{
    namespace detail {
        template<typename Target, typename Source, bool Same>
        class type_caster{
        public:
            static Target cast(const Source& arg) {
                Target ret;
                std::stringstream ss;
                ss << arg;
                ss >> ret;
                return std::move(ret);
            }
        };

        template<typename Target, typename Source>
        class type_caster<Target, Source, true> {
        public:
            static Target cast(const Source& arg) {
                return arg;
            }
        };

        template<typename Source>
        class type_caster<std::string, Source, false> {
        public:
            static std::string cast(const Source& arg) {
                return std::move(std::to_string(arg));
            }
        };

        template <typename T1, typename T2>
        struct is_same {
            static const bool value = false;
        };

        template <typename T>
        struct is_same<T, T> {
            static const bool value = true;
        };

        template<typename Target, typename Source>
        Target type_cast(const Source &arg)
        {
            return type_caster<Target, Source, detail::is_same<Target, Source>::value>::cast(arg);
        }
    }

    class cmdline_parser
    {
    protected:
        /** <param_name > */
        typedef std::unordered_set<std::string>                 param_set_type;
        /** <param_name,  param_value> */
        typedef std::unordered_map<std::string, std::string>    param_kv_map_type;
        /** <param_name, param_value> */
        typedef std::pair<std::string, std::string>             param_kv_type;
        typedef std::vector<param_kv_type>                      param_list_type;

    protected:
        param_set_type          total_params_set_;
        param_kv_map_type       param_kv_map_;
        param_list_type         param_list_;

    public:
        cmdline_parser(){
        }

        ~cmdline_parser() {
        }

    public:
        void    initialize(int32_t argc, char* argv[]) {
            for (int32_t i = 0; i < argc; ++i) {
                std::string param_kv = argv[i];

                if (param_kv.find('-') != std::string::npos) {
                    param_kv = param_kv.substr(1);
                }

                std::string::size_type equal_pos = param_kv.find('=');
                if (equal_pos != std::string::npos) {
                    std::string key = param_kv.substr(0, equal_pos);
                    std::string value = param_kv.substr(equal_pos + 1);

                    total_params_set_.insert(key);
                    param_list_.push_back(std::make_pair(key, value));

                    param_kv_map_[key] = value;
                }
                else {
                    total_params_set_.insert(param_kv);
                    param_list_.push_back(std::make_pair(param_kv, ""));
                }
            }
        }

        int32_t     params_count() const {
            return (int32_t)param_list_.size();
        }

        std::string get_param(int32_t index) {
            if (index >= 0 && index < (int32_t)param_list_.size()) {
                return param_list_[index].first;
            }
            return "";
        }

        bool        has_param(const std::string& p) const {
            return total_params_set_.find(p) != total_params_set_.end();
        }

        bool        has_value(const std::string& p) const {
            return param_kv_map_.find(p) != param_kv_map_.end();
        }

        bool        has_value(int32_t index) const {
            const std::string* v = get_param_value_by_index(index);
            return v && !v->empty();
        }

        std::string get_value(const std::string& p) const {
            const std::string* v = get_param_value(p);
            if (v) {
                return *v;
            }
            return "";
        }

        std::string  get_value(int32_t index) {
            if (index >= 0 && index < (int32_t)param_list_.size()) {
                return param_list_[index].second;
            }
            return "";
        }

        int32_t     get_value_int32(const std::string& p) const {
            const std::string* v = get_param_value(p);
            if (!v) {
                return 0;
            }

            return detail::type_cast<int32_t>(*v);
        }

        int32_t     get_value_int32(int32_t index) const {
            const std::string* v = get_param_value_by_index(index);
            if (!v) {
                return 0;
            }

            return detail::type_cast<int32_t>(*v);
        }

        uint32_t    get_value_uint32(const std::string& p) const {
            const std::string* v = get_param_value(p);
            if (!v) {
                return 0;
            }

            return detail::type_cast<uint32_t>(*v);
        }

        uint32_t    get_value_uint32(int32_t index) const {
            const std::string* v = get_param_value_by_index(index);
            if (!v) {
                return 0;
            }

            return detail::type_cast<uint32_t>(*v);
        }

        int64_t     get_value_int64(const std::string& p) const {
            const std::string* v = get_param_value(p);
            if (!v) {
                return 0;
            }

            return detail::type_cast<int64_t>(*v);
        }

        int64_t     get_value_int64(int32_t index) const {
            const std::string* v = get_param_value_by_index(index);
            if (!v) {
                return 0;
            }

            return detail::type_cast<int64_t>(*v);
        }

        uint64_t    get_value_uint64(const std::string& p) const {
            const std::string* v = get_param_value(p);
            if (!v) {
                return 0;
            }

            return detail::type_cast<uint64_t>(*v);
        }

        uint64_t    get_value_uint64(int32_t index) const {
            const std::string* v = get_param_value_by_index(index);
            if (!v) {
                return 0;
            }

            return detail::type_cast<uint64_t>(*v);
        }

        float       get_value_float(const std::string& p) const {
            const std::string* v = get_param_value(p);
            if (!v) {
                return 0;
            }

            return detail::type_cast<float>(*v);
        }

        float       get_value_float(int32_t index) const {
            const std::string* v = get_param_value_by_index(index);
            if (!v) {
                return 0;
            }

            return detail::type_cast<float>(*v);
        }

        double      get_value_double(const std::string& p) const {
            const std::string* v = get_param_value(p);
            if (!v) {
                return 0;
            }

            return detail::type_cast<double>(*v);
        }

        double      get_value_double(int32_t index) const {
            const std::string* v = get_param_value_by_index(index);
            if (!v) {
                return 0;
            }

            return detail::type_cast<double>(*v);
        }

    protected:
        const std::string* get_param_value(const std::string& p) const {
            auto iter = param_kv_map_.find(p);
            if (iter != param_kv_map_.end()) {
                return &iter->second;
            }
            return nullptr;
        }

        const std::string* get_param_value_by_index(int32_t index) const {
            if (index >= 0 && index < (int32_t)param_list_.size()) {
                if (!param_list_[index].second.empty()) {
                    return &param_list_[index].second;
                }
            }
            return nullptr;
        }
    };
}   // utility

#endif