#pragma once

namespace WNET
{
	enum ErrorSection
	{
		None = 0,

		ISocketSend = 1,
		ISocketReceive,
		ISocketCheckSocketLastError,
		ISocketGetMaxPacketSize,
		ISocketSendTo,
		ISocketReceiveFrom,

		IClientSocketConnect = 101,

		IHostSocketBind = 201,
		IHostSocketUnBind,
		IHostSocketPoll,
	};

	struct ErrorInfo
	{
		ErrorSection section;
		int error;
		int details;
	};
}
