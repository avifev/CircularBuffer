#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
//#include <string.h>
#include <string>
#include <stdexcept>
#include <packet.hpp>

#include <sstream>
#include <iomanip>


namespace logger {
	class client{
		private:
			int sock;

			//const std::string ack = std::string("ACKNOWLEDGED");
			char buf[4096];
			int connect_to_server(int sock, sockaddr_in hint);
			const std::string parse_msg(struct logger::packet_s message);
		public:
			bool init_client(int port = 8080, std::string ipAddress = "127.0.0.1");
			bool send_to_server(struct logger::packet_s);
			void close_socket();
	};
} // namespace logger
