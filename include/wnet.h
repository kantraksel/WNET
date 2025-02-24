#pragma once
#ifndef _WNET_INCLUDE
#define _WNET_INCLUDE

#include <cstring>
#include <vector>

#ifdef _WIN32
	#ifndef _WINSOCK2API_
		#ifndef _WINSOCKAPI_
			typedef size_t SOCKET;
		#endif
	#endif
	#pragma comment(lib, "Ws2_32.lib")
#else
	typedef int SOCKET;
#endif

namespace WNET
{
	struct Address;
	typedef std::vector<Address> DnsResponse;
	
	struct Endpoint
	{
		unsigned int address;
		unsigned short port;

		Address ToAddress() const;
	};

	struct Address
	{
		char address[16];
		unsigned short port;

		Address() : address{}, port(0)
		{
		}

		bool ToEndpoint(Endpoint& ep) const;
	};

	struct AddressPtr
	{
		const char* address;
		unsigned short port;

		bool ToEndpoint(Endpoint& ep) const;
	};
}

#include "wnet/Subsystem.h"
#include "wnet/Socket.h"
#include "wnet/UdpSocket.h"
#include "wnet/TcpSocket.h"
#include "wnet/PollFD.h"

#endif
