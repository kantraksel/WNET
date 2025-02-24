#pragma once

namespace WNET
{
	class TcpSocket : public Socket
	{
		public:
			TcpSocket();
			~TcpSocket();

			bool Create();
			
			bool Listen(unsigned int queue);
			bool TryAccept(TcpSocket& connection);
			bool FinalizeConnection();

			bool SetKeepAlive(bool enabled, unsigned int interval, unsigned int countTimeout);
	};
}
