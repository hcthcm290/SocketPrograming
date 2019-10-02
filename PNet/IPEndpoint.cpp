#include "IPEndpoint.h"
#include <sstream>
#include <iostream>
#include <iterator>
#include <assert.h>

namespace PNet
{
	IPEndpoint::IPEndpoint(const char * ip, unsigned short port)
	{
		this->port = port;

		in_addr addr;

		int result = inet_pton(AF_INET, ip, &addr);

		if (result == 1)
		{
			if (addr.S_un.S_addr != INADDR_NONE)
			{
				ip_string = ip;
				hostname = ip;

				ip_bytes.resize(sizeof(ULONG));
				memcpy(&ip_bytes[0], &addr.S_un.S_addr, sizeof(ULONG));

				ipversion = IPVersion::IPv4;
				return;
			}
		}

		addrinfo hint = {};
		hint.ai_family = AF_INET;
		addrinfo* hostinfo = nullptr;
		result = getaddrinfo(ip, NULL, &hint, &hostinfo);
		if (result == 0)
		{
			sockaddr_in * host_addr = reinterpret_cast<sockaddr_in*>(hostinfo->ai_addr);
			ip_string.resize(16);

			hostname = ip;

			ip_bytes.resize(sizeof(ULONG));
			memcpy(&ip_bytes[0], &host_addr->sin_addr.S_un.S_addr, sizeof(ULONG));

			ipversion = IPVersion::IPv4;

			std::stringstream ip_addr;
			std::copy(ip_bytes.begin(), ip_bytes.end(), std::ostream_iterator<ULONG>(ip_addr, "."));
			ip_string = ip_addr.str();
			ip_string = ip_string.substr(0, ip_string.length() - 1);

			freeaddrinfo(hostinfo);
		}
	}

	IPEndpoint::IPEndpoint(sockaddr* addr)
	{
		assert(addr->sa_family == AF_INET);
		sockaddr_in* addrv4 = reinterpret_cast<sockaddr_in*>(addr);
		ipversion = IPVersion::IPv4;
		port = ntohs(addrv4->sin_port);
		ip_bytes.resize(sizeof(ULONG));
		std::memcpy(&ip_bytes[0], &addrv4->sin_addr.S_un.S_addr, sizeof(ULONG));
		ip_string.resize(16);
		inet_ntop(AF_INET, &addrv4->sin_addr.S_un.S_addr, &ip_string[0], 16);
		hostname = ip_string;
	}

	IPVersion IPEndpoint::GetIPVersion()
	{
		return ipversion;
	}

	std::string IPEndpoint::GetHostname()
	{
		return hostname;
	}

	std::string IPEndpoint::GetIPString()
	{
		return ip_string;
	}

	unsigned short IPEndpoint::GetPort()
	{
		return port;
	}

	std::vector<uint8_t> IPEndpoint::GetIPBytes()
	{
		return ip_bytes;
	}

	sockaddr_in IPEndpoint::GetSockaddrIPv4()
	{
		assert(ipversion == IPVersion::IPv4);
		sockaddr_in addr = {};
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		memcpy(&addr.sin_addr, &ip_bytes[0], sizeof(ULONG));
		return addr;
	}
	void IPEndpoint::Print()
	{
		switch (ipversion)
		{
		case IPv4: 
			std::cout << "IP version 4" << std::endl;
			break;
		case IPv6:
			std::cout << "IP version 6" << std::endl;
			break;
		default:
			std::cout << "IP version UNknown" << std::endl;
			break;
		}
		std::cout << "Hostname: "			<< hostname << std::endl;
		std::cout << "Port: "		<< port		 << std::endl;
		std::cout << "IP bytes "				 << std::endl;
		for (auto &i : ip_bytes)
		{
			std::cout << (int)i << std::endl;
		}
	}
}


