#include "HostSocket.h"
#include "SocketFactory.h"

#ifdef _WINDOWS
#define poll WSAPoll
#endif

using namespace WNET;

IHostSocket* IHostSocket::Create()
{
	return new HostSocket();
}

IHostSocket::~IHostSocket()
{

}

HostSocket::HostSocket()
{
	pollFD.events = POLLIN;
}

HostSocket::~HostSocket()
{

}

bool HostSocket::BindSocket(const char* host, unsigned short port)
{
	errorInfo.section = ErrorSection::IHostSocketBind;
	errorInfo.error = 0;
	errorInfo.details = 0;

	addrinfo info = SocketFactory::CreateAddrInfo(host, port);
	if (!info.ai_addr) errorInfo.error = 1;
	else if ((isocket = SocketFactory::CreateSocket(info)) == INVALID_SOCKET)
	{
		errorInfo.error = 2;
		errorInfo.details = SocketLastError;
	}
	else
	{
		if (bind(isocket, info.ai_addr, info.ai_addrlen) == SOCKET_ERROR) errorInfo.error = 3;

		if (errorInfo.error)
		{
			errorInfo.details = SocketLastError;
			closesocket(isocket);
			isocket = INVALID_SOCKET;
		}
	}

	pollFD.fd = isocket;
	SocketFactory::ReleaseAddrInfo(info);
	return errorInfo.error == 0;
}

void HostSocket::UnBindSocket()
{
	errorInfo.section = ErrorSection::IHostSocketUnBind;
	errorInfo.error = 0;
	errorInfo.details = 0;

	closesocket(isocket);
	isocket = INVALID_SOCKET;
	pollFD.fd = INVALID_SOCKET;
}

bool HostSocket::Poll(int timeout)
{
	errorInfo.section = ErrorSection::IHostSocketPoll;
	errorInfo.error = 0;
	errorInfo.details = 0;

	int result = poll(&pollFD, 1, timeout);
	if (result == SOCKET_ERROR)
	{
		errorInfo.details = SocketLastError;
	}

	return result > 0;
}

void HostSocket::SetPeerData(PeerData& peerData)
{
	this->peerData = peerData;
}
