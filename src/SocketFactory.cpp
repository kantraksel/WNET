#ifdef _WIN32
	#include <Ws2tcpip.h>
#else
	#include <arpa/inet.h>
	#include <netdb.h>
#endif
#include "wnet.h"
#include "SocketFactory.h"

using namespace WNET;

addrinfo SocketFactory::CreateAddrInfo(const char* host, unsigned short port)
{
	addrinfo info;
	memset(&info, 0, sizeof(info));
	info.ai_family = AF_INET;
	info.ai_socktype = SOCK_DGRAM;
	info.ai_protocol = IPPROTO_UDP;

	in_addr addr;
	if (inet_pton(info.ai_family, host, &addr) == 1)
	{
		sockaddr_in* sin = new sockaddr_in;
		memset(sin->sin_zero, 0, sizeof(sin->sin_zero));
		sin->sin_family = info.ai_family;
		sin->sin_addr = addr;
		sin->sin_port = htons(port);

		info.ai_addr = (sockaddr*)sin;
		info.ai_addrlen = sizeof(sockaddr);
	}

	return info;
}

void SocketFactory::ReleaseAddrInfo(addrinfo& info)
{
	delete info.ai_addr;
	info.ai_addr = nullptr;
	info.ai_addrlen = 0;
}

SOCKET SocketFactory::CreateSocket(const addrinfo& info)
{
	return socket(info.ai_family, info.ai_socktype, info.ai_protocol);
}
