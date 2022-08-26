#include <ringbuffer.hpp>
#include <connector.hpp>
#include <iostream>
#include <thread>

int main(int argc, char const *argv[]) {
    
    //logger::client client;
    logger::ring_buffer<logger::packet_s,2000> buff;
    logger::dynamic_connector<decltype(buff)> connector;

    connector.init(argc,argv);
    connector.set_target_buffer(buff);
    connector.init_transmit_handler();

    // std::thread t1([&](){
    //     for(int i{0};i<1000;i++) {
    //         buff.push(i);
    //     }
    // });
    // std::thread t2([&](){
    //     for(int i{1000};i<2010;i++) {
    //         buff.push(i);
    //     }
    // });

    // t1.join();
    // t2.join();

    // std::cout << buff.size() << std::endl;

    // std::thread t3([&](){
    //     while (!buff.empty()) {
    //         buff.pop();
    //     }
    // });
    // std::thread t4([&](){
    //     while (!buff.empty()) {
    //         buff.pop();
    //     }
    // });

    // t3.join();
    // t4.join();

    // std::cout << buff.size() << std::endl;

    return 0;
}
