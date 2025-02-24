#pragma once

namespace WNET
{
	class UdpSocket : public Socket
	{
		public:
			UdpSocket();
			~UdpSocket();

			bool Create();

			int SendTo(const void* pBuffer, int bufferLenght);
			int ReceiveFrom(void* pBuffer, int bufferLenght);
			int SendTo(const void* pBuffer, int bufferLenght, const Endpoint& toPeer);
			int ReceiveFrom(void* pBuffer, int bufferLenght, Endpoint& fromPeer);

			bool SetBroadcastFlag(bool enabled);
			bool GetMessageMaxSize(unsigned int& size);
	};
}
