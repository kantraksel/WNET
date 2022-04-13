#pragma once
#ifdef _WINDOWS
	#include <WinSock2.h>
	#define SocketLastError WSAGetLastError()
#else
	#include <unistd.h>
	#include <sys/socket.h>
	#include <errno.h>
	#include <netdb.h>

	#define INVALID_SOCKET -1
	#define closesocket close
	#define SocketLastError errno
	#define IN_ADDR in_addr
#endif
#include "wnet.h"

#define SOCKET_STATE_BLOCK 0
#define SOCKET_STATE_NONBLOCK 1
