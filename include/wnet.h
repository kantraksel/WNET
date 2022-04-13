#ifndef _WNET_INCLUDE
#define _WNET_INCLUDE

#include <cstring>

#ifdef _WINDOWS
	#include <winerror.h>

	#ifndef _WINSOCK2API_
		#ifndef _WINSOCKAPI_
			#pragma message("WinSock2.h not imported, using own definition of SOCKET")
			typedef size_t SOCKET;
			#define SOCKET_ERROR (~0)
		#endif
	#endif
#else
	typedef int SOCKET;
	#define SOCKET_ERROR -1
#endif

namespace WNET
{
	struct CharTable
	{
		char* data;
		int size;

		~CharTable()
		{
			delete data;
		}
	};

	struct PeerInfo
	{
		char ipAddr[16];
		int ipAddrSize;
		unsigned short port;

		PeerInfo()
		{
			memset(this, 0, sizeof(PeerInfo));
		}
	};

	struct DNSResponse
	{
		CharTable pIpAddr;

		DNSResponse* pPreviousEntry;
		DNSResponse* pNextEntry;
	};

	struct PeerData
	{
		unsigned int address;
		unsigned short port;
	};
}

#include "Subsystem.h"
#include "ErrorCodes.h"
#include "ISocket.h"
#include "IHostSocket.h"
#include "IClientSocket.h"

#endif
