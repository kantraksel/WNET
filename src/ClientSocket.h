#pragma once
#include "Socket.h"

namespace WNET
{
	class ClientSocket : public IClientSocket
	{
		public:
			ClientSocket();
			~ClientSocket();

			bool Connect(const char* host, unsigned short port);
	};
}
