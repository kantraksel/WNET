#pragma once

namespace WNET
{
	struct Subsystem
	{
		static bool Initialize();
		static bool Release();
		static int GetLastError();
		static void SetLastError(int value);

		static DnsResponse ResolveLocalIPs();
		static DnsResponse ResolveHostName(const char* host);
	};
}
