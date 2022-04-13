#pragma once

namespace WNET
{
	class ISocket
	{
		protected:
			SOCKET isocket;
			PeerData peerData;
			ErrorInfo errorInfo;
			void* externalData;

			ISocket();

		public:
			virtual ~ISocket();

			virtual int Send(const void* pBuffer, int bufferLenght);
			virtual int Receive(void* pBuffer, int bufferLenght);

			virtual int SendTo(const void* pBuffer, int bufferLenght);
			virtual int ReceiveFrom(void* pBuffer, int bufferLenght);

			virtual int SendTo(const void* pBuffer, int bufferLenght, PeerData& toPeer);
			virtual int ReceiveFrom(void* pBuffer, int bufferLenght, PeerData& fromPeer);

			virtual unsigned int CheckSocketLastError(int* pErrorCode);
			virtual int GetMaxPacketSize(unsigned int* pMPS);

			virtual PeerData& GetPeerData();
			virtual SOCKET GetSocket();
			virtual ErrorInfo GetLastError();
			virtual bool SetBlockingMode(bool isBlocking);

			virtual void SetExternalData(void* data);
			virtual void* GetExternalData();
	};
}
