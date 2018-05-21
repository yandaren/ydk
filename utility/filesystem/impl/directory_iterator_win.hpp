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

#ifndef __ydk_utility_filesystem_directory_iterator_win_hpp__
#define __ydk_utility_filesystem_directory_iterator_win_hpp__

#include <io.h>
#include <string>

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
        _finddata_t     m_find_data;

    public:
        file_data_t() {
        }

        ~file_data_t() {
        }

    public:
        std::string name() {
            return m_find_data.name;
        }

        bool    is_directory() {
            return (m_find_data.attrib & _A_SUBDIR) > 0 ;
        }

        bool    is_file() {
            return !is_directory();
        }

    };

    class directory_iterator
    {
    protected:
        intptr_t                     m_handle;
        file_data_t*                 m_cur_file_data;
        bool                         m_eof;

    public:
        directory_iterator()
            : m_handle(-1)
            , m_cur_file_data(nullptr)
            , m_eof(true){
        }

        directory_iterator(const char* path) {
            if (path) {
                std::string search_path = path;
                search_path.append("/*.*");

                m_cur_file_data = new file_data_t();

                m_handle = _findfirst(search_path.c_str(), &m_cur_file_data->m_find_data);

                m_eof = (m_handle == -1);
            }
            else {
                m_handle = -1;
                m_cur_file_data = nullptr;
                m_eof = true;
            }
        }

        ~directory_iterator() {
            if (m_handle != -1) {
                _findclose(m_handle);
                m_handle = -1;
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
            if (m_eof || m_handle == -1 || !m_cur_file_data) {
                return;
            }

            m_eof = (_findnext(m_handle, &m_cur_file_data->m_find_data) != 0);
        }
    };
}
}

#endif