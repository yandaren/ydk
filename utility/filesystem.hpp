/**
 *
 * filesystem
 * 
 * c++11应该已经支持filesystem, 对于一些还没有支持filesytem特性的编译器，
 * 就暂时用这个文件
 *
 * 一些文件和文件夹相关操作借口
 *
 * @author  :   yandaren1220@126.com
 * @date    :   2018-04-17
 */

#ifndef __ydk_utility_filesystem_hpp__
#define __ydk_utility_filesystem_hpp__

#ifdef _WIN32
#include <io.h>
#include <direct.h> 
#elif defined(__linux__)
#include <unistd.h>
#include <sys/stat.h>
#endif

#include <stdint.h>
#include <string>
#define MAX_PATH_LEN 256

#ifdef _WIN32
#define access _access
#endif

namespace utility
{
namespace filesystem
{

/** 
 * @brief 文件是否存在
 */
static bool exist(const char* file_name) {
	return access(file_name, 0) == 0;
}

/** 
 * @brief 创建文件夹
 */
static bool create_directory(const char* path) {
#ifdef _WIN32
	return _mkdir(path) == 0;
#elif defined(__linux__)
	return mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0;
#else
#error "create_directory unsupported platform"
	return false;
#endif
}

/** 
 * @brief 创建多级文件夹目录
 */
static bool create_directories(const char* path) {
	char path_tmp[MAX_PATH_LEN] = { 0 };
	uint32_t len = (uint32_t)strlen(path);
	memcpy(path_tmp, path, len);

	if (path_tmp[len - 1] != '\\' && path_tmp[len - 1] != '/') {
		path_tmp[len] = '/';
	}

	for (uint32_t i = 0; i <= len; ++i) {
		if (path_tmp[i] == '\\' || path_tmp[i] == '/') {
			path_tmp[i] = '\0';
			if (!exist(path_tmp)) {
				if (!create_directory(path_tmp)) {
					return false;
				}
			}
			path_tmp[i] = '/';
		}
	}
	return true;
}

}
}

#endif