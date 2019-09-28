//Server code

#include <PNet/IncludeMe.h>
#include <iostream>

using namespace PNet;

int main()
{
	
	if (Network::Initialize())
	{
		std::cout << "Initialize Successfull" << std::endl;

		IPEndpoint test("192.168.0.2", 8080);
		if (test.GetIPVersion() == IPVersion::IPv4)
		{
			std::cout << "Host name: " << test.GetHostname() << std::endl;
			std::cout << "IP: " << test.GetIPString() << std::endl;
			std::cout << "Port: " << test.GetPort() << std::endl;
			std::cout << "IP Bytes: " << std::endl;
			for(auto digit : test.GetIPBytes())
			{
				std::cout << (int)digit << std::endl;
			}
		}
		else
		{
			std::cerr << "this is not an ipv4 adress." << std::endl;
		}

		Socket socket;
		if (socket.Create() == PResult::P_Success)
		{
			std::cout << "Create socket successful" << std::endl;
			socket.Close();
		}
		else
		{
			std::cerr << "Fail to creat socket" << std::endl;

		}
	}

	Network::Shutdown();
	system("pause");
	return 0;
}