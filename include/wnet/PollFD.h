#pragma once

namespace WNET
{
	struct PollFD
	{
		SOCKET fd;
		short events;
		short revents;

		PollFD();
		bool isSignaled();

		inline void clearSignal()
		{
			revents = 0;
		}

		static int Poll(PollFD* pFDs, int count, int timeout);
	};
}
