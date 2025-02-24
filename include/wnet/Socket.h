#pragma once

namespace WNET
{
	class Socket
	{
		protected:
			SOCKET socket;
			Endpoint peer;
			void* userData;

			bool Create(int type, int proto);

		public:
			Socket();
			~Socket();

			//bool Create();
			void Close();

			bool Bind(const char* host, unsigned short port);
			bool Bind(Endpoint& ep);
			bool Connect(const char* host, unsigned short port);
			bool Connect(Endpoint& ep);

			int Send(const void* pBuffer, int bufferLenght);
			int Receive(void* pBuffer, int bufferLenght);
			bool Poll(int timeout);

			bool GetSocketLastError(int& errorCode);
			int GetLastError();
			Endpoint& GetPeer();
			Address GetPeerAddress();
			bool SetBlockingMode(bool value);
			SOCKET GetSocket();

			void SetUserData(void* data);
			void* GetUserData();
	};
}
