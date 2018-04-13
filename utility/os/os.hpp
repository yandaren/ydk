/** 
 * 
 * File os.hpp
 * 一些os相关的接口
 *
 * @author  :   yandaren1220@126.com
 * @date    :   2017-03-16
 */

#ifndef __ydk_utility_os_os_hpp__
#define __ydk_utility_os_os_hpp__

#include <stdint.h>
#include <stdlib.h>
#include <io.h>
#include <string>
#include <filesystem>
#include <vector>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif
#include <direct.h>
#include <Windows.h>
#elif defined(__linux__)
#include <unistd.h>
#endif

namespace ydk
{
namespace os
{

    static const char           path_sep_win = '\\';
    static const char           path_sep_linux = '/';
    static const std::string    line_sep_win = "\r\n";
    static const std::string    line_sep_linux = "\n";

    namespace fs = std::tr2::sys;

    /** 
     * @brief return the os path seperator
     */
    static const char& path_sep()
    {
#ifdef _WIN32
        return path_sep_win;
#else
        return path_sep_linux;
#endif
    }

    /** 
     * @brief return the os line seperator
     */
    static const std::string& line_sep()
    {
#ifdef _WIN32
        return line_sep_win;
#else
        return line_sep_linux;
#endif
    }

    /** 
     * @brief return current work dir
     */
    static std::string get_cwd()
    {
        fs::path pt = fs::current_path<fs::path>();
        return pt.file_string();
    }

    /** 
     * @brief set the current work dir
     */
    static void        set_cwd(const char* dir)
    {
        fs::path pt(dir);
        fs::current_path(pt);
    }

    /** 
     * @brief get the binary directory
     */
    static  std::string     get_bin_dir()
    {
#define MAX_PATH_LEN 260
        char path[MAX_PATH_LEN] = { 0 };
        int32_t len = 0;
#ifdef _WIN32
        ::GetModuleFileName(::GetModuleHandle(nullptr), path, MAX_PATH_LEN);
        len = strlen(path);
#else
        len = readlink("/proc/self/exe", path, MAX_PATH_LEN);
        if (len < 0 || len >= MAX_PATH_LEN)
            return std::string("");
#endif
        for (int32_t i = len - 1; i >= 0; --i)
        {
            if (path[i] == os::path_sep())
            {
                path[i + 1] = '\0';
                break;
            }
        }
#undef  MAX_PATH_LEN
        return path;
    }

    /** 
     * @brief set the environment variable
     */
    static  bool        set_env(const char* name, const char* value)
    {
#ifdef _WIN32
        return !!::SetEnvironmentVariable(name, value);
#else
        return setenv(name, value, 1) == 0;
#endif
    }

    /** 
     * @brief get hte environment variable
     */
    static  std::string get_env(const char* name)
    {
#ifdef _WIN32
        char buffer[1024] = { 0 };
        ::GetEnvironmentVariable(name, buffer, 1023);
        return buffer;
#else
        return getenv(name);
#endif
    }

    /** 
     * @brief list the specific path files
     */
    static std::vector<std::string> list_dirs(const char* path)
    {
        std::vector<std::string> file_list;
        fs::directory_iterator end_iter;
        for (fs::directory_iterator file_iter(path); file_iter != end_iter; ++file_iter)
        {
            file_list.push_back((*file_iter).path().leaf());
        }
        return file_list;
    }

    /**
    * @brief list the specific path files
    */
    static std::vector<std::string> list_dirs_complete_path(const char* path)
    {
        std::vector<std::string> file_list;
        fs::directory_iterator end_iter;
        for (fs::directory_iterator file_iter(path); file_iter != end_iter; ++file_iter)
        {
            fs::path pt = (*file_iter).path().leaf();
            pt = fs::system_complete(pt);
            file_list.push_back(pt.string());
        }
        return file_list;
    }

    /** 
     * @brief create directory
     */
    static bool create_dir(const char* path_name)
    {
        fs::path pt(path_name);
        return fs::create_directory(pt);
    }

    /** 
     * @brief create directorys
     */
    static bool create_dirs(const char* path_name)
    {
        fs::path pt(path_name);
        return fs::create_directories(pt);
    }

    /** 
     * @brief remove file
     */
    static bool remove(const char* path_name)
    {
        fs::path pt(path_name);
        return fs::remove(pt);
    }

    /** 
     * @brief remove directory
     */
    static bool remove_dir(const char* path_name)
    {
        fs::path pt(path_name);
        return fs::remove_directory(pt);
    }

    /** 
     * @brief remove directorys
     */
    static uint32_t remove_dirs(const char* path_name)
    {
        fs::path pt(path_name);
        return fs::remove_all(pt);
    }

    /** 
     * @brief rename old path to new path
     */
    static bool rename(const char* old_name, const char* new_name)
    {
        fs::path old_pt(old_name);
        fs::path new_pt(new_name);

        return fs::rename(old_pt, new_pt);
    }

    /** 
     * @brief get the file stat
     */
    static fs::file_type stat(const char* path)
    {
        fs::path pt(path);

        return fs::status(pt).type();
    }
}
}

#endif