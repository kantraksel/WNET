#include "UDPSocket.h"
#include "SocketFactory.h"

using namespace WNET;

IUDPSocket* IUDPSocket::Create()
{
	return new UDPSocket();
}

void IUDPSocket::Close(IUDPSocket* pSocket)
{
	delete pSocket;
}

IUDPSocket::~IUDPSocket() {}

UDPSocket::UDPSocket()
{
	CreateSocket();
}

UDPSocket::~UDPSocket()
{

}

bool UDPSocket::CreateSocket()
{
	SocketSetLastError(0);

	if (socket != INVALID_SOCKET)
		return false;

	addrinfo info;
	memset(&info, 0, sizeof(info));
	info.ai_family = AF_INET;
	info.ai_socktype = SOCK_DGRAM;
	info.ai_protocol = IPPROTO_UDP;

	socket = SocketFactory::CreateSocket(info);
	pollFD.fd = socket;
	
	return socket != INVALID_SOCKET;
}

void UDPSocket::CloseSocket()
{
	closesocket(socket);
	socket = INVALID_SOCKET;
	pollFD.fd = INVALID_SOCKET;
}

bool UDPSocket::BindSocket(const char* host, unsigned short port)
{
	SocketSetLastError(0);

	addrinfo info = SocketFactory::CreateAddrInfo(host, port);
	bool success = info.ai_addr;
	success = success && bind(socket, info.ai_addr, info.ai_addrlen) != SOCKET_ERROR;
	
	SocketFactory::ReleaseAddrInfo(info);
	return success;
}

bool UDPSocket::Connect(const char* host, unsigned short port)
{
	SocketSetLastError(0);

	addrinfo info = SocketFactory::CreateAddrInfo(host, port);
	bool success = info.ai_addr;
	success = success && connect(socket, info.ai_addr, info.ai_addrlen) != SOCKET_ERROR;

	SocketFactory::ReleaseAddrInfo(info);
	return success;
}

int UDPSocket::SendTo(const void* pBuffer, int bufferLenght)
{
	return SendTo(pBuffer, bufferLenght, peerData);
}

int UDPSocket::ReceiveFrom(void* pBuffer, int bufferLenght)
{
	return ReceiveFrom(pBuffer, bufferLenght, peerData);
}

int UDPSocket::SendTo(const void* pBuffer, int bufferLenght, const PeerData& toPeer)
{
	sockaddr_in peerAddr;
	peerAddr.sin_family = AF_INET;
	peerAddr.sin_addr.s_addr = toPeer.address;
	peerAddr.sin_port = htons(toPeer.port);

	return sendto(socket, (const char*)pBuffer, bufferLenght, 0, (sockaddr*)&peerAddr, sizeof(peerAddr));
}

int UDPSocket::ReceiveFrom(void* pBuffer, int bufferLenght, PeerData& fromPeer)
{
	sockaddr_in peerAddr;

	PARAMLENTYPE returnValue = sizeof(sockaddr_in);
	returnValue = recvfrom(socket, (char*)pBuffer, bufferLenght, 0, (sockaddr*)&peerAddr, &returnValue);
	if (returnValue != SOCKET_ERROR)
	{
		fromPeer.address = peerAddr.sin_addr.s_addr;
		fromPeer.port = ntohs(peerAddr.sin_port);
	}

	return returnValue;
}

//Socket
int UDPSocket::Send(const void* pBuffer, int bufferLenght)
{
	return Socket::Send(pBuffer, bufferLenght);
}

int UDPSocket::Receive(void* pBuffer, int bufferLenght)
{
	return Socket::Receive(pBuffer, bufferLenght);
}

bool UDPSocket::Poll(int timeout)
{
	return Socket::Poll(timeout);
}

bool UDPSocket::GetSocketLastError(int* pErrorCode)
{
	return Socket::GetSocketLastError(pErrorCode);
}

bool UDPSocket::GetMessageMaxSize(unsigned int& size)
{
	return Socket::GetMessageMaxSize(size);
}

int UDPSocket::GetLastError()
{
	return Socket::GetLastError();
}

PeerData& UDPSocket::GetPeerData()
{
	return Socket::GetPeerData();
}

bool UDPSocket::SetBlockingMode(bool value)
{
	return Socket::SetBlockingMode(value);
}

SOCKET UDPSocket::GetSocket()
{
	return Socket::GetSocket();
}

void UDPSocket::SetExternalData(void* data)
{
	return Socket::SetExternalData(data);
}

void* UDPSocket::GetExternalData()
{
	return Socket::GetExternalData();
}
