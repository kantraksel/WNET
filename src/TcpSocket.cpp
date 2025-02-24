#include "Platform.h"
#include "wnet.h"

using namespace WNET;

TcpSocket::TcpSocket()
{
}

TcpSocket::~TcpSocket()
{
}

bool TcpSocket::Create()
{
	return Socket::Create(SOCK_STREAM, IPPROTO_TCP);
}

bool TcpSocket::Listen(unsigned int queue)
{
	SocketSetLastError(0);
	return listen(socket, (int)queue) != SOCKET_ERROR;
}

bool TcpSocket::TryAccept(TcpSocket& connection)
{
	SocketSetLastError(0);

	sockaddr_in addr;
	socklen_t size = sizeof(addr);
	auto conn = accept(socket, (sockaddr*)&addr, &size);
	if (conn == INVALID_SOCKET)
		return false;

	if (connection.socket != INVALID_SOCKET)
		closesocket(socket);

	connection.socket = conn;
	connection.peer = Endpoint
	{
		addr.sin_addr.s_addr,
		addr.sin_port,
	};
	return true;
}

bool TcpSocket::FinalizeConnection()
{
#if _WIN32
	return shutdown(socket, SD_SEND) == 0;
#else
	return shutdown(socket, SHUT_WR) == 0;
#endif
}

bool TcpSocket::SetKeepAlive(bool enabled, unsigned int interval, unsigned int countTimeout)
{
	int value = enabled ? 1 : 0;
	if (setsockopt(socket, SOL_SOCKET, SO_KEEPALIVE, (char*)&value, sizeof(value)) == SOCKET_ERROR)
		return false;

	if (enabled)
	{
		value = interval;
		if (setsockopt(socket, IPPROTO_TCP, TCP_KEEPIDLE, (char*)&value, sizeof(value)) == SOCKET_ERROR)
			return false;
		if (setsockopt(socket, IPPROTO_TCP, TCP_KEEPINTVL, (char*)&value, sizeof(value)) == SOCKET_ERROR)
			return false;
		value = countTimeout;
		if (setsockopt(socket, IPPROTO_TCP, TCP_KEEPCNT, (char*)&value, sizeof(value)) == SOCKET_ERROR)
			return false;
	}
	
	return true;
}
