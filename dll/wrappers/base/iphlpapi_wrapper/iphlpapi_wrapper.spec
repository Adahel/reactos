@ stdcall AddIPAddress( long long long ptr ptr )
@ stdcall AllocateAndGetArpEntTableFromStack(ptr long ptr long long)
@ stdcall AllocateAndGetIfTableFromStack( ptr long long long )
@ stdcall AllocateAndGetIpAddrTableFromStack( ptr long long long )
@ stdcall AllocateAndGetIpForwardTableFromStack( ptr long long long )
@ stdcall AllocateAndGetIpNetTableFromStack( ptr long long long )
@ stdcall AllocateAndGetTcpExTable2FromStack(long long ptr long ptr ptr)
@ stdcall AllocateAndGetTcpExTableFromStack(ptr long ptr long long)
@ stdcall AllocateAndGetTcpTableFromStack( ptr long long long )
@ stdcall AllocateAndGetUdpExTable2FromStack(long long ptr long ptr ptr)
@ stdcall AllocateAndGetUdpExTableFromStack(ptr long ptr long long)
@ stdcall AllocateAndGetUdpTableFromStack( ptr long long long )
@ stdcall CancelIPChangeNotify(ptr)
@ stdcall CancelSecurityHealthChangeNotify(ptr) iphlpapibase.CancelSecurityHealthChangeNotify
@ stdcall CreateIpForwardEntry( ptr )
@ stdcall CreateIpNetEntry( ptr )
@ stdcall CreateProxyArpEntry( long long long )
@ stdcall DeleteIPAddress( long )
@ stdcall DeleteIpForwardEntry( ptr )
@ stdcall DeleteIpNetEntry( ptr )
@ stdcall DeleteProxyArpEntry( long long long )
@ stdcall DisableMediaSense(ptr ptr)
@ stdcall EnableRouter( ptr ptr )
@ stdcall FlushIpNetTable( long )
@ stdcall FlushIpNetTableFromStack(long)
@ stdcall GetAdapterIndex( wstr ptr )
@ stdcall GetAdapterOrderMap()
@ stdcall GetAdaptersAddresses( long long ptr ptr ptr )
@ stdcall GetAdaptersInfo( ptr ptr )
@ stdcall GetBestInterface( long ptr )
@ stdcall GetBestInterfaceEx(ptr ptr)
@ stdcall GetBestInterfaceFromStack(ptr ptr)
@ stdcall GetBestRoute(long long ptr)
@ stdcall GetBestRouteFromStack(ptr ptr ptr)
@ stdcall GetExtendedTcpTable( ptr ptr long long long long )
@ stdcall GetExtendedUdpTable(ptr ptr long long long long)
@ stdcall GetFriendlyIfIndex( long )
@ stdcall GetIcmpStatistics( ptr )
@ stdcall GetIcmpStatisticsEx(ptr long)
@ stdcall GetIcmpStatsFromStack(ptr)
@ stdcall GetIcmpStatsFromStackEx(ptr ptr)
@ stdcall GetIfEntry( ptr )
@ stdcall GetIfEntryFromStack(ptr ptr ptr)
@ stdcall GetIfTable( ptr ptr long )
@ stdcall GetIfTableFromStack(ptr ptr ptr ptr)
@ stdcall GetIgmpList(long ptr ptr)
@ stdcall GetInterfaceInfo( ptr ptr )
@ stdcall GetIpAddrTable( ptr ptr long )
@ stdcall GetIpAddrTableFromStack(ptr ptr ptr)
@ stdcall GetIpErrorString(long ptr ptr)
@ stdcall GetIpForwardTable( ptr ptr long )
@ stdcall GetIpForwardTableFromStack(ptr ptr ptr)
@ stdcall GetIpNetTable( ptr ptr long )
@ stdcall GetIpNetTableFromStack(ptr long long long)
@ stdcall GetIpStatistics( ptr )
@ stdcall GetIpStatisticsEx(ptr long)
@ stdcall GetIpStatsFromStack(ptr)
@ stdcall GetIpStatsFromStackEx(ptr ptr)
@ stdcall GetNetworkParams( ptr ptr)
@ stdcall GetNumberOfInterfaces( ptr )
@ stdcall GetOwnerModuleFromTcp6Entry(ptr long ptr ptr)
@ stdcall GetOwnerModuleFromTcpEntry ( ptr long ptr ptr )
@ stdcall GetOwnerModuleFromUdp6Entry(ptr long ptr ptr)
@ stdcall GetOwnerModuleFromUdpEntry(ptr long ptr ptr)
@ stdcall GetPerAdapterInfo( long ptr ptr )
@ stdcall GetRTTAndHopCount( long ptr long ptr )
@ stdcall GetTcpExTable2FromStack(long long long ptr ptr)
@ stdcall GetTcpStatistics( ptr )
@ stdcall GetTcpStatisticsEx(ptr long)
@ stdcall GetTcpStatsFromStack(ptr)
@ stdcall GetTcpStatsFromStackEx(ptr ptr)
@ stdcall GetTcpTable( ptr ptr long )
@ stdcall GetTcpTableFromStack(ptr ptr ptr)
@ stdcall GetUdpExTable2FromStack(long long long ptr ptr)
@ stdcall GetUdpStatistics( ptr )
@ stdcall GetUdpStatisticsEx(ptr long)
@ stdcall GetUdpStatsFromStack(ptr)
@ stdcall GetUdpStatsFromStackEx(ptr ptr)
@ stdcall GetUdpTable( ptr ptr long )
@ stdcall GetUdpTableFromStack(ptr ptr ptr)
@ stdcall GetUniDirectionalAdapterInfo( ptr ptr )
@ stdcall Icmp6CreateFile()
@ stdcall Icmp6ParseReplies(ptr long)
@ stdcall Icmp6SendEcho2(ptr ptr ptr ptr ptr ptr ptr long ptr ptr long long)
@ stdcall IcmpCloseHandle(ptr) 
@ stdcall IcmpCreateFile() 
@ stdcall IcmpParseReplies(ptr long) 
@ stdcall IcmpSendEcho2(ptr ptr ptr ptr long ptr long ptr ptr long long) 
@ stdcall IcmpSendEcho(ptr long ptr long ptr ptr long long)
@ stdcall InternalCreateIpForwardEntry(ptr)
@ stdcall InternalCreateIpNetEntry(ptr)
@ stdcall InternalDeleteIpForwardEntry(ptr)
@ stdcall InternalDeleteIpNetEntry(ptr)
@ stdcall InternalGetIfTable(ptr ptr long)
@ stdcall InternalGetIpAddrTable(ptr ptr long)
@ stdcall InternalGetIpForwardTable(ptr ptr long)
@ stdcall InternalGetIpNetTable(ptr ptr long)
@ stdcall InternalGetTcpTable(ptr ptr long)
@ stdcall InternalGetUdpTable(ptr ptr long)
@ stdcall InternalSetIfEntry(ptr)
@ stdcall InternalSetIpForwardEntry(ptr)
@ stdcall InternalSetIpNetEntry(ptr)
@ stdcall InternalSetIpStats(ptr)
@ stdcall InternalSetTcpEntry(ptr)
@ stdcall IpReleaseAddress( ptr )
@ stdcall IpRenewAddress( ptr )
@ stdcall IsLocalAddress(ptr long)
@ stdcall NTPTimeToNTFileTime(long ptr long)
@ stdcall NTTimeToNTPTime(ptr)
@ stdcall NhGetGuidFromInterfaceName(long long long long)
@ stdcall NhGetInterfaceNameFromDeviceGuid(long long long long long)
@ stdcall NhGetInterfaceNameFromGuid(long long long long long)
@ stdcall NhpAllocateAndGetInterfaceInfoFromStack(ptr ptr long ptr long)
@ stdcall NhpGetInterfaceIndexFromStack(wstr ptr)
@ stdcall NotifyAddrChange( ptr ptr )
@ stdcall NotifyRouteChange( ptr ptr )
@ stdcall NotifyRouteChangeEx(ptr ptr long)
@ stdcall NotifySecurityHealthChange(ptr ptr ptr) iphlpapibase.NotifySecurityHealthChange
@ stdcall RestoreMediaSense(ptr ptr)
@ stdcall SendARP(long long ptr ptr)
@ stdcall SetAdapterIpAddress(ptr long ptr ptr ptr)
@ stdcall SetBlockRoutes(ptr ptr ptr)
@ stdcall SetIfEntry( ptr )
@ stdcall SetIfEntryToStack(ptr ptr)
@ stdcall SetIpForwardEntry( ptr )
@ stdcall SetIpForwardEntryToStack( ptr )
@ stdcall SetIpMultihopRouteEntryToStack(ptr)
@ stdcall SetIpNetEntry( ptr )
@ stdcall SetIpNetEntryToStack(ptr long)
@ stdcall SetIpRouteEntryToStack(ptr)
@ stdcall SetIpStatistics( ptr )
@ stdcall SetIpStatsToStack(ptr)
@ stdcall SetIpTTL( long )
@ stdcall SetProxyArpEntryToStack(long long long long long)
@ stdcall SetRouteWithRef(ptr)
@ stdcall SetTcpEntry( ptr )
@ stdcall SetTcpEntryToStack(ptr)
@ stdcall UnenableRouter( ptr ptr )
@ stdcall _PfAddFiltersToInterface@24(ptr long ptr long ptr ptr) 
@ stdcall _PfAddGlobalFilterToInterface@8(ptr long) 
@ stdcall _PfBindInterfaceToIPAddress@12(ptr ptr ptr) 
@ stdcall _PfBindInterfaceToIndex@16(ptr long ptr ptr) 
@ stdcall _PfCreateInterface@24(long long long long long ptr) 
@ stdcall _PfDeleteInterface@4(ptr) 
@ stdcall _PfDeleteLog@0() 
@ stdcall _PfGetInterfaceStatistics@16(ptr ptr ptr long) 
@ stdcall _PfMakeLog@4(ptr) 
@ stdcall _PfRebindFilters@8(ptr ptr) 
@ stdcall _PfRemoveFilterHandles@12(ptr long ptr) 
@ stdcall _PfRemoveFiltersFromInterface@20(ptr long ptr long ptr) 
@ stdcall _PfRemoveGlobalFilterFromInterface@8(ptr long) 
@ stdcall _PfSetLogBuffer@28(ptr long long long ptr ptr ptr) 
@ stdcall _PfTestPacket@20(ptr ptr long ptr ptr) 
@ stdcall _PfUnBindInterface@4(ptr) 
@ stdcall do_echo_rep(long ptr long long long long long long ptr long)
@ stdcall do_echo_req(ptr ptr ptr long ptr long long long ptr long)
@ stdcall register_icmp()

;Vista functions
@ stdcall CancelMibChangeNotify2(ptr)
@ stdcall ConvertLengthToIpv4Mask(long ptr)
@ stdcall ConvertInterfaceGuidToLuid(ptr ptr)
@ stdcall ConvertInterfaceIndexToLuid( long ptr )
@ stdcall ConvertInterfaceLuidToIndex(ptr ptr)
@ stdcall ConvertInterfaceLuidToNameA( ptr ptr long )
@ stdcall ConvertInterfaceLuidToNameW( ptr ptr long )
@ stdcall ConvertInterfaceNameToLuidA( str ptr )
@ stdcall ConvertInterfaceNameToLuidW( wstr ptr )
@ stdcall CreateSortedAddressPairs(ptr long ptr long long ptr ptr)
@ stdcall FreeMibTable(ptr)
@ stdcall GetIfEntry2(ptr)
@ stdcall GetIpNetEntry2(ptr)
@ stdcall GetTeredoPort(ptr)
@ stdcall NotifyStableUnicastIpAddressTable(long ptr ptr ptr ptr)
@ stdcall ParseNetworkString(ptr long ptr ptr ptr)
@ stdcall ResolveNeighbor(ptr ptr ptr)
@ stdcall ResolveIpNetEntry2(ptr ptr)
@ stdcall ConvertInterfaceLuidToGuid(ptr ptr)
@ stdcall GetIfTable2Ex(long ptr)
@ stdcall GetIfTable2(long ptr)
@ stdcall GetIpInterfaceEntry(ptr)
@ stdcall if_indextoname(long ptr) IPHLP_if_indextoname
@ stdcall if_nametoindex(str) IPHLP_if_nametoindex
@ stdcall NotifyUnicastIpAddressChange(long ptr ptr long ptr)
@ stdcall NotifyRouteChange2(ptr ptr ptr long ptr)
@ stdcall GetBestRoute2(ptr long ptr ptr long ptr ptr)
@ stdcall NotifyIpInterfaceChange(long ptr ptr long ptr)