/**
 *
 * a ring_buffer implementation
 * 
 * can used for lockless programming in the situation that only one thread read and only one thread write
 *
 * @author  :   yandaren1220@126.com
 * @date    :   2018-04-10
 */

#include <stdint.h>

namespace utility{

template<class T, uint32_t N>
class ring_buffer{
protected:
    T     		arr_[N + 1];
    uint32_t   	head_;
    uint32_t   	tail_;
    const uint32_t buffer_size;

public:
    ring_buffer() :head_(0), tail_(0), buffer_size(N + 1){
    }

    bool empty(){
        return head_ == tail_;
    }

    bool full(){
        return (tail_ + 1) % buffer_size == head_;
    }
    
    void clear(){
		head_ = 0;
		tail_ = 0;
	}
	
	uint32_t size(){
		return tail_ >= head_ ? (tail_ - head_) : (buffer_size + tail_ - head_);
	}
	
	uint32_t max_size(){
		return N;
	}

    T* pop(){
        if (empty()){
            return nullptr;
        }

        T* r = &arr_[head_];
        head_ = (head_ + 1) % buffer_size;
        return r;
    }

    bool push(T& v){
        if (full()){
            return false;
        }

        arr_[tail_] = v;
        tail_ = (tail_ + 1) % buffer_size;
        return true;
    }

    int head(){
        return head_;
    }

    int tail(){
        return tail_;
    }
};

} // end namespace utility