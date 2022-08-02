#pragma once

namespace WNET
{
	struct SocketFactory
	{
		static addrinfo CreateAddrInfo(const char* host, unsigned short port);
		static void ReleaseAddrInfo(addrinfo& info);
		static SOCKET CreateSocket(const addrinfo& info);
	};
}
