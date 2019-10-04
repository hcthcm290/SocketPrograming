#include "Socket.h"
#include <assert.h>
#include <iostream>

namespace PNet
{
	Socket::Socket(IPVersion ipversion, SocketHandle handle)
		:ipversion(ipversion),
		handle(handle)
	{
		assert(ipversion == IPVersion::IPv4);
	}

	PResult Socket::Create()
	{
		assert(ipversion == IPVersion::IPv4);

		if (handle != INVALID_SOCKET)
		{
			return PResult::P_GenericError;
		}

		handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if(handle == INVALID_SOCKET)
		{
			int error = WSAGetLastError();
			return PResult::P_GenericError;
		}

		if (SetSocketOption(SocketOption::TCP_NoDelay, TRUE) != PResult::P_Success)
		{
			return PResult::P_GenericError;
		}

		return PResult::P_Success;
	}

	PResult Socket::Close()
	{
		if (handle == INVALID_SOCKET)
		{
			return PResult::P_GenericError;
		}

		int result = closesocket(handle);

		if (result != 0) // error occurr when try to close the socket
		{
			int error = WSAGetLastError();
			return PResult::P_GenericError;
		}

		handle = INVALID_SOCKET;

		return PResult::P_Success;
	}

	PResult Socket::SetSocketOption(SocketOption option, BOOL value)
	{
		int result;
		switch (option)
		{
		case SocketOption::TCP_NoDelay:
			result = setsockopt(handle, IPPROTO_TCP,TCP_NODELAY , (const char*)&value, sizeof(value));
			break;
		default:
			return PResult::P_GenericError;
		}

		if (result != 0)
		{
			int error = WSAGetLastError();
			return PResult::P_GenericError;
		}

		return PResult::P_Success;
	}

	PResult Socket::Bind(IPEndpoint endpoint)
	{
		sockaddr_in addr = endpoint.GetSockaddrIPv4();
		int result = bind(handle, (sockaddr*)(&addr), sizeof(sockaddr_in));

		if (result != 0)
		{
			int error = WSAGetLastError();
			return PResult::P_GenericError;
		}
		return PResult::P_Success;
	}

	PResult Socket::Listen(IPEndpoint endpoint, int backlog)
	{
		if (Bind(endpoint) == PResult::P_GenericError)
		{
			return PResult::P_GenericError;
		}

		int result = listen(handle, backlog);

		if (result != 0)
		{
			int error = WSAGetLastError();
			return PResult::P_GenericError;
		}

		return PResult::P_Success;
	}

	PResult Socket::Accept(Socket & outsocket)
	{
		sockaddr_in addr = {};
		int size = sizeof(addr);
		SocketHandle acceptedConnectionHandle = accept(handle,(sockaddr*)(&addr), &size);
		if (acceptedConnectionHandle == INVALID_SOCKET)
		{
			int error = WSAGetLastError();
			return PResult::P_GenericError;
		}
		IPEndpoint NewIPConnection((sockaddr*)(&addr));
		std::cout << "New connection accepted!" << std::endl;
		NewIPConnection.Print();

		outsocket = Socket(IPVersion::IPv4, acceptedConnectionHandle);
		return PResult::P_Success;
	}

	PResult Socket::Send(const void * data, int numberOfBytes, int & bytesSent)
	{
		bytesSent = send(handle, (const char*)data, numberOfBytes, NULL);
		if (bytesSent == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			return PResult::P_GenericError;
		}
		return PResult::P_Success;
	}

	PResult Socket::SendAll(const void * data, int numberOfBytes)
	{
		int totalBytesSent = 0;
		while (totalBytesSent < numberOfBytes)
		{
			int bytesRemaining = numberOfBytes - totalBytesSent;
			int bytesSent = 0;
			char* buffer = (char*)data + totalBytesSent;
			PResult result = Send(buffer, bytesRemaining, bytesSent);
			if (result != PResult::P_Success)
			{
				return PResult::P_GenericError;
			}
			totalBytesSent += bytesSent;
		}
		return PResult::P_Success;
	}

	PResult Socket::Recv(void * destination, int numberOfBytes, int & bytesReceive)
	{
		bytesReceive = recv(handle, (char*)destination, numberOfBytes, NULL);

		if (bytesReceive == 0) //The connection has been gracefully closed
		{
			return PResult::P_GenericError;
		}

		if (bytesReceive == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			return PResult::P_GenericError;
		}

		return PResult::P_Success;
	}

	PResult Socket::RecvAll(void * destination, int numberOfBytes)
	{
		int totalBytesRecv = 0;
		while (totalBytesRecv < numberOfBytes)
		{
			int bytesRemaining = numberOfBytes - totalBytesRecv;
			int bytesRecv = 0;
			char* buffer = (char*)destination + totalBytesRecv;
			PResult result = Recv(buffer, bytesRemaining, bytesRecv);
			if (result != PResult::P_Success)
			{
				return PResult::P_GenericError;
			}
			totalBytesRecv += bytesRecv;
		}
		return PResult::P_Success;
	}

	PResult Socket::Send(const Packet packet)
	{
		uint32_t encodedPacketSize = htonl(packet.buffer.size());
		PResult result = SendAll(&encodedPacketSize, sizeof(uint32_t));
		if (result != PResult::P_Success)
		{
			return PResult::P_GenericError;
		}

		result = SendAll(packet.buffer.data(), packet.buffer.size());
		if (result != PResult::P_Success)
		{
			return PResult::P_GenericError;
		}

		return PResult::P_Success;
	}

	PResult Socket::Recv(Packet & packet)
	{
		packet.Clear();
		uint32_t encodedPacketSize = 0;
		PResult result = RecvAll(&encodedPacketSize, sizeof(uint32_t));
		if (result != PResult::P_Success)
		{
			return PResult::P_GenericError;
		}
		encodedPacketSize = ntohl(encodedPacketSize);
		packet.buffer.resize(encodedPacketSize);
		result = RecvAll(packet.buffer.data(), encodedPacketSize);
		if (result != PResult::P_Success)
		{
			return PResult::P_GenericError;
		}
		return PResult();
	}

	PResult Socket::Connect(IPEndpoint endpoint)
	{
		sockaddr_in addr = endpoint.GetSockaddrIPv4();
		int result = connect(handle, (sockaddr*)(&addr), sizeof(sockaddr_in));
		if (result != 0)
		{
			int error = WSAGetLastError();
			return PResult::P_GenericError;
		}


		return PResult::P_Success;
	}

	SocketHandle Socket::GetHandle()
	{
		return handle;
	}

	IPVersion Socket::GetIPVersion()
	{
		return ipversion;
	}
}
