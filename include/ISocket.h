#pragma once

namespace WNET
{
	class ISocket
	{
		public:
			virtual ~ISocket();

			virtual bool CreateSocket() = 0;
			virtual void CloseSocket() = 0;
			virtual bool BindSocket(const char* host, unsigned short port) = 0;
			virtual bool Connect(const char* host, unsigned short port) = 0;

			virtual int Send(const void* pBuffer, int bufferLenght) = 0;
			virtual int Receive(void* pBuffer, int bufferLenght) = 0;
			virtual bool Poll(int timeout) = 0;

			virtual bool GetSocketLastError(int* pErrorCode) = 0;
			virtual bool GetMessageMaxSize(unsigned int& size) = 0;

			virtual int GetLastError() = 0;
			virtual PeerData& GetPeerData() = 0;
			virtual bool SetBlockingMode(bool value) = 0;
			virtual SOCKET GetSocket() = 0;

			virtual void SetExternalData(void* data) = 0;
			virtual void* GetExternalData() = 0;
	};
}
