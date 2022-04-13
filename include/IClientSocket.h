#pragma once

namespace WNET
{
	class IClientSocket : public ISocket
	{
		public:
			static IClientSocket* Create();
			virtual ~IClientSocket();

			virtual bool Connect(const char* host, unsigned short port) = 0;
	};
}
