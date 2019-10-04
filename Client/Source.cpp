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
			if (socket.Connect(IPEndpoint("42.118.65.139", 4790)) == PResult::P_Success)
			{
				std::cout << "Connect to server successful" << std::endl;
				
				Packet packet;
				packet << "This is the first packet!";
				packet << "This is a second packet!";
				while (true)
				{
					PResult result = socket.Send(packet);
					if (result != PResult::P_Success)
					{
						break;
					}
					std::cout << "Attemp to send chunk of data!" << std::endl;
					Sleep(500);
				}
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