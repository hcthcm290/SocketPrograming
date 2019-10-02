#pragma once
#include "SocketHandle.h"
#include "PResult.h"
#include "IPVersion.h"
#include "SocketOption.h"
#include "IPEndpoint.h"
#include <string>

namespace PNet
{
	class Socket
	{
	public:
		Socket( IPVersion ipversion = IPVersion::IPv4,
				SocketHandle sockethandle = INVALID_SOCKET);
		PResult Create();
		PResult Close();
		PResult SetSocketOption(SocketOption option, BOOL value);
		PResult Bind(IPEndpoint endpoint);
		PResult Listen(IPEndpoint endpoint, int backlog = 5);
		PResult Accept(Socket &outsocket);
		PResult Send(void* data, int numberOfBytes, int& bytesSent);
		PResult Recv(void* destination, int numberOfBytes, int& bytesReceive);
		PResult Connect(IPEndpoint endpoint);
		SocketHandle GetHandle();
		IPVersion GetIPVersion();

	private:
		IPVersion ipversion = IPVersion::IPv4;
		SocketHandle handle = INVALID_SOCKET;
	};
}