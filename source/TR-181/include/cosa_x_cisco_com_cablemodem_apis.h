/*
 * If not stated otherwise in this file or this component's LICENSE file the
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

/**************************************************************************

    module: cosa_deviceinfo_apis.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.

**************************************************************************/


#ifndef  _COSA_CM_APIS_H
#define  _COSA_CM_APIS_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"
/**********************************************************************
                STRUCTURE AND CONSTANT DEFINITIONS
**********************************************************************/
#if defined (FEATURE_RDKB_WAN_MANAGER)
#define WAN_DBUS_PATH                     "/com/cisco/spvtg/ccsp/wanmanager"
#define WAN_COMPONENT_NAME                "eRT.com.cisco.spvtg.ccsp.wanmanager"
#define WAN_PHY_STATUS_PARAM_NAME         "Device.X_RDK_WanManager.CPEInterface.%d.Phy.Status"
#define WAN_OPER_STATUS_PARAM_NAME        "Device.X_RDK_WanManager.CPEInterface.%d.Wan.OperationalStatus"
#define WAN_CM_INTERFACE_INSTANCE_NUM      1
#endif
/**
 * @addtogroup CM_AGENT_TYPES
 * @{
 */

/*
   Warning: The COSA_CM_DHCP_INFO struct must exactly match CMMGMT_CM_DHCP_INFO in halinterface/cm_hal.h
*/
typedef  struct
_COSA_CM_DHCP_INFO
{
    ANSC_IPV4_ADDRESS               IPAddress;
    char                            BootFileName[64];
    ANSC_IPV4_ADDRESS               SubnetMask;
    ANSC_IPV4_ADDRESS               Gateway;
    ANSC_IPV4_ADDRESS               TFTPServer;
    char                            TimeServer[64];
    INT                             TimeOffset;
    ULONG                           LeaseTimeRemaining;
    char                            RebindTimeRemaining[64];
    char                            RenewTimeRemaining[64];
    char                            MACAddress[64];
    char                            DOCSISDHCPStatus[64];
}
COSA_CM_DHCP_INFO, *PCOSA_CM_DHCP_INFO;

typedef  struct
_COSA_CM_DS_CHANNEL
{
    ULONG                           ChannelID;
    char                            Frequency[64];
    char                            PowerLevel[64];
    char                            SNRLevel[64];
    char                            Modulation[64];
    ULONG                           Octets;
    ULONG                           Correcteds;
    ULONG                           Uncorrectables;
    char                            LockStatus[64];
}
COSA_CM_DS_CHANNEL, *PCOSA_CM_DS_CHANNEL;

typedef  struct
_COSA_CM_US_CHANNEL
{
    ULONG                           ChannelID;
    char                            Frequency[64];
    char                            PowerLevel[64];
    char                            ChannelType[64];
    char                            SymbolRate[64];
    char                            Modulation[64];
    char                            LockStatus[64];
}
COSA_CM_US_CHANNEL, *PCOSA_CM_US_CHANNEL;

typedef  struct
_COSA_CM_DOCSIS_INFO
{
    char                            DOCSISVersion[64];
    char                            DOCSISDownstreamScanning[64];
    char                            DOCSISDownstreamRanging[64];
    char                            DOCSISUpstreamScanning[64];
    char                            DOCSISUpstreamRanging[64];
    char                            DOCSISTftpStatus[64];
    char                            DOCSISDataRegComplete[64];
    ULONG                           DOCSISDHCPAttempts;
    char                            DOCSISConfigFileName[64];
    ULONG                           DOCSISTftpAttempts;
    char                            ToDStatus[64];
    BOOLEAN                         BPIState;
    BOOLEAN                         NetworkAccess;
    ANSC_IPV4_ADDRESS               UpgradeServerIP;
    ULONG                           MaxCpeAllowed;
    char                            UpstreamServiceFlowParams[64];
    char                            DownstreamServiceFlowParams[64];
    char                            DOCSISDownstreamDataRate[64];
    char                            DOCSISUpstreamDataRate[64];
    char                            CoreVersion[64];
}
COSA_CM_DOCSIS_INFO, *PCOSA_CM_DOCSIS_INFO;

typedef  struct
_COSA_CM_IPV6DHCP_INFO
{
    char                            IPv6Address[40];
    char                            IPv6BootFileName[64];
    char                            IPv6Prefix[40];
    char                            IPv6Router[40];
    char                            IPv6TFTPServer[40];
    char                            IPv6TimeServer[40];

    ULONG                           IPv6LeaseTimeRemaining;
    ULONG                           IPv6RebindTimeRemaining;
    ULONG                           IPv6RenewTimeRemaining;
}
COSA_CM_IPV6DHCP_INFO, *PCOSA_CM_IPV6DHCP_INFO;

typedef  struct
_COSA_DML_CM_LOG
{
    BOOLEAN                         EnableLog;
    BOOLEAN                         CleanDocsisLog;
}
COSA_DML_CM_LOG,  *PCOSA_DML_CM_LOG;

typedef  struct
_COSA_DML_CM_WANCFG
{
    char                            wanInstanceNumber[4];
    BOOLEAN                         MonitorPhyStatusAndNotify;
    BOOLEAN                         MonitorOperStatusAndNotify;
    BOOLEAN                         ConfigureWan;
    BOOLEAN                         CustomWanConfigUpdate;
}
COSA_DML_CM_WANCFG,  *PCOSA_DML_CM_WANCFG;

typedef  struct
_COSA_DML_CMERRORCODEWORDS_FULL
{
    ULONG                           UnerroredCodewords;
    ULONG                           CorrectableCodewords;
    ULONG                           UncorrectableCodewords;
}
COSA_DML_CMERRORCODEWORDS_FULL,  *PCOSA_DML_CMERRORCODEWORDS_FULL;

typedef  struct
_COSA_DML_DOCSISLOG_FULL
{
    ULONG                           Index;
    ULONG                           EventID;
    ULONG                           EventLevel;
    CHAR                            Time[64];
    CHAR                            Description[256];
}
COSA_DML_DOCSISLOG_FULL,  *PCOSA_DML_DOCSISLOG_FULL;

typedef  struct
_COSA_DML_CPE_LIST
{
    CHAR                            IPAddress[32];
    CHAR                            MACAddress[32];
}
COSA_DML_CPE_LIST,  *PCOSA_DML_CPE_LIST;

typedef enum _CM_RESET_TYPE
{
	CABLE_MODEM_RESET = 1,
	LOCAL_RESET,
	DOCSIS_RESET,
	EROUTER_RESET
}CM_RESET_TYPE;

/** @} */  //END OF GROUP CM_AGENT_TYPES

/**********************************************************************
                FUNCTION PROTOTYPES
**********************************************************************/

/**
 * @addtogroup CM_AGENT_APIS
 * @{
 */

/**
 * @brief This function is used to Initialize middle layer for cosa device info object Device.DeviceInfo.
 *
 * @param[in] hDml       Data Model Layer Handle.
 * @param[in] phContext  Data Model CM Object.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on successfully initialize middle layer for cosa device.
 */
ANSC_STATUS
CosaDmlCMInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

/**
 * @brief This function is used to retrieve Cable Modem DOCSIS status.
 *
 * @param[in] hContext     Data Model CM Object.
 * @param[out] pValue      Output buffer to get status.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on success case, Otherwise returns ANSC_STATUS_FAILURE.
 */
ANSC_STATUS
CosaDmlCMGetStatus
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

/**
 * @brief This function is used to get loop diagnosticsstart.
 *
 * @param[in] hContext    	Data Model CM Object.
 * @param[out] pBool        Output buffer to get start loop.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on success case, Otherwise returns ANSC_STATUS_FAILURE.
 */
ANSC_STATUS
CosaDmlCMGetLoopDiagnosticsStart
    (
        ANSC_HANDLE                 hContext,
        BOOL*                       pBool
    );

/**
 * @brief This function is used to set loop diagnosticsstart.
 *
 * @param[in] hContext     Data Model CM Object.
 * @param[in] pBool        boolean value to set loop start.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on success case, Otherwise returns ANSC_STATUS_FAILURE.
 */
ANSC_STATUS
CosaDmlCMSetLoopDiagnosticsStart
    (
        ANSC_HANDLE                 hContext,
        BOOL                        pBool
    );

/**
 * @brief This function is used to get loop diagnostics details.
 *
 * @param[in] hContext     Data Model CM Object.
 * @param[out] pValue      Output buffer to collect details.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on success case, Otherwise returns ANSC_STATUS_FAILURE.
 */
ANSC_STATUS
CosaDmlCMGetLoopDiagnosticsDetails
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

/**
 * @brief This function is used to get telephony DHCP status.
 *
 * @param[in] hContext     Data Model CM Object.
 * @param[out] pValue      Output buffer to collect status.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on success case, Otherwise returns ANSC_STATUS_FAILURE.
 */
ANSC_STATUS
CosaDmlCMGetTelephonyDHCPStatus
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

/**
 * @brief This function is used to get telephony TFTP status.
 *
 * @param[in] hContext     Data Model CM Object.
 * @param[out] pValue      Output buffer to collect status.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on success case, Otherwise returns ANSC_STATUS_FAILURE.
 */
ANSC_STATUS
CosaDmlCMGetTelephonyTftpStatus
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

/**
 * @brief This function is used to get telephony registration status.
 *
 * @param[in] hContext     Data Model CM Object.
 * @param[out] pValue      Output buffer to collect registration status.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on success case, Otherwise returns ANSC_STATUS_FAILURE.
 */
ANSC_STATUS
CosaDmlCMGetTelephonyRegistrationStatus
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

/**
 * @brief This function is used to retrieve all the DHCP info from CM hal.
 *
 * @param[in] hContext     Data Model CM Object.
 * @param[out] pInfo       Output buffer to get DHCP info.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on success case, Otherwise returns ANSC_STATUS_FAILURE.
 */
ANSC_STATUS
CosaDmlCMGetDHCPInfo
    (
        ANSC_HANDLE                 hContext,
        PCOSA_CM_DHCP_INFO          pInfo
    );

/**
 * @brief This function is used to retrieve DownStream channel info.
 *
 * @param[in] hContext     Data Model CM Object.
 * @param[out] pInfo       Output buffer to get DS channel info.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on success case, Otherwise returns ANSC_STATUS_FAILURE.
 */
ANSC_STATUS
CosaDmlCMGetDSChannel
    (
        ANSC_HANDLE                 hContext,
        PCOSA_CM_DS_CHANNEL         pInfo
    );

/**
 * @brief This function is used to retrieve UpStream channel info.
 *
 * @param[in] hContext     Data Model CM Object.
 * @param[out] pInfo       Output buffer to get US channel info.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on success case, Otherwise returns ANSC_STATUS_FAILURE.
 */
ANSC_STATUS
CosaDmlCMGetUSChannel
    (
        ANSC_HANDLE                 hContext,
        PCOSA_CM_US_CHANNEL         pInfo
    );

/**
 * @brief This function is used to get DOCSIS registration status.
 *
 * @param[in] hContext     Data Model CM Object.
 * @param[out] pInfo       Output buffer to get docsis info.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on success case, Otherwise returns ANSC_STATUS_FAILURE.
 */
ANSC_STATUS
CosaDmlCMGetDOCSISInfo
    (
        ANSC_HANDLE                 hContext,
        PCOSA_CM_DOCSIS_INFO        pInfo
    );

/**
 * @brief This function is used to retrieve log information.
 *
 * @param[in] hContext    Data Model CM Object.
 * @param[out] pCfg       Log info.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on success case, Otherwise returns ANSC_STATUS_FAILURE.
 */
ANSC_STATUS
CosaDmlCmGetLog
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_CM_LOG           pCfg
    );

/**
 * @brief This function is used to Clear the DOCSIS event log.
 *
 * @param[in] hContext    Data Model CM Object.
 * @param[in] pCfg       Log handling configuration.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on success case, Otherwise returns ANSC_STATUS_FAILURE.
 */
ANSC_STATUS
CosaDmlCmSetLog
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_CM_LOG            pCfg
    );

/**
 * @brief This function is used to get Docsis log information.
 *
 * @param[in] hContext    Data Model CM Object.
 * @param[out] pulCount   Docsis log number.
 * @param[out] ppCfg      Docsis log.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on success case, Otherwise returns ANSC_STATUS_FAILURE.
 */
ANSC_STATUS
CosaDmlCmGetDocsisLog
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
        PCOSA_DML_DOCSISLOG_FULL    *ppCfg
    );

/**
 * @brief This function is used to retrieve downstream channel details.
 *
 * @param[in] hContext    Data Model CM Object.
 * @param[out] pulCount   Number of active receiving channel.
 * @param[out] ppConf     DS Channel info.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on success case, Otherwise returns ANSC_STATUS_FAILURE.
 */
ANSC_STATUS
CosaDmlCmGetDownstreamChannel
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
        PCOSA_CM_DS_CHANNEL         *ppConf        
    );    

/**
 * @brief This function is used to retrieve upstream channel details.
 *
 * @param[in] hContext    Data Model CM Object.
 * @param[out] pulCount   Number of active trasmitting channel.
 * @param[out] ppConf     US Channel info.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on success case, Otherwise returns ANSC_STATUS_FAILURE.
 */
ANSC_STATUS
CosaDmlCmGetUpstreamChannel
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
        PCOSA_CM_US_CHANNEL         *ppConf        
    );    

/**
 * @brief This function is used to retrieve the market of the modem.
 *
 * @param[in] hContext    Data Model CM Object.
 * @param[out] pValue     Name of the market for the modem, "US" or "EURO", to be returned.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on success case, Otherwise returns ANSC_STATUS_FAILURE.
 */
ANSC_STATUS
CosaDmlCMGetMarket
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

/**
 * @brief This function is used to retrieve retrieve the current MIMO status.
 *
 * @param[in] hContext    Data Model CM Object.
 * @param[out] pValue     Received status.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on success case, Otherwise returns ANSC_STATUS_FAILURE.
 */
ANSC_STATUS
CosaDmlCMGetMDDIPOverride
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

/**
 * @brief This function is used to set the current global MIMO status.
 *
 * @param[in] hContext    Data Model CM Object.
 * @param[in] pValue      Status to be set to.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on success case, Otherwise returns ANSC_STATUS_FAILURE.
 */
ANSC_STATUS
CosaDmlCMSetMDDIPOverride
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

/**
 * @brief This function is used to retrieve error code words for all the active receiving channels.
 *
 * @param[in] hContext    Data Model CM Object.
 * @param[out] pulCount   Number of active channels.
 * @param[out] ppConf     Error code words.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on success case, Otherwise returns ANSC_STATUS_FAILURE.
 */
ANSC_STATUS
CosaDmlCmGetCMErrorCodewords
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
        PCOSA_DML_CMERRORCODEWORDS_FULL   *ppConf        
    );

/**
 * @brief This function is used to retrieve all the relevant IPv6 DHCP info for the CM.
 *
 * @param[in] hContext    Data Model CM Object.
 * @param[out] pInfo      All IPv6 DHCP info for CM, to be returned.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on success case, Otherwise returns ANSC_STATUS_FAILURE.
 */
ANSC_STATUS
CosaDmlCMGetIPv6DHCPInfo
    (
        ANSC_HANDLE                 hContext,
        PCOSA_CM_IPV6DHCP_INFO      pInfo
    );

/**
 * @brief This function is used to retrieve the US channel ID in its MAC domain.
 *
 * @param[in] hContext    Data Model CM Object.
 * @param[out] pValue     Channel ID.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on success case, Otherwise returns ANSC_STATUS_FAILURE.
 */
ANSC_STATUS
CosaDmlCMGetLockedUpstreamChID
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pValue
    );

/**
 * @brief This function is used to set the US channel ID in its MAC domain.
 *
 * @param[in] hContext    Data Model CM Object.
 * @param[in] value       Index to set the UpStream Channel ID.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on success case, Otherwise returns ANSC_STATUS_FAILURE.
 */
ANSC_STATUS
CosaDmlCMSetLockedUpstreamChID
    (
        ANSC_HANDLE                 hContext,
        ULONG                      value
    );

/**
 * @brief This function is used to retrieve the current primary channel DS channel frequency.
 *
 * @param[in] hContext    Data Model CM Object.
 * @param[out] pValue     Channel Frequency.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on success case, Otherwise returns ANSC_STATUS_FAILURE.
 */
ANSC_STATUS
CosaDmlCMGetStartDSFrequency
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pValue
    );

/**
 * @brief This function is used to change the DS primary channel frequency.
 *
 * @param[in] hContext    Data Model CM Object.
 * @param[in] value       Channel Frequency.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on success case, Otherwise returns ANSC_STATUS_FAILURE.
 */
ANSC_STATUS
CosaDmlCMSetStartDSFrequency
    (
        ANSC_HANDLE                 hContext,
        ULONG                      value
    );
	
/**
 * @brief This function is used to retrieve the location of the certificate.
 *
 * @param[in] hContext    Data Model CM Object.
 * @param[out] pValue      Location of the certificate.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on success case, Otherwise returns ANSC_STATUS_FAILURE.
 */
ANSC_STATUS
CosaDmlCmGetCMCert
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

/**
 * @brief This function is used to retrieve the provisioned wan0 IP type.
 *
 * @param[in] hContext    Data Model CM Object.
 * @param[out] pValue     Ip type currently provisioned on wan0(IPv4/IPv6/unknown).
 *
 * @return  Returns ANSC_STATUS_SUCCESS on success case, Otherwise returns ANSC_STATUS_FAILURE.
 */
ANSC_STATUS
CosaDmlCMGetProvType
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

/**
 * @brief This function is used to retrieve the count of cable modem reset.
 *
 * @param[in] hContext    Data Model CM Object.
 * @param[in] type        CM Reset type.
 * @param[out] pValue     Reset count.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on success case, Otherwise returns ANSC_STATUS_FAILURE.
 */
ANSC_STATUS
CosaDmlCMGetResetCount
    (
        ANSC_HANDLE                 hContext,
		CM_RESET_TYPE         		type,
        ULONG                       *pValue
    );

int Ccsp_cm_clnt_lock(void);
int Ccsp_cm_clnt_unlock(void);

ANSC_STATUS
CosaDmlCmGetCPEList
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulInstanceNumber,
        PCOSA_DML_CPE_LIST          *ppCPEList
    );

ANSC_STATUS
CosaDmlCmGetCMCertStatus
    (
        ANSC_HANDLE                 hContext,
        BOOL*                       pBool
    );

#if defined (FEATURE_RDKB_WAN_MANAGER)
ANSC_STATUS
CosaDmlCMWanUpdateCustomConfig
    (
        void* arg,
        BOOL             bValue
    );

ANSC_STATUS CosaDmlCMWanMonitorPhyStatusAndNotify(void* arg);
ANSC_STATUS CosaDmlCMWanMonitorOperStatusAndNotify(void* arg);
ANSC_STATUS CosaDmlCMUpdateInformMsgToWanMgr(void *arg);
#endif

/** @} */  //END OF GROUP CM_AGENT_APIS
#endif
