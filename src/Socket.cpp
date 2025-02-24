#include "Platform.h"
#include "wnet.h"

using namespace WNET;

Socket::Socket()
{
	socket = INVALID_SOCKET;
	peer.address = 0;
	peer.port = 0;
	userData = 0;
}

Socket::~Socket()
{
	if (socket != INVALID_SOCKET)
		closesocket(socket);
}

bool Socket::Create(int type, int proto)
{
	SocketSetLastError(0);
	if (socket != INVALID_SOCKET)
		return false;

	socket = ::socket(AF_INET, type, proto);
	return socket != INVALID_SOCKET;
}

void Socket::Close()
{
	closesocket(socket);
	socket = INVALID_SOCKET;
}

bool Socket::Bind(const char* host, unsigned short port)
{
	SocketSetLastError(0);

	AddressPtr addr{ host, port };
	Endpoint ep;
	if (!addr.ToEndpoint(ep))
		return false;
	return Bind(ep);
}

bool Socket::Bind(Endpoint& ep)
{
	SocketSetLastError(0);

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = ep.address;
	addr.sin_port = ep.port;
	
	return bind(socket, (sockaddr*)&addr, sizeof(addr)) != SOCKET_ERROR;
}

bool Socket::Connect(const char* host, unsigned short port)
{
	SocketSetLastError(0);

	AddressPtr addr{ host, port };
	Endpoint ep;
	if (!addr.ToEndpoint(ep))
		return false;
	return Connect(ep);
}

bool Socket::Connect(Endpoint& ep)
{
	SocketSetLastError(0);

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = ep.address;
	addr.sin_port = ep.port;
	
	peer = ep;
	return connect(socket, (sockaddr*)&addr, sizeof(addr)) != SOCKET_ERROR;
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
	pollfd pollFD{};
	pollFD.fd = socket;
	pollFD.events = POLLIN;
	pollFD.revents = 0;

#if _WIN32
	int result = WSAPoll(&pollFD, 1, timeout);
#else
	int result = poll(&pollFD, 1, timeout);
#endif
	return result > 0;
}

bool Socket::GetSocketLastError(int& errorCode)
{
	socklen_t size = sizeof(errorCode);
	return getsockopt(socket, SOL_SOCKET, SO_ERROR, (char*)&errorCode, &size) != SOCKET_ERROR;
}

int Socket::GetLastError()
{
	return Subsystem::GetLastError();
}

Endpoint& Socket::GetPeer()
{
	return peer;
}

Address Socket::GetPeerAddress()
{
	return peer.ToAddress();
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

void Socket::SetUserData(void* data)
{
	userData = data;
}

void* Socket::GetUserData()
{
	return userData;
}
