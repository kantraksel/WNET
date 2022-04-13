#pragma once

namespace WNET
{
	struct Subsystem
	{
		static bool Initialize();
		static int Release();

		static DNSResponse* ResolveLocalIPs();
		static DNSResponse* ResolveHostName(const char* host);

		static void GetPeerInfo(PeerData& data, PeerInfo& info);
	};
}
