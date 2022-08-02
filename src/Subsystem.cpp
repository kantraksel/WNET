#ifdef _WIN32
	#include <Ws2tcpip.h>
	#include <iphlpapi.h>
	#pragma comment(lib, "IPHLPAPI.lib")
#else
	#include <unistd.h>
	#include <netdb.h>
	#include <arpa/inet.h>
	#include <cerrno>
#endif
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

int Subsystem::Release()
{
#ifdef _WIN32
	return WSACleanup();
#else
	return 0;
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
	
	return DnsResponse();
}

DnsResponse Subsystem::ResolveHostName(const char* host)
{
	addrinfo hint;
	memset(&hint, 0, sizeof(addrinfo));
	hint.ai_flags = AI_CANONNAME;
	hint.ai_family = AF_INET;

	addrinfo* info;
	int nValue = getaddrinfo(host, nullptr, &hint, &info);
	if (nValue) return DnsResponse();
	else
	{
		int tableSize = 0;
		for (addrinfo* ptr = info; ptr != nullptr; ptr = ptr->ai_next)
			++tableSize;

		Table<Table<char> > retTable;
		if (tableSize > 0)
		{
			auto pTable = new Table<char>[tableSize];
			
			char* pBuff = nullptr;
			const int buffSize = 16;

			int i = 0;
			for (addrinfo* ptr = info; ptr != nullptr; ptr = ptr->ai_next)
			{
				if (!pBuff) pBuff = new char[buffSize];
				if (inet_ntop(ptr->ai_family, &((sockaddr_in*)(ptr->ai_addr))->sin_addr, pBuff, buffSize))
				{
					auto& entry = pTable[i];
					entry.data = pBuff;
					entry.size = buffSize;

					pBuff = nullptr;
					++i;
				}
			}

			if (i == 0)
			{
				delete[] pTable;
				delete[] pBuff;
			}
			else
			{
				retTable.data = pTable;
				retTable.size = i;
			}
		}
		freeaddrinfo(info);
		
		return retTable;
	}
}

void Subsystem::ReleaseDnsResponse(DnsResponse& data)
{
	for (int i = 0; i < data.size; ++i)
	{
		data.data[i].Release();
	}
	data.Release();
	data.data = nullptr;
	data.size = 0;
}

void Subsystem::GetPeerInfo(const PeerData& data, PeerInfo& info)
{
	char* buff = info.addr;
	const int buffSize = sizeof(info.addr);

	memset(buff, 0, buffSize);
	inet_ntop(AF_INET, &data.address, buff, buffSize);

	info.port = data.port;
}

bool Subsystem::GetPeerData(const PeerInfo& info, PeerData& data)
{
	if (inet_pton(AF_INET, info.addr, &data.address) == 1)
	{
		data.port = info.port;
		return true;
	}
	
	data.address = 0;
	data.port = 0;
	return false;
}
