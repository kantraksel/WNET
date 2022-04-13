#include "Socket.h"
#include "SocketFactory.h"
#include "ErrorCodes.h"

#ifdef _WINDOWS
	#pragma comment(lib, "Ws2_32.lib")
	#define PARAMLENTYPE int
#else
	#include <fcntl.h>
	#define PARAMLENTYPE unsigned int
#endif

using namespace WNET;

ISocket::ISocket()
{
	isocket = INVALID_SOCKET;

	memset(&peerData, 0, sizeof(peerData));
	memset(&errorInfo, 0, sizeof(errorInfo));
	externalData = 0;
}

ISocket::~ISocket()
{
	if (isocket != INVALID_SOCKET) closesocket(isocket);
}

int ISocket::Send(const void* pBuffer, int bufferLenght)
{
	errorInfo.section = ErrorSection::ISocketSend;
	errorInfo.error = 0;
	errorInfo.details = 0;

	int returnValue = send(isocket, (const char*)pBuffer, bufferLenght, 0);
	if (returnValue == SOCKET_ERROR) errorInfo.details = SocketLastError;
	return returnValue;
}

int ISocket::Receive(void* pBuffer, int bufferLenght)
{
	errorInfo.section = ErrorSection::ISocketReceive;
	errorInfo.error = 0;
	errorInfo.details = 0;

	int returnValue = recv(isocket, (char*)pBuffer, bufferLenght, 0);
	if (returnValue == SOCKET_ERROR) errorInfo.details = SocketLastError;
	return returnValue;
}

unsigned int ISocket::CheckSocketLastError(int* pErrorCode)
{
	errorInfo.section = ErrorSection::ISocketCheckSocketLastError;
	errorInfo.error = 0;
	errorInfo.details = 0;

	PARAMLENTYPE error_code_size = sizeof(int);
	error_code_size = getsockopt(isocket, SOL_SOCKET, SO_ERROR, (char*)pErrorCode, &error_code_size);
	if (error_code_size == SOCKET_ERROR) errorInfo.details = SocketLastError;
	
	return error_code_size;
}

int ISocket::GetMaxPacketSize(unsigned int* pMPS)
{
	errorInfo.section = ErrorSection::ISocketGetMaxPacketSize;
	errorInfo.error = 0;
	errorInfo.details = 0;

#ifdef _WINDOWS
	int error_code_size = sizeof(unsigned int);
	error_code_size = getsockopt(isocket, SOL_SOCKET, SO_MAX_MSG_SIZE, (char*)pMPS, &error_code_size);
	if (error_code_size == SOCKET_ERROR) errorInfo.details = SocketLastError;

	return error_code_size;
#else
	return SOCKET_ERROR;
#endif
}

int ISocket::SendTo(const void* pBuffer, int bufferLenght)
{
	return SendTo(pBuffer, bufferLenght, peerData);
}

int ISocket::ReceiveFrom(void* pBuffer, int bufferLenght)
{
	return ReceiveFrom(pBuffer, bufferLenght, peerData);
}

int ISocket::SendTo(const void* pBuffer, int bufferLenght, PeerData& toPeer)
{
	errorInfo.section = ErrorSection::ISocketSendTo;
	errorInfo.error = 0;
	errorInfo.details = 0;

	sockaddr_in peerAddr;
	peerAddr.sin_family = AF_INET;
	peerAddr.sin_addr.s_addr = toPeer.address;
	peerAddr.sin_port = htons(toPeer.port);

	int returnValue = sendto(isocket, (const char*)pBuffer, bufferLenght, 0, (sockaddr*)&peerAddr, sizeof(peerAddr));
	if (returnValue == SOCKET_ERROR) errorInfo.details = SocketLastError;

	return returnValue;
}

int ISocket::ReceiveFrom(void* pBuffer, int bufferLenght, PeerData& fromPeer)
{
	//TODO: SendTo and ReceivedFrom may be called async - errorInfo will be overwritten
	errorInfo.section = ErrorSection::ISocketReceiveFrom;
	errorInfo.error = 0;
	errorInfo.details = 0;

	sockaddr_in peerAddr;

	PARAMLENTYPE returnValue = sizeof(sockaddr_in);
	returnValue = recvfrom(isocket, (char*)pBuffer, bufferLenght, 0, (sockaddr*)&peerAddr, &returnValue);
	if (returnValue == SOCKET_ERROR) errorInfo.details = SocketLastError;
	else
	{
		fromPeer.address = peerAddr.sin_addr.s_addr;
		fromPeer.port = ntohs(peerAddr.sin_port);
	}

	return returnValue;
}

PeerData& ISocket::GetPeerData()
{
	return peerData;
}

SOCKET ISocket::GetSocket()
{
	return isocket;
}

ErrorInfo ISocket::GetLastError()
{
	return errorInfo;
}

bool ISocket::SetBlockingMode(bool isBlocking)
{
#ifdef _WINDOWS
	u_long l = isBlocking ? 0 : 1;
	return ioctlsocket(isocket, FIONBIO, &l) == 0;
#else
	int flags = fcntl(isocket, F_GETFL);
	if (isBlocking) flags &= ~O_NONBLOCK;
	else flags |= O_NONBLOCK;
	fcntl(isocket, F_SETFL, flags);
	return true;
#endif
}

void ISocket::SetExternalData(void* data)
{
	externalData = data;
}

void* ISocket::GetExternalData()
{
	return externalData;
}
