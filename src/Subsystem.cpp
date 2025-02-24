#include "Platform.h"
#include "wnet.h"

using namespace WNET;

bool Subsystem::Initialize()
{
#ifdef _WIN32
	WSAData wd;
	return WSAStartup(MAKEWORD(2, 2), &wd) == NO_ERROR;
#else
	return true;
#endif
}

bool Subsystem::Release()
{
#ifdef _WIN32
	return WSACleanup() == 0;
#else
	return true;
#endif
}

int Subsystem::GetLastError()
{
#ifdef _WIN32
	return WSAGetLastError();
#else
	return errno;
#endif
}

DnsResponse Subsystem::ResolveLocalIPs()
{
	char name[256];
	if (gethostname(name, sizeof(name)) == 0)
		return ResolveHostName(name);
	
	return {};
}

DnsResponse Subsystem::ResolveHostName(const char* host)
{
	DnsResponse response;

	addrinfo* info;
	addrinfo hint{};
	hint.ai_flags = AI_CANONNAME;
	hint.ai_family = AF_INET;
	if (getaddrinfo(host, nullptr, &hint, &info) == 0)
	{
		Endpoint ep{};
		for (addrinfo* ptr = info; ptr != nullptr; ptr = ptr->ai_next)
		{
			ep.address = ((sockaddr_in*)(ptr->ai_addr))->sin_addr.s_addr;
			response.push_back(ep.ToAddress());
		}
		freeaddrinfo(info);
	}
	return response;
}

Address Endpoint::ToAddress() const
{
	Address addr;

	inet_ntop(AF_INET, &address, addr.address, sizeof(addr.address));
	addr.port = ntohs(port);
	return addr;
}

bool Address::ToEndpoint(Endpoint& ep) const
{
	AddressPtr ptr{ address, port };
	return ptr.ToEndpoint(ep);
}

bool AddressPtr::ToEndpoint(Endpoint& ep) const
{
	if (inet_pton(AF_INET, address, &ep.address) == 1)
	{
		ep.port = htons(port);
		return true;
	}

	ep.address = 0;
	ep.port = 0;
	return false;
}
