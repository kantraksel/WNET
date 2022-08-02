#pragma once
#ifdef _WIN32
	#include <WinSock2.h>
	#define PARAMLENTYPE int
	#define SocketSetLastError(param) WSASetLastError(param)
#else
	#include <unistd.h>
	#include <sys/socket.h>
	#include <errno.h>
	#include <netdb.h>
	#include <poll.h>

	#define PARAMLENTYPE unsigned int
	#define SocketSetLastError(param) (errno = param)
	#define INVALID_SOCKET -1
	#define SOCKET_ERROR -1
	#define closesocket close
#endif
#include "wnet.h"

namespace WNET
{
	class Socket : ISocket
	{
		protected:
			SOCKET socket;
			PeerData peerData;
			void* externalData;
			pollfd pollFD;

		public:
			Socket();
			virtual ~Socket() override;

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
