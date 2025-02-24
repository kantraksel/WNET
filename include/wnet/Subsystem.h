#pragma once

namespace WNET
{
	struct Subsystem
	{
		static bool Initialize();
		static bool Release();
		static int GetLastError();

		static DnsResponse ResolveLocalIPs();
		static DnsResponse ResolveHostName(const char* host);
	};
}
