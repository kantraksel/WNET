#include "Socket.h"
#include "SocketFactory.h"

#ifdef _WIN32
	#pragma comment(lib, "Ws2_32.lib")
	#define poll WSAPoll
#else
	#include <fcntl.h>
#endif

using namespace WNET;

ISocket::~ISocket() {}

Socket::Socket()
{
	socket = INVALID_SOCKET;

	memset(&peerData, 0, sizeof(peerData));
	externalData = 0;
	pollFD.events = POLLIN;
	pollFD.fd = INVALID_SOCKET;
}

Socket::~Socket()
{
	if (socket != INVALID_SOCKET) closesocket(socket);
}

int Socket::Send(const void* pBuffer, int bufferLenght)
{
	return send(socket, (const char*)pBuffer, bufferLenght, 0);
}

int Socket::Receive(void* pBuffer, int bufferLenght)
{
	return recv(socket, (char*)pBuffer, bufferLenght, 0);
}

bool Socket::Poll(int timeout)
{
	int result = poll(&pollFD, 1, timeout);
	return result > 0;
}

bool Socket::GetSocketLastError(int* pErrorCode)
{
	PARAMLENTYPE value = sizeof(int);
	value = getsockopt(socket, SOL_SOCKET, SO_ERROR, (char*)pErrorCode, &value);
	return value != SOCKET_ERROR;
}

bool Socket::GetMessageMaxSize(unsigned int& size)
{
#ifdef _WIN32
	int var = sizeof(unsigned int);
	var = getsockopt(socket, SOL_SOCKET, SO_MAX_MSG_SIZE, (char*)&size, &var);
	return var != SOCKET_ERROR;
#else
	return false;
#endif
}

int Socket::GetLastError()
{
	return Subsystem::GetLastError();
}

PeerData& Socket::GetPeerData()
{
	return peerData;
}

bool Socket::SetBlockingMode(bool value)
{
#ifdef _WIN32
	u_long l = value ? 0 : 1;
	return ioctlsocket(socket, FIONBIO, &l) == 0;
#else
	int flags = fcntl(socket, F_GETFL);
	if (value) flags &= ~O_NONBLOCK;
	else flags |= O_NONBLOCK;
	fcntl(socket, F_SETFL, flags);
	return true;
#endif
}

SOCKET Socket::GetSocket()
{
	return socket;
}

void Socket::SetExternalData(void* data)
{
	externalData = data;
}

void* Socket::GetExternalData()
{
	return externalData;
}
