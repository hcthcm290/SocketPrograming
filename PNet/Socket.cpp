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
			return PResult::P_NotYetImplemented;
		}

		handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if(handle == INVALID_SOCKET)
		{
			int error = WSAGetLastError();
			return PResult::P_NotYetImplemented;
		}

		if (SetSocketOption(SocketOption::TCP_NoDelay, TRUE) != PResult::P_Success)
		{
			return PResult::P_NotYetImplemented;
		}

		return PResult::P_Success;
	}

	PResult Socket::Close()
	{
		if (handle == INVALID_SOCKET)
		{
			return PResult::P_NotYetImplemented;
		}

		int result = closesocket(handle);

		if (result != 0) // error occurr when try to close the socket
		{
			int error = WSAGetLastError();
			return PResult::P_NotYetImplemented;
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
			return PResult::P_NotYetImplemented;
		}

		if (result != 0)
		{
			int error = WSAGetLastError();
			return PResult::P_NotYetImplemented;
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
			return PResult::P_NotYetImplemented;
		}
		return PResult::P_Success;
	}

	PResult Socket::Listen(IPEndpoint endpoint, int backlog)
	{
		if (Bind(endpoint) == PResult::P_NotYetImplemented)
		{
			return PResult::P_NotYetImplemented;
		}

		int result = listen(handle, backlog);

		if (result != 0)
		{
			int error = WSAGetLastError();
			return PResult::P_NotYetImplemented;
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
			return PResult::P_NotYetImplemented;
		}
		IPEndpoint NewIPConnection((sockaddr*)(&addr));
		std::cout << "New connection accepted!" << std::endl;
		NewIPConnection.Print();

		outsocket = Socket(IPVersion::IPv4, acceptedConnectionHandle);
		return PResult::P_Success;
	}

	PResult Socket::Send(void * data, int numberOfBytes, int & bytesSent)
	{
		bytesSent = send(handle, (const char*)data, numberOfBytes, NULL);
		if (bytesSent == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			return PResult::P_NotYetImplemented;
		}
		return PResult::P_Success;
	}

	PResult Socket::Recv(void * destination, int numberOfBytes, int & bytesReceive)
	{
		bytesReceive = recv(handle, (char*)destination, numberOfBytes, NULL);

		if (bytesReceive == 0) //The connection has been gracefully closed
		{
			return PResult::P_NotYetImplemented;
		}

		if (bytesReceive == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			return PResult::P_NotYetImplemented;
		}

		return PResult::P_Success;
	}

	PResult Socket::Connect(IPEndpoint endpoint)
	{
		sockaddr_in addr = endpoint.GetSockaddrIPv4();
		int result = connect(handle, (sockaddr*)(&addr), sizeof(sockaddr_in));
		if (result != 0)
		{
			int error = WSAGetLastError();
			return PResult::P_NotYetImplemented;
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
