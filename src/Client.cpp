#include "ClientSocket.h"
#include "SocketFactory.h"

using namespace WNET;

IClientSocket* IClientSocket::Create()
{
	return new ClientSocket();
}

IClientSocket::~IClientSocket()
{

}

ClientSocket::ClientSocket()
{

}

ClientSocket::~ClientSocket()
{

}

bool ClientSocket::Connect(const char* host, unsigned short port)
{
	errorInfo.section = ErrorSection::IClientSocketConnect;
	errorInfo.error = 0;
	errorInfo.details = 0;

	addrinfo info = SocketFactory::CreateAddrInfo(host, port);
	if (!info.ai_addr) errorInfo.error = 1;
	else if ((isocket = SocketFactory::CreateSocket(info)) == INVALID_SOCKET)
	{
		errorInfo.error = 2;
		errorInfo.details = SocketLastError;
	}
	else if (connect(isocket, info.ai_addr, info.ai_addrlen) == SOCKET_ERROR)
	{
		errorInfo.error = 3;
		errorInfo.details = SocketLastError;
		closesocket(isocket);
		isocket = INVALID_SOCKET;
	}
	SocketFactory::ReleaseAddrInfo(info);
	return errorInfo.error == 0;
}
