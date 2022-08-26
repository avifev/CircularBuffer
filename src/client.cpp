#include "client.hpp"

namespace logger {

	bool client::init_client(int port, std::string ipAddress)
	{
		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(port);
		inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);
		try
		{
			sock = socket(AF_INET, SOCK_STREAM, 0);
			
			if (sock == -1)
			{
				throw std::runtime_error("Socket Not Created Successfully");
				return false;
			}
	
			//	Connect to the server on the socket
			int connectRes = -1;
			while(connectRes == -1)
			{
				connectRes = connect(sock, (sockaddr*)&hint, sizeof(hint));

			}

		} 
		catch (const std::runtime_error& e) {
			std::cerr << e.what() << std::endl;
			return false;
		}
		return true;
	}


	int client::connect_to_server(int sock, sockaddr_in hint)
	{
		int connectRes = connect(sock, (sockaddr*)&hint, sizeof(hint));
		if (connectRes == -1)
		{
			connect_to_server(sock, hint);
		}
		return connectRes;
	}


	const std::string client::parse_msg(struct logger::packet_s message)
	{
		// std::cout << message._timestamp << std::endl;
		// std::cout << message._canframe.can_id << std::endl;
		// std::cout << message._canframe.can_dlc << std::endl;
		// std::cout << message._canframe.__pad << std::endl;
		// std::cout << message._canframe.__res0 << std::endl;
		// std::cout << message._canframe.__res1 << std::endl;
		// std::cout << message._canframe.data << std::endl;
		
		std::string time_stamp = message._timestamp;

		std::string msg = std::to_string(message._canframe.can_id);
		msg.append(" ");
		msg.append(std::to_string(message._canframe.can_dlc));
		msg.append(" ");
		msg.append(std::to_string(message._canframe.__pad));
		msg.append(" ");
		msg.append(std::to_string(message._canframe.__res0));
		msg.append(" ");
		msg.append(std::to_string(message._canframe.__res1));
		msg.append(" ");
		//std::string _data(0, message._canframe.can_dlc); //= reinterpret_cast<char *>(message._canframe.data);
		
		std::stringstream s;	
		//char temp[1] = {0};

		//char _data[message._canframe.can_dlc + 1] = {'\0'};
		for(int i = 0; i < message._canframe.can_dlc; ++i)
		{
			//sprintf(temp, "%02x ", (unsigned char)message._canframe.data[i]);
			//_data[i] = temp[0];
			s << std::hex <<std::setfill('0')<<std::setw(2)<<(unsigned short)message._canframe.data[i] <<" ";
		}

		//_data[i] = '\0';

		//std::cout << "Data: " << _data << std::endl;
		std::cout << s.str() << std::endl;
		
		msg.append(s.str());
		msg.append(" ");
		msg.append(time_stamp);

		std::cout << time_stamp << std::endl;
		return msg;
	}


	bool client::send_to_server(struct logger::packet_s message)
	{
		const std::string& msg = parse_msg(message);
		// std::cout << msg << "\n";

		try 
			{
				//	Send to server
				int sendRes = send(sock, msg.c_str(), msg.size() + 1, 0);
				if (sendRes == -1)
				{
					throw std::runtime_error("Could not send to server! Whoops!");
					return false;
				}
	
		
			//	Wait for the acknowledgement from server
				
				// int bytesReceived = recv(sock, buf, 4096, 0);
				// if (bytesReceived == -1 || (std::string(buf).compare(std::string("ACKNOWLEDGED"))!=0))
				// {
				// 	send_to_server(message);
				// }
		
			
			}
			catch(const std::runtime_error& e)
			{
				std::cerr << e.what() << std::endl;
				return false;
			}
			return true;
	}


	void client::close_socket()
	{
		close(sock);
	}

	
} // namespace logger
