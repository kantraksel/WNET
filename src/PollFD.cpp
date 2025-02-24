#include "Platform.h"
#include "wnet.h"

using namespace WNET;
static_assert(sizeof(PollFD) == sizeof(pollfd), "Poll structs do not match!");

PollFD::PollFD()
{
	fd = INVALID_SOCKET;
	events = POLLIN;
	revents = 0;
}

bool PollFD::isSignaled()
{
	return revents & POLLIN;
}

int PollFD::Poll(PollFD* pFDs, int count, int timeout)
{
#ifdef _WIN32
	return WSAPoll((pollfd*)pFDs, count, timeout);
#else
	return poll((pollfd*)pFDs, count, timeout);
#endif
}
