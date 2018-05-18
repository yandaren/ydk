/**
 *
 * filesystem - directory_iterator
 *
 * c++11应该已经支持filesystem, 对于一些还没有支持filesytem特性的编译器，
 * 就暂时用这个文件
 *
 * 一些文件和文件夹相关操作接口
 *
 * @author  :   yandaren1220@126.com
 * @date    :   2018-05-11
 */

#ifndef __ydk_utility_filesystem_directory_iterator_hpp__
#define __ydk_utility_filesystem_directory_iterator_hpp__

#ifdef _WIN32
#include "directory_iterator_win.hpp"
#elif defined(__linux__)
#include "directory_iterator_linux.hpp"
#endif

#endif