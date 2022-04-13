#ifdef _WINDOWS
	#include <Ws2tcpip.h>
	#include <iphlpapi.h>
	#pragma comment(lib, "IPHLPAPI.lib")
#else
	#include <unistd.h>
	#include <netdb.h>
	#include <arpa/inet.h>
#endif

#include "wnet.h"

using namespace WNET;

bool Subsystem::Initialize()
{
#ifdef _WINDOWS
	WSAData wd;
	return WSAStartup(MAKEWORD(2, 2), &wd) == NO_ERROR;
#else
	return true;
#endif
}

int Subsystem::Release()
{
#ifdef _WINDOWS
	return WSACleanup();
#else
	return 0;
#endif
}

DNSResponse* Subsystem::ResolveLocalIPs()
{
	char name[256];
	if (gethostname(name, sizeof(name)) == 0)
		return ResolveHostName(name);
	
	return nullptr;
}

DNSResponse* Subsystem::ResolveHostName(const char* host)
{
	addrinfo info;
	addrinfo* retInfo = nullptr;
	DNSResponse* retBuff = nullptr;

	memset(&info, 0, sizeof(addrinfo));
	info.ai_flags = AI_CANONNAME;
	info.ai_family = AF_INET;

	retBuff = (DNSResponse*)getaddrinfo(host, nullptr, &info, &retInfo);
	if (retBuff) retBuff = nullptr;
	else
	{
		retBuff = nullptr;
		DNSResponse* currentBuff = nullptr;

		for (addrinfo* ptr = retInfo; ptr != nullptr; ptr = ptr->ai_next)
		{
			char buffer[16];
			if (inet_ntop(ptr->ai_family, &((sockaddr_in*)(ptr->ai_addr))->sin_addr, buffer, sizeof(buffer)))
			{
				if (retBuff)
				{
					currentBuff->pNextEntry = new DNSResponse;
					currentBuff->pNextEntry->pPreviousEntry = currentBuff;
					currentBuff = currentBuff->pNextEntry;
				}
				else
				{
					retBuff = new DNSResponse;
					currentBuff = retBuff;
					currentBuff->pPreviousEntry = nullptr;
				}

				currentBuff->pNextEntry = nullptr;
				currentBuff->pIpAddr.size = sizeof(buffer);
				currentBuff->pIpAddr.data = new char[currentBuff->pIpAddr.size];
				memcpy(currentBuff->pIpAddr.data, buffer, sizeof(buffer));
			}
		}
		freeaddrinfo(retInfo);
	}

	return retBuff;
}

void Subsystem::GetPeerInfo(PeerData& data, PeerInfo& info)
{
	char buff[16];
	memset(buff, 0, sizeof(buff));
	inet_ntop(AF_INET, &data.address, buff, sizeof(buff));

	memcpy(info.ipAddr, buff, sizeof(buff));
	info.ipAddrSize = strlen(info.ipAddr);
	info.port = ntohs(data.port);
}
