#ifndef _WNET_INCLUDE
#define _WNET_INCLUDE

#include <cstring>

#ifdef _WIN32
	#include <winerror.h>

	#ifndef _WINSOCK2API_
		#ifndef _WINSOCKAPI_
			typedef size_t SOCKET;
		#endif
	#endif
#else
	typedef int SOCKET;
#endif

namespace WNET
{
	template <typename T>
	struct Table
	{
		T* data;
		int size;

		Table() : data(0), size(0)
		{}

		inline void Release()
		{
			delete[] data;
		}
	};

	typedef Table<Table<char> > DnsResponse;

	struct PeerData
	{
		unsigned int address;
		unsigned short port;
	};

	struct PeerInfo
	{
		char addr[16];
		unsigned short port;

		PeerInfo()
		{
			memset(this, 0, sizeof(PeerInfo));
		}
	};
}

#include "Subsystem.h"
#include "ISocket.h"
#include "IUDPSocket.h"
#include "PollFD.h"

#endif
