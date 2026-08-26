#include "Platform.h"
#include "wnet.h"

using namespace WNET;

UdpSocket::UdpSocket()
{
}

UdpSocket::~UdpSocket()
{
}

bool UdpSocket::Create()
{
	return Socket::Create(SOCK_DGRAM, IPPROTO_UDP);
}

int UdpSocket::SendTo(const void* pBuffer, int bufferLenght)
{
	return SendTo(pBuffer, bufferLenght, peer);
}

int UdpSocket::ReceiveFrom(void* pBuffer, int bufferLenght)
{
	return ReceiveFrom(pBuffer, bufferLenght, peer);
}

int UdpSocket::SendTo(const void* pBuffer, int bufferLenght, const Endpoint& toPeer)
{
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = toPeer.address;
	addr.sin_port = toPeer.port;

	return sendto(socket, (const char*)pBuffer, bufferLenght, 0, (sockaddr*)&addr, sizeof(addr));
}

int UdpSocket::ReceiveFrom(void* pBuffer, int bufferLenght, Endpoint& fromPeer)
{
	sockaddr_in addr;
	socklen_t size = sizeof(addr);

	int returnValue = recvfrom(socket, (char*)pBuffer, bufferLenght, 0, (sockaddr*)&addr, &size);
	if (returnValue != SOCKET_ERROR)
	{
		fromPeer.address = addr.sin_addr.s_addr;
		fromPeer.port = addr.sin_port;
	}

	return returnValue;
}

bool UdpSocket::SetBroadcastFlag(bool enabled)
{
	int flag = enabled ? 1 : 0;
	return setsockopt(socket, SOL_SOCKET, SO_BROADCAST, (char*)&flag, sizeof(flag)) != SOCKET_ERROR;
}

int UdpSocket::GetMessageMaxSize()
{
	// IPv4 header - 20 bytes
	// UDP header - 8 bytes

	int size;
	socklen_t var = sizeof(size);
	if (getsockopt(socket, IPPROTO_IP, IP_MTU, (char*)&size, &var) != SOCKET_ERROR)
		return size - 28;
	else
		return -1;
}

bool UdpSocket::SetFragmentationFlag(bool enabled)
{
	int value = IP_PMTUDISC_DO;
	return setsockopt(socket, IPPROTO_IP, IP_MTU_DISCOVER, (char*)&value, sizeof(value)) != SOCKET_ERROR;
}
