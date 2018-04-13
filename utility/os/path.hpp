/**
 *
 * File path.hpp
 * 一些os.path相关的接口
 *
 * @author  :   yandaren1220@126.com
 * @date    :   2017-03-16
 */

#ifndef __ydk_utility_os_path_hpp__
#define __ydk_utility_os_path_hpp__

#include <stdint.h>
#include <string>
#include <filesystem>
#include <deque>
#include <str.hpp>

namespace ydk
{
namespace os
{
namespace fs = std::tr2::sys;
namespace path
{
    /** 
     * @brief split the file's path and filename
     */
    static std::pair<std::string, std::string> split(const char* file_name)
    {
        fs::path pt(file_name);
        return std::make_pair(pt.parent_path().string(), pt.filename());
    }

    /** 
     * @brief split the filename and the extension
     */
    static std::pair<std::string, std::string> splitext(const char* file_name)
    {
        fs::path pt(file_name);
        return std::make_pair(pt.basename(), pt.extension());
    }

    /** 
     * @brief check the path is a file
     */
    static bool isfile(const char* file_name)
    {
        fs::path pt(file_name);
        return fs::is_regular_file(pt);
    }

    /** 
     * @brief check the path is a directory
     */
    static bool isdir(const char* file_name)
    {
        fs::path pt(file_name);
        return fs::is_directory(pt);
    }

    /** 
     * @brief check the path exist
     */
    static bool exist(const char* file_name)
    {
        fs::path pt(file_name);
        return fs::exists(pt);
    }

    /** 
     * @brief get file size
     */
    static uint64_t file_size(const char* file_name)
    {
        fs::path pt(file_name);
        return fs::file_size(pt);
    }

    /** 
     * @brief get the absolutely path
     */
    static std::string abspath(const char* path_name)
    {
        fs::path pt(path_name);
        return fs::system_complete(pt).string();
    }

    /** 
     * @brief check the path is absolutely path
     */
    static bool isabs(const char* path_name)
    {
        fs::path pt(path_name);
        return pt.is_complete();
    }

    /** 
     * @brief nomalize the path
     */
    static std::string normpath(const char* path_name)
    {
        fs::path pt(path_name);
        fs::path result = pt.root_path();
        std::string root_string = result.string();
        std::deque <std::string> stack;
        int32_t i = 0;
        for (auto e : pt.relative_path())
        {
            if (e == "..")
            {
                if (i > 0 && !stack.empty() && stack.back() != "..")
                {
                    stack.pop_back();
                }
                else if (i == 0 && str::string_endwith(root_string, "/"))
                {
                    ;
                }
                else
                {
                    stack.push_back(e);
                }
            }
            else if (e != ".")
            {
                stack.push_back(e);
            }

            i++;
        }

        for (auto e : stack)
        {
            result /= e;
        }

        return std::move(result.file_string());
    }
}
}

}

#endif