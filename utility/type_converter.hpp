/**
 *
 * 类型转换器
 *
 * @author  :   yandaren1220@126.com
 * @date    :   2018-10-24
 */

#ifndef __ydk_utility_type_converter_hpp__
#define __ydk_utility_type_converter_hpp__

#include <string>
#include <sstream>

namespace utility
{
    namespace type_converter {
        template<typename Target, typename Source, bool Same>
        class type_caster {
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
            return type_caster<Target, Source, type_converter::is_same<Target, Source>::value>::cast(arg);
        }
    }
}

#endif