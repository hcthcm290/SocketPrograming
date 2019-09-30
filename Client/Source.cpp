//Client code

#include <PNet/IncludeMe.h>
#include <iostream>

using namespace PNet;

int main()
{
	if (Network::Initialize())
	{
		std::cout << "Initialize Successfull" << std::endl;
		Socket socket;
		if (socket.Create() == PResult::P_Success)
		{
			std::cout << "Create socket successful" << std::endl;
			if (socket.Connect(IPEndpoint("192.168.0.104", 4790)) == PResult::P_Success)
			{
				std::cout << "Connect to server successful" << std::endl;
			}
			else
			{
				std::cout << "Fail to connect to server" << std::endl;
			}
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