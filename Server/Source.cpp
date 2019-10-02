//Server code

#include <PNet/IncludeMe.h>
#include <iostream>

using namespace PNet;

int main(){
	
	if (Network::Initialize())
	{
		std::cout << "Initialize Successfull" << std::endl;

		Socket socket;
		if (socket.Create() == PResult::P_Success)
		{
			std::cout << "Create socket successful" << std::endl;
			if (socket.Listen(IPEndpoint("0.0.0.0", 4790)) == PResult::P_Success)
			{
				std::cout << "Socket succesfully listening on port 4790." << std::endl;
				Socket NewConnection;
				if (socket.Accept(NewConnection) == PResult::P_Success)
				{
					std::cout << "Successful letting other connected" << std::endl;
					
					char buffer[256];
					int bytesReceive = 0;
					int result = NewConnection.Recv(buffer, 256, bytesReceive);
					if (result != PResult::P_Success)
					{
						std::cout << "Fail to receive message from client." << std::endl;
					}
					std::cout << "Message Received: " << buffer << std::endl;

					NewConnection.Close();
				}
				else
				{
					std::cout << "Fail letting other connected" << std::endl;
				}
			} 
			else
			{
				std::cout << "Fail to listen port 4790." << std::endl;
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