#pragma once
#include "SocketHandle.h"
#include "PResult.h"
#include "IPVersion.h"
#include "SocketOption.h"
#include "IPEndpoint.h"
#include "Constant.h"
#include <string>
#include "Packet.h"

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
		PResult Send(const void* data, int numberOfBytes, int& bytesSent);
		PResult SendAll(const void* data, int numberOfBytes);
		PResult Recv(void* destination, int numberOfBytes, int& bytesReceive);
		PResult RecvAll(void* destination, int numberOfBytes);
		PResult Send(const Packet packet);
		PResult Recv(Packet& packet);
		PResult Connect(IPEndpoint endpoint);
		SocketHandle GetHandle();
		IPVersion GetIPVersion();

	private:
		IPVersion ipversion = IPVersion::IPv4;
		SocketHandle handle = INVALID_SOCKET;
	};
}