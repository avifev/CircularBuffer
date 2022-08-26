#include <linux/can.h>
#include <string>

namespace logger {
    struct packet_s {
        std::string _timestamp;
        struct can_frame _canframe;
    };
        
} // namespace logger
