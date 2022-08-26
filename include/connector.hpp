#include <vector>
#include <thread>
#include <functional>
#include <sstream>
#include <linux/can.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <limiter.hpp>
#include <client.hpp>
#include <lib.h>
//#include <packet.hpp>
#include <atomic>
#include <time.h>
#include <stdio.h>

namespace logger {
    /**
     * @brief This class is supposed to dynamically adapt to the
     * number of data input sockets and accordingly spawn threads
     * accept incomming data and push into the ring_buffer
    */
    template <typename type>
    class dynamic_connector : logger::limiter<dynamic_connector<type>,1> {
        private:
            std::vector<std::thread> m_thread_store;
            std::thread m_sender;
            std::thread m_sender_two;
            type* m_target_buffer;
            logger::client client;
        public:
            ~dynamic_connector() {
                for(auto& t:m_thread_store) {
                    t.join();
                }
            }
            void init(int argc, char const *argv[]) {
                for(int i{1};i<argc;i++) {
                    // int out; std::stringstream ss; ss << std::string(argv[i]); ss >> out;
                    this->m_thread_store.push_back(std::thread(&dynamic_connector::inner_monitor_proc,this,argv[i],"127.0.0.1"));
                }
            }
            void set_target_buffer(type& _buffer) {
                /**
                 * through this function, our connector object will know where to push the incomming data
                 * */
                this->m_target_buffer = &_buffer;
            }
            void init_transmit_handler() {
                
                client.init_client();
                this->m_sender = std::thread(&dynamic_connector::inner_sender_proc,this);
                this->m_sender_two = std::thread(&dynamic_connector::inner_sender_proc,this);
            }

        private:
            void inner_sender_proc(
                
            ) {
                

                /**
                 * all code to pop and send data will be here
                */
                std::cout << "inside sender thread\n";
                while(true) {
                   
                        logger::packet_s packet;
                        bool _status = m_target_buffer->pop(packet);
                        if(_status)
                            client.send_to_server(packet);
                }

            }

            void inner_monitor_proc(
                std::string _port,
                std::string _ipaddress
            ) {
                /**
                 * this thread func will take in PORT number and IP address as args
                 * so as to initialize the socket to receive incomming data
                 * this function will have code to setup the socket and start listening
                */
                std::cout << "thread " << std::this_thread::get_id() << " started\n";
                // char* argv[]={""};
                // can_receive(2,argv);

                int s;
                struct sockaddr_can addr;
                struct ifreq ifr;

                s = socket(PF_CAN, SOCK_RAW, CAN_RAW);

                //strcpy(ifr.ifr_name, _port.c_str() );
                ioctl(s, SIOCGIFINDEX, &ifr);

                addr.can_family = AF_CAN;
                addr.can_ifindex = ifr.ifr_ifindex;
                
                if(bind(s, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
                    std::cout << "bind failed...\n";
                } else std::cout << "bind done...\n";
                
                struct can_frame frame;
                ssize_t nbytes;
                
                std::cout << "status: ready to read can frames\n";
               
                /**
                 * if anyone does cangen on a can device named can0
                 * then we will be able to read it here
                */
                while(true) {

                    nbytes = read(s, &frame, sizeof(struct can_frame));
                    if (nbytes < 0) {
                            std::cerr << "can raw socket read less than 0 bytes\n";
                    } else {
                        // std::cout << nbytes << std::endl;
                        if (nbytes < sizeof(struct can_frame)) {
                            std::cerr << "status: incomplete CAN frame read\n";
                        } else {
                            /**
                             * here the push operation will be handled
                            */
                            logger::packet_s packet;
                            time_t time_;
                            struct tm * t_info;
                            time(&time_);
                            t_info = localtime(&time_);
                            packet._timestamp = asctime(t_info);
                            // packet._timestamp = std::to_string(
                            //     std::chrono::system_clock::now().time_since_epoch().count()
                            // );
                            packet._canframe = frame;
                            // std::cout << packet._timestamp << std::endl;
                            // std::cout << packet._canframe.can_id << std::endl;
                            // std::cout << packet._canframe.can_dlc << std::endl;
                            // std::cout << packet._canframe.__pad << std::endl;
                            // std::cout << packet._canframe.__res0 << std::endl;
                            // std::cout << packet._canframe.__res1 << std::endl;
                            //std::cout << packet._canframe.data << std::endl;
                            // std::cout << frame.can_id
                            //     << ":"
                            // std::cout << "Data: "
                            //     << (uint8_t)packet._canframe.data[0] << ","
                            //     << (uint8_t)packet._canframe.data[1] << ","
                            //     << (uint8_t)packet._canframe.data[2] << ","
                            //     << (uint8_t)packet._canframe.data[3] << ","
                            //     << (uint8_t)packet._canframe.data[4] << ","
                            //     << (uint8_t)packet._canframe.data[5] << ","
                            //     << (uint8_t)packet._canframe.data[6] << ","
                            //     << (uint8_t)packet._canframe.data[7] << "\n";
                            
                            for(int i = 0; i < packet._canframe.can_dlc; i++)
                            {
                                printf("%x ", packet._canframe.data[i]);
                               // sprintf()
                            }

                            std::cout << std::endl;

                            // std::thread tmp_thread([&](){
                            //     this->m_target_buffer->push(packet);
                            // });
                            // tmp_thread.detach();
                            m_target_buffer->push(packet);
                            
                            //bzero(&frame,sizeof(struct can_frame));
                            // std::cout << "status: pushed\n";
                        }
                    }
                }

            }
    };
} // namespace logger
