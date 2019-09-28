#pragma once
#include "SocketHandle.h"
#include "PResult.h"
#include "IPVersion.h"
#include "SocketOption.h"
#include "IPEndpoint.h"

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
		SocketHandle GetHandle();
		IPVersion GetIPVersion();

	private:
		IPVersion ipversion = IPVersion::IPv4;
		SocketHandle handle = INVALID_SOCKET;
	};
}