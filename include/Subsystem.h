#pragma once

namespace WNET
{
	struct Subsystem
	{
		static bool Initialize();
		static int Release();
		static int GetLastError();

		static DnsResponse ResolveLocalIPs();
		static DnsResponse ResolveHostName(const char* host);
		static void ReleaseDnsResponse(DnsResponse& data);

		static void GetPeerInfo(const PeerData& data, PeerInfo& info);
		static bool GetPeerData(const PeerInfo& info, PeerData& data);
	};
}
