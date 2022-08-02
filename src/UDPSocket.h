#pragma once
#include "Socket.h"

namespace WNET
{
	class UDPSocket : public IUDPSocket, Socket
	{
		public:
			UDPSocket();
			virtual ~UDPSocket() override;

			virtual bool CreateSocket() override;
			virtual void CloseSocket() override;
			virtual bool BindSocket(const char* host, unsigned short port) override;
			virtual bool Connect(const char* host, unsigned short port) override;

			virtual int SendTo(const void* pBuffer, int bufferLenght) override;
			virtual int ReceiveFrom(void* pBuffer, int bufferLenght) override;

			virtual int SendTo(const void* pBuffer, int bufferLenght, const PeerData& toPeer) override;
			virtual int ReceiveFrom(void* pBuffer, int bufferLenght, PeerData& fromPeer) override;

			//Socket
			virtual int Send(const void* pBuffer, int bufferLenght) override;
			virtual int Receive(void* pBuffer, int bufferLenght) override;
			virtual bool Poll(int timeout) override;

			virtual bool GetSocketLastError(int* pErrorCode) override;
			virtual bool GetMessageMaxSize(unsigned int& size) override;

			virtual int GetLastError() override;
			virtual PeerData& GetPeerData() override;
			virtual bool SetBlockingMode(bool value) override;
			virtual SOCKET GetSocket() override;

			virtual void SetExternalData(void* data) override;
			virtual void* GetExternalData() override;
	};
}
