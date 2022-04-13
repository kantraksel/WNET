#pragma once

namespace WNET
{
	class IHostSocket : public ISocket
	{
		public:
			static IHostSocket* Create();
			virtual ~IHostSocket();

			virtual bool BindSocket(const char* host, unsigned short port) = 0;
			virtual void UnBindSocket() = 0;

			virtual bool Poll(int timeout) = 0;
			virtual void SetPeerData(PeerData& peerData) = 0;
	};
}
