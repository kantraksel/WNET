#pragma once
#include "Socket.h"
#ifdef _WINDOWS
	#define pollfd WSAPOLLFD
#else
	#include <poll.h>
#endif

namespace WNET
{
	class HostSocket : public IHostSocket
	{
		private:
			pollfd pollFD;

		public:
			HostSocket();
			~HostSocket();

			bool BindSocket(const char* host, unsigned short port);
			void UnBindSocket();

			bool Poll(int timeout);
			//TODO: obsolete! GetPeerData returns reference, so user can modify it directly
			void SetPeerData(PeerData& peerData);
	};
}
