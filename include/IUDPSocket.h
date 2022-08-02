#pragma once

namespace WNET
{
	class IUDPSocket : public ISocket
	{
		public:
			static IUDPSocket* Create();
			static void Close(IUDPSocket* pSocket);
			virtual ~IUDPSocket() override;

			virtual int SendTo(const void* pBuffer, int bufferLenght) = 0;
			virtual int ReceiveFrom(void* pBuffer, int bufferLenght) = 0;

			virtual int SendTo(const void* pBuffer, int bufferLenght, const PeerData& toPeer) = 0;
			virtual int ReceiveFrom(void* pBuffer, int bufferLenght, PeerData& fromPeer) = 0;
	};
}
