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
			if (socket.Connect(IPEndpoint("1.54.12.14", 4790)) == PResult::P_Success)
			{
				std::cout << "Connect to server successful" << std::endl;
				char buffer[256];
				std::cout << "Make a message you want to send: ";
				std::cin >> buffer;
				int bytesSent = 0;
				int result = socket.Send(buffer, 256, bytesSent);
				if (result == PResult::P_Success)
				{
					std::cout << "Message sent." << std::endl;
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