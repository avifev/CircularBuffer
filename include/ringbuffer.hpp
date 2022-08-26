#include <atomic>
#include <mutex>
#include <stddef.h>
#include <iostream>
#include <thread>

namespace logger {
    template <typename type, size_t buffersize>
    class ring_buffer {
        private:
            type* m_inner_data; // pointer to inner buffer
            std::atomic<size_t> m_size; // will give actual number of entries
            size_t m_max_size; // will store the max size of this buffer
            std::atomic<size_t> head, tail; // insertion will happen at head and pop at tail

        public:
            //default constructor
            ring_buffer() : m_max_size(buffersize) {
                m_inner_data = new type[m_max_size];
                head = tail = 0;
                m_size = 0;
            }
            //destructor
            ~ring_buffer() {delete[] m_inner_data;}

            //push
            void push(type _dataitem) {
                if(!this->full()) {
                    
                    size_t _tTail = tail;
                    while(!(tail.compare_exchange_strong(_tTail, (tail+1) % m_max_size)));
                    m_inner_data[tail] = _dataitem;
                    ++m_size;
                } // else std::cout << "full\n";
            }
            
            //pop
            bool pop(type& _outdata) {
                if(!this->empty()) {
                    size_t _tHead = head;
                    while(!this->head.compare_exchange_strong(_tHead, (head+1) % m_max_size));
                    _outdata = m_inner_data[head];
                    --m_size;
                    return true;
                } // else std::cout << "empty\n";

                return false;
            }

            size_t size() const { return this->m_size; }
                        
            bool full() const { return m_size==m_max_size; }
            bool empty() const { return m_size==0; }
    };
} // namespace logger
