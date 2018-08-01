/** 
 * @brief a unique id generator
 *
 * @from :https://github.com/twitter/snowflake
 * 
 * @author  :   yandaren1220@126.com
 * @date    :   2018-08-01
 */

#include <stdint.h>
#include <chrono>
#include <mutex>
#include <stdexcept>

namespace utility
{
    class idworker
    {
    protected:
        static const int64_t twepoch = 1288834974657L;
        static const int64_t workerid_bits = 5L;
        static const int64_t datacenterid_bits = 5L;
        static const int64_t max_worker_id = -1L ^ (-1L << (int32_t)workerid_bits);
        static const int64_t max_data_center_id = -1L ^ (-1L << (int32_t)datacenterid_bits);
        static const int64_t sequence_bits = 12L;

        static const int64_t workerid_shift = sequence_bits;
        static const int64_t datacenter_id_shift = sequence_bits + workerid_bits;
        static const int64_t timestamp_left_shift = sequence_bits + workerid_bits + datacenterid_bits;
        static const int64_t sequence_mask = -1L ^ (-1L << (int32_t)sequence_bits);

        std::mutex  mtx_;
        int64_t     worker_id_;
        int64_t     datacenter_id_;
        int64_t     sequence_ = 0;
        int64_t     last_timestamp = -1;

    public:

        /** 
         * @brief worker_id         - 5bits[0~31]
         * @brief data_center_id    - 5bits[0~31]
         */
        idworker(int64_t worker_id, int64_t data_center_id) {
            if (worker_id > max_worker_id || worker_id < 0) {
                char expection_info[128];
                sprintf(expection_info, "workerid[%lld] can't be greater than %lld or less 0", worker_id, max_worker_id);
                throw std::runtime_error(expection_info);
            }

            if (data_center_id > max_data_center_id || data_center_id == 0) {
                char expection_info[128];
                sprintf(expection_info, "datacenterid[%lld] can't be greater than %lld or less 0", data_center_id, max_data_center_id);
                throw std::runtime_error(expection_info);
            }

            worker_id_ = worker_id;
            datacenter_id_ = data_center_id;
        }

        ~idworker(){}

    public:
        int64_t next_id() {
            std::lock_guard<std::mutex> locker(mtx_);

            int64_t timestamp = time_gen();

            if (timestamp < last_timestamp) {
                char expection_info[128];
                sprintf(expection_info, "clock moved backwards. refusing to generate id for %lld milliseconds", last_timestamp - timestamp);

                throw std::runtime_error(expection_info);
            }

            if (last_timestamp == timestamp) {
                sequence_ = (sequence_ + 1) & sequence_mask;
                if (sequence_ == 0) {
                    timestamp = til_next_millis(last_timestamp);
                }
            }
            else {
                sequence_ = 0;
            }

            last_timestamp = timestamp;

            return ((timestamp - twepoch) << (int32_t)timestamp_left_shift) | (datacenter_id_ << (int32_t)datacenter_id_shift) | (worker_id_ << (int32_t)workerid_shift) | sequence_;
        }

    protected:
        int64_t time_gen() {
            return std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
        }

        int64_t til_next_millis(int64_t last_time_stamp) {
            int64_t timestamp = time_gen();
            while (timestamp <= last_time_stamp)
            {
                timestamp = time_gen();
            }
            return timestamp;
        }
    };
}   // utility