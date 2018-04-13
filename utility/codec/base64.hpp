/**
 *
 * base64 encode and decode
 *
 *
 * @author  :   yandaren1220@126.com
 * @date    :   2017-07-10
 */

#ifndef __ydk_utility_codec_base64_hpp__
#define __ydk_utility_codec_base64_hpp__

#include <sstream>

namespace utility
{
namespace codec
{

    /** 
     * @brief base64 tab
     */
    static const unsigned char to_base64_tab[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    /** 
     * @brief base64 encode 
     */
    static std::string base64_encode(const std::string& in){
        std::stringstream ss;
        std::size_t cur_index = 0;
        std::size_t k = 0;
        while (cur_index < in.size()){
            k++;
            if (k == 3){
                ss << to_base64_tab[((in[cur_index - 2] >> 2) & 0x3f)];
                ss << to_base64_tab[(((in[cur_index - 2] << 4) & 0x30) | ((in[cur_index - 1] >> 4) & 0x0f))];
                ss << to_base64_tab[(((in[cur_index - 1] << 2) & 0x3c) | ((in[cur_index - 0] >> 6) & 0x03))];
                ss << to_base64_tab[((in[cur_index - 0] >> 0) & 0x3f)];
                k = 0;
            }

            cur_index++;
        }

        if (k == 1){
            cur_index = in.size() - 1;
            ss << to_base64_tab[((in[cur_index - 0] >> 2) & 0x3f)];
            ss << to_base64_tab[((in[cur_index - 0] << 4) & 0x30)];
            ss << "==";
        }
        else if (k == 2){
            cur_index = in.size() - 1;
            ss << to_base64_tab[((in[cur_index - 1] >> 2) & 0x3f)];
            ss << to_base64_tab[(((in[cur_index - 1] << 4) & 0x30) | ((in[cur_index - 0] >> 4) & 0x0f))];
            ss << to_base64_tab[((in[cur_index - 0] << 2) & 0x3c)];
            ss << "=";
        }

        return std::move(ss.str());
    }

    /** 
     * @brief unbase64 tab
     */
    static const unsigned char un_base64_tab[] = {
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
        64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
        64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    };

    /** 
     * @brief base64 decode
     */
    static std::string base64_decode(const std::string& in){
        std::stringstream ss;
        std::size_t cur_index = 0;
        std::size_t k = 0;
        std::string error_rt = "";

        // check size
        if (in.size() % 4 != 0){
            return std::move(error_rt);
        }

        while (cur_index < in.size()){
            k++;
            // check character value
            if (in[cur_index] > 127 || (in[cur_index] != '=' && un_base64_tab[in[cur_index]] > 63)){
                return std::move(error_rt);
            }

            if (in[cur_index] == '='){
                if (k == 3){
                    // next charator is not '='
                    if (cur_index + 1 >= in.size() || in[cur_index + 1] != '='){
                        return std::move(error_rt);
                    }

                    // two '='
                    ss << (unsigned char)(((un_base64_tab[in[cur_index - 2]] << 2) & 0xfc) | ((un_base64_tab[in[cur_index - 1]] >> 4) & 0x03));
                    ss << (unsigned char)(((un_base64_tab[in[cur_index - 1]] << 4) & 0xf0));
                }
                else if (k == 4){
                    // one '='
                    ss << (unsigned char)(((un_base64_tab[in[cur_index - 3]] << 2) & 0xfc) | ((un_base64_tab[in[cur_index - 2]] >> 4) & 0x03));
                    ss << (unsigned char)(((un_base64_tab[in[cur_index - 2]] << 4) & 0xf0) | ((un_base64_tab[in[cur_index - 1]] >> 2) & 0x0f));
                    ss << (unsigned char)(((un_base64_tab[in[cur_index - 1]] << 6) & 0xc0));
                }

                break;
            }
            else if (k == 4){
                ss << (unsigned char)(((un_base64_tab[in[cur_index - 3]] << 2) & 0xfc) | ((un_base64_tab[in[cur_index - 2]] >> 4) & 0x03));
                ss << (unsigned char)(((un_base64_tab[in[cur_index - 2]] << 4) & 0xf0) | ((un_base64_tab[in[cur_index - 1]] >> 2) & 0x0f));
                ss << (unsigned char)(((un_base64_tab[in[cur_index - 1]] << 6) & 0xc0) | ((un_base64_tab[in[cur_index - 0]] >> 0) & 0x3f));
                k = 0;
            }

            cur_index++;
        }

        return std::move(ss.str());
    }

}
}

#endif