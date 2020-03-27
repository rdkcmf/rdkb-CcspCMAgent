/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2015 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

/**********************************************************************
   Copyright [2014] [Cisco Systems, Inc.]
 
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
 
       http://www.apache.org/licenses/LICENSE-2.0
 
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
**********************************************************************/


/**********************************************************************

    module:	cosa_apis_util.h

        This is base file for all parameters H files.

    ---------------------------------------------------------------

    description:

        This file contains all utility functions for COSA DML API development.

    ---------------------------------------------------------------

    environment:

        COSA independent

    ---------------------------------------------------------------

    author:

        Roger Hu
        leichen2@cisco.com

    ---------------------------------------------------------------

    revision:

        01/30/2011    initial revision.
        06/15/2012    add IPv4 address utils

**********************************************************************/


#ifndef  _COSA_APIS_UTIL_H
#define  _COSA_APIS_UTIL_H

#include "cosa_dml_api_common.h"

/**
 * @addtogroup CM_AGENT_TYPES
 * @{
 */
typedef struct StaticRoute 
{
    char         name[64];
    char         dest_lan_ip[16];
    char         netmask[16];
    char         gateway[16];
    int          metric;
    char         dest_intf[10];
    char         origin[16];
}StaticRoute;

#ifndef _BUILD_ANDROID
enum
{
    IPV6_ADDR_SCOPE_UNKNOWN,
    IPV6_ADDR_SCOPE_GLOBAL,
    IPV6_ADDR_SCOPE_LINKLOCAL,
    IPV6_ADDR_SCOPE_SITELOCAL,
    IPV6_ADDR_SCOPE_COMPATv4,
    IPV6_ADDR_SCOPE_LOOPBACK
};
#endif

typedef struct 
{
    int    scope;
    char   v6addr[64];
    char   v6pre[64];
}ipv6_addr_info_t;

typedef  enum
_COSA_DML_LINK_TYPE
{
    COSA_DML_LINK_TYPE_Eth       = 1,
    COSA_DML_LINK_TYPE_EthLink,
    COSA_DML_LINK_TYPE_EthVlan,
    COSA_DML_LINK_TYPE_Usb,
    COSA_DML_LINK_TYPE_Hpna,
    COSA_DML_LINK_TYPE_Moca,
    COSA_DML_LINK_TYPE_HomePlug,
    COSA_DML_LINK_TYPE_Upa,
    COSA_DML_LINK_TYPE_WiFiSsid,
    COSA_DML_LINK_TYPE_Bridge,
    COSA_DML_LINK_TYPE_PPP,
    COSA_DML_LINK_TYPE_DOCSIS,
    COSA_DML_LINK_TYPE_LAST
}
COSA_DML_LINK_TYPE, *PCOSA_DML_LINK_TYPE;

#define COSA_DML_LINK_TYPE_TOTAL                    COSA_DML_LINK_TYPE_LAST

typedef struct
LINKTYPE_MAP
{
    char*                           LinkTypePath;
    char*                           LinkTypeStr;
    COSA_DML_LINK_TYPE              LinkType;
}
LINKTYPE_MAP_T;
/** @} */  //END OF GROUP CM_AGENT_TYPES

/**
 * @addtogroup CM_AGENT_APIS
 * @{
 */

 /**
 * @brief This function is used to get IP address of the requested device.
 *
 * @param[in] netdev  Device name.
 *
 * @return  Returns IP address value.
 */
ULONG
CosaUtilGetIfAddr
    (
        char*       netdev
    );


PUCHAR
CosaUtilGetLowerLayers
    (
        PUCHAR                      pTableName,
        PUCHAR                      pKeyword
    );

/**
 * @brief This funcation serves for searching other pathname  except lowerlayer.
 *
 * @param[in] pTableName      Table names divided by ",". For example "Device.Ethernet.Interface., Device.Dhcpv4." .
 * @param[in] pParameterName  parameter name which hold the keyword. eg: "name".
 * @param[in] pKeyword        keyword. eg: "wan0"..
 *
 * @return  Returns result string.
 */
PUCHAR
CosaUtilGetFullPathNameByKeyword
    (
        PUCHAR                      pTableName,
        PUCHAR                      pParameterName,
        PUCHAR                      pKeyword
    );

/**
 * @brief This function is called to get interface status info like transmitted/received packet info, error info etc.
 *
 * @param[in] ifname   Interface name.
 * @param[out] pStats  Buffer to retrieve status info.
 *
 * @return  Returns TRUE once get the status, Otherwise return 0.
 */
int CosaUtilGetIfStats(char * ifname, PCOSA_DML_IF_STATS  pStats);

/**
 * @brief This function is used to trigger ioctl calls.
 *
 * @param[in] if_name    Interface name.
 * @param[in] method     Method to call.
 * @param[in] input      Required input userdata to set if any.
 *
 * @return  Returns status of the operation.
 */
ULONG CosaUtilIoctlXXX(char * if_name, char * method, void * input);

/**
 * @brief This function is used to get number of IP addressess in the subnet from the netmask IP address.
 *
 * @param[in] netmask    Netmask IP address.
 *
 * @return  Returns the number count.
 */
ULONG NetmaskToNumber(char *netmask);

/**
 * @brief This function is used to get static route table info.
 *
 * @param[out] count        Total Line count of route table info excluding first two lines.
 * @param[out] out_route    Static route table info.
 *
 * @return  Returns the status of the operation.
 */
ANSC_STATUS
CosaUtilGetStaticRouteTable
    (
        UINT                        *count,
        StaticRoute                 **out_route
    );

/**
 * @brief This function is used to get IPV6 address info.
 *
 * @param[in] ifname       Interface name.
 * @param[out] pp_info     IPV6 address info.
 * @param[out] num         Total count.
 *
 * @return  Returns the status of the operation.
 */
int CosaUtilGetIpv6AddrInfo (char * ifname, ipv6_addr_info_t ** pp_info, int * num);

/**
 * @brief This function is used to do string copy.
 *
 * @param[out] dst          Destination buffer to copy.
 * @param[in] src           Source buffer.
 * @param[in] dst_size      Destination buffer size.
 *
 * @return  Returns the status of the operation.
 */
int safe_strcpy(char * dst, char * src, int dst_size);
int  __v6addr_mismatch(char * addr1, char * addr2, int pref_len);
int  __v6addr_mismatches_v6pre(char * v6addr,char * v6pre);
int  __v6pref_mismatches(char * v6pref1,char * v6pref2);

/**
 * @brief This function is used to validate whether both the V6 IP address are equal or not.
 *
 * @param[in] p_addr1       IPV6 address 1.
 * @param[in] p_addr2       IPV6 address 2.
 *
 * @return  Returns the status of the operation.
 */
int CosaDmlV6AddrIsEqual(char * p_addr1, char * p_addr2);

/**
 * @brief This function is used to validate whether both the V6 prefix length are equal or not.
 *
 * @param[in] p_pref1       IPV6 prefix 1.
 * @param[in] p_pref2       IPV6 prefix 2.
 *
 * @return  Returns the status of the operation.
 */
int CosaDmlV6PrefIsEqual(char * p_pref1, char * p_pref2);

/**
 * @brief This function is used to write the requested value in given file.
 *
 * @param[in] fn       File name.
 * @param[in] val      Data to write into file.
 *
 * @return  Returns the status of the operation.
 */
int _write_sysctl_file(char * fn, int val);

/*utility functions for linktype and lowerlayer*/

/**
 * @brief This function is used to get link path of bridge link type(COSA_DML_LINK_TYPE_Eth/COSA_DML_LINK_TYPE_DOCSIS etc).
 *
 * @param[in] LinkType       DML link type.
 *
 * @return  Returns the link path on success case, Otherwise returns NULL.
 */
char*               CosaUtilGetLinkTypePath(COSA_DML_LINK_TYPE LinkType);

/**
 * @brief This function is used to get link type(COSA_DML_LINK_TYPE_Eth/COSA_DML_LINK_TYPE_DOCSIS etc) string from Link path.
 *
 * @param[in] pLinkTypePath       Link Path.
 *
 * @return  Returns the string format of link type on success case, Otherwise returns NULL.
 */
char*               CosaUtilGetStrFromLinkTypePath(char* pLinkTypePath);

/**
 * @brief This function is used to get link type(COSA_DML_LINK_TYPE_Eth/COSA_DML_LINK_TYPE_DOCSIS etc) string from Link type.
 *
 * @param[in] LinkType       Link type.
 *
 * @return  Returns the string format of link type on success case, Otherwise returns NULL.
 */
char*               CosaUtilGetLinkTypeStr(COSA_DML_LINK_TYPE LinkType);

/**
 * @brief This function is used to get link type(COSA_DML_LINK_TYPE_Eth/COSA_DML_LINK_TYPE_DOCSIS etc) from string path.
 *
 * @param[in] pLinkTypeStr       Link type string path.
 *
 * @return  Returns the string format of link type on success case, Otherwise returns NULL.
 */
COSA_DML_LINK_TYPE  CosaUtilGetLinkTypeFromStr(char* pLinkTypeStr);

/**
 * @brief This function is used to get link type(COSA_DML_LINK_TYPE_Eth/COSA_DML_LINK_TYPE_DOCSIS etc) from the path.
 *
 * @param[in] pLinkTypePath       Link path.
 *
 * @return  Returns the link type.
 */
COSA_DML_LINK_TYPE  CosaUtilGetLinkTypeFromPath(char*pLinkTypePath);

// utility functions for getting Bridge information
/**
 * @brief This function is used to Find the management port in Bridge and return name from that port.
 *
 * @param[in] pBridgePath       Bridge path.
 *
 * @return  Returns the name from the management port.
 */
PUCHAR CosaUtilFindBridgeName(char* pBridgePath);

/**
 * @brief This function is used to Find the Bridge path.
 *
 * @param[in] pBridgeName       Bridge name.
 *
 * @return  Returns the bridge path.
 */
PUCHAR CosaUtilFindBridgePath(char* pBridgeName);

/** @} */  //END OF GROUP CM_AGENT_APIS

/**
 * @addtogroup CM_AGENT_TYPES
 * @{
 */
#define BRIDGE_MODE_JUDGEMENT_IFTRUE_RETURNFALSE                                \
    {                                                                           \
        BOOL bridgeMode;                                                        \
        if ((ANSC_STATUS_SUCCESS == is_usg_in_bridge_mode(&bridgeMode)) &&      \
            (TRUE == bridgeMode))                                               \
            return FALSE;                                                       \
    }                                                                           \

/*
 * IPV4 Address check functions
 */
/* __NOTE__ all addresses here are in the host byte order */
#define IPV4_CLASSA(a)        ((((uint32_t)(a)) & 0x80000000UL) == 0) 
#define IPV4_CLASSA_NET       0xff000000 
#define IPV4_CLASSA_NSHIFT    24 
#define IPV4_CLASSA_HOST      (0xffffffff & ~IP_CLASSA_NET) 
#define IPV4_CLASSA_MAX       128 
 
#define IPV4_CLASSB(a)        ((((uint32_t)(a)) & 0xc0000000UL) == 0x80000000UL) 
#define IPV4_CLASSB_NET       0xffff0000 
#define IPV4_CLASSB_NSHIFT    16 
#define IPV4_CLASSB_HOST      (0xffffffff & ~IP_CLASSB_NET) 
#define IPV4_CLASSB_MAX       65536 
 
#define IPV4_CLASSC(a)        ((((uint32_t)(a)) & 0xe0000000UL) == 0xc0000000UL) 
#define IPV4_CLASSC_NET       0xffffff00 
#define IPV4_CLASSC_NSHIFT    8 
#define IPV4_CLASSC_HOST      (0xffffffff & ~IP_CLASSC_NET) 
 
#define IPV4_CLASSD(a)        (((uint32_t)(a) & 0xf0000000UL) == 0xe0000000UL) 
#define IPV4_CLASSD_NET       0xf0000000
#define IPV4_CLASSD_NSHIFT    28
#define IPV4_CLASSD_HOST      0x0fffffff
#define IPV4_MULTICAST(a)     IP_CLASSD(a) 
 
#define IPV4_EXPERIMENTAL(a)  (((uint32_t)(a) & 0xf0000000UL) == 0xf0000000UL) 
#define IPV4_BADCLASS(a)      (((uint32_t)(a) & 0xf0000000UL) == 0xf0000000UL) 
/** @} */  //END OF GROUP CM_AGENT_TYPES

/**
 * @addtogroup CM_AGENT_APIS
 * @{
 */
/* __NOTE__ all addresses here are in the network byte order */
/**
 * @brief This function is used to find whether both the IP address are in same sub network or not.
 *
 * @param[in] addr1       IP address 1.
 * @param[in] addr2       IP address 2.
 * @param[in] mask        Network mask.
 *
 * @return  Returns the status of the operation.
 */
int IPv4Addr_IsSameNetwork(uint32_t addr1, uint32_t addr2, uint32_t mask);

/**
 * @brief This function is used to find the loopback address.
 *
 * @param[in] addr       IPV4 address.
 *
 * @return  Returns the status of the operation.
 */
int IPv4Addr_IsLoopback(uint32_t addr);

/**
 * @brief This function is used to find the multicast address.
 *
 * @param[in] addr       IPV4 address.
 *
 * @return  Returns the status of the operation.
 */
int IPv4Addr_IsMulticast(uint32_t addr);

/**
 * @brief This function is used to find the broadcast address.
 *
 * @param[in] addr       IPV4 address.
 * @param[in] net        Network.
 * @param[in] mask       Network mask.
 *
 * @return  Returns the status of the operation.
 */
int IPv4Addr_IsBroadcast(uint32_t addr, uint32_t net, uint32_t mask);

/**
 * @brief This function is used to ensure the network address.
 *
 * @param[in] addr       IPV4 address.
 * @param[in] net        Network.
 * @param[in] mask       Network mask.
 *
 * @return  Returns the status of the operation.
 */
int IPv4Addr_IsNetworkAddr(uint32_t addr, uint32_t net, uint32_t mask);

/**
 * @brief This function is used to find whether netmask is valid or not.
 *
 * @param[in] netmask       Net mask.
 *
 * @return  Returns the status of the operation.
 */
int IPv4Addr_IsNetmaskValid(uint32_t netmask);

/**
 * @brief This function is used to determine whether the IP address belongs to CLASS A.
 *
 * @param[in] addr       IPV4 address.
 *
 * @return  Returns the status of the operation.
 */
int IPv4Addr_IsClassA(uint32_t addr);

/**
 * @brief This function is used to determine whether the IP address belongs to CLASS B.
 *
 * @param[in] addr       IPV4 address.
 *
 * @return  Returns the status of the operation.
 */
int IPv4Addr_IsClassB(uint32_t addr);

/**
 * @brief This function is used to determine whether the IP address belongs to CLASS C.
 *
 * @param[in] addr       IPV4 address.
 *
 * @return  Returns the status of the operation.
 */
int IPv4Addr_IsClassC(uint32_t addr);

/**
 * @brief This function is used to determine whether the IP address belongs to CLASS D.
 *
 * @param[in] addr       IPV4 address.
 *
 * @return  Returns the status of the operation.
 */
int IPv4Addr_IsClassD(uint32_t addr);

/**
 * @brief This function is used to dispatch system command.
 *
 * @return  Returns the status of the operation.
 */
int vsystem(const char *fmt, ...);

void chomp(char *line);

/**
 * @brief This function is used to retrieve hardware address.
 *
 * @param[in] ifname      Interface name.
 * @param[out] mac        MAC address.
 * @param[out] size       Size of MAC address.
 *
 * @return  Returns the status of the operation.
 */
int get_if_hwaddr(const char *ifname, char *mac, size_t size);

/** @} */  //END OF GROUP CM_AGENT_APIS
#endif
