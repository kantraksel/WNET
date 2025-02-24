#pragma once

#ifdef _WIN32
	#include <WS2tcpip.h>
	typedef int socklen_t;
	#define SocketSetLastError(param) WSASetLastError(param)
#else
	#include <unistd.h>
	#include <sys/socket.h>
	#include <cerrno>
	#include <netdb.h>
	#include <poll.h>
	#include <arpa/inet.h>
	#include <fcntl.h>
	#include <netinet/tcp.h>

	#define SocketSetLastError(param) (errno = param)
	#define INVALID_SOCKET -1
	#define SOCKET_ERROR -1
	#define closesocket close
#endif
