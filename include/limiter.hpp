#include <stddef.h>
#include <atomic>
#include <stdexcept>

namespace logger {
    template <typename type, size_t max_instances>
    class limiter {
        private:
            static std::atomic<int> m_instance_count;
        public:
            limiter() {
                if(m_instance_count >= max_instances) {
                    throw std::logic_error("max instances reached");
                } else m_instance_count++;
            }
            ~limiter() {m_instance_count--;}
    };

    template <typename type, size_t max_instances>
    std::atomic<int> logger::limiter<type,max_instances>::m_instance_count{0};
} // namespace logger
