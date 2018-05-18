/**
*
* filesystem - directory_iterator
*
* c++11应该已经支持filesystem, 对于一些还没有支持filesytem特性的编译器，
* 就暂时用这个文件
*
* 一些文件和文件夹相关操作借口
*
* @author  :   yandaren1220@126.com
* @date    :   2018-05-11
*/

#ifndef __ydk_utility_filesystem_directory_iterator_linux_hpp__
#define __ydk_utility_filesystem_directory_iterator_linux_hpp__

#include <stdio.h>
#include <string>
#include <dirent.h>

namespace utility
{
    namespace filesystem
    {
        class directory_iterator;
        class file_data_t
        {
        public:
            friend class directory_iterator;
        protected:
            struct dirent*  m_file_data;

        public:
            file_data_t():m_file_data(nullptr){
            }

            ~file_data_t() {
            }

        public:
            std::string name() {
                if (m_file_data) {
                    return m_file_data->d_name;
                }
                else {
                    return "";
                }
            }

            bool    is_directory() {
                if (m_file_data) {
                    return m_file_data->d_type == 4;
                }
                else {
                    return false;
                }
            }

            bool    is_file() {
                if (m_file_data) {
                    // 8, file; 10, link file
                    return (m_file_data->d_type == 8 || m_file_data->d_type == 10);
                }
                else {
                    return false;
                }
            }

        };

        class directory_iterator
        {
        protected:
            DIR*                         m_dir;
            file_data_t*                 m_cur_file_data;
            bool                         m_eof;

        public:
            directory_iterator()
                : m_dir(nullptr)
                , m_cur_file_data(nullptr)
                , m_eof(true) {
            }

            directory_iterator(const char* path) {
                if (path) {
                    m_cur_file_data = new file_data_t();

                    m_dir = opendir(path);

                    m_eof = (m_dir == NULL);

                    if (!m_eof) {
                        m_cur_file_data->m_file_data = readdir(m_dir);
                        m_eof = (m_cur_file_data->m_file_data == NULL);
                    }
                }
                else {
                    m_dir = NULL;
                    m_cur_file_data = nullptr;
                    m_eof = true;
                }
            }

            ~directory_iterator() {
                if (m_dir != NULL){
                    closedir(m_dir);
                    m_dir = NULL;
                }

                if (m_cur_file_data) {
                    delete m_cur_file_data;
                    m_cur_file_data = nullptr;
                }
            }

        public:
            file_data_t* get_file_data() {
                return m_cur_file_data;
            }

            directory_iterator& operator++() {
                get_next();
                return *this;
            }

            bool    eof() const {
                return m_eof;
            }

        protected:
            void    get_next() {
                if (m_eof || !m_dir || !m_cur_file_data) {
                    return;
                }

                m_cur_file_data->m_file_data = readdir(m_dir);
                m_eof = (m_cur_file_data->m_file_data == NULL);
            }
        };
    }
}

#endif
