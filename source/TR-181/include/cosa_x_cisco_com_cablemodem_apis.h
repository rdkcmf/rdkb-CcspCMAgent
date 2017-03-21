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

/**************************************************************************

    module: cosa_deviceinfo_apis.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    copyright:

        Cisco Systems, Inc.
        All Rights Reserved.

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

#define MAX_FREQ_SZ       64
#define MAX_LOCKSTATUS_SZ 64
#define MAX_MOD_OFDM_SZ   64
#define MAX_RXPOWER_SZ    16

typedef  struct
_COSA_CM_OFDM_DS_CHANNEL
{
   unsigned int dcid;
   char OFDMFrequency[MAX_FREQ_SZ];
   char tunerFrequency[MAX_FREQ_SZ];
   char plcCenterFrequency[MAX_FREQ_SZ];
   char LockStatus[MAX_LOCKSTATUS_SZ];
   char Modulation[MAX_MOD_OFDM_SZ];
   unsigned int timeInterleavingDepth;
   int fftType;
   int cpSize;
   int numOfDataSubcarr;
   unsigned int rollOff;
   char rxPower[MAX_RXPOWER_SZ];
}
COSA_CM_OFDM_DS_CHANNEL, *PCOSA_CM_OFDM_DS_CHANNEL;

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

/**********************************************************************
                FUNCTION PROTOTYPES
**********************************************************************/

ANSC_STATUS
CosaDmlCMInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

ANSC_STATUS
CosaDmlCMGetStatus
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

ANSC_STATUS
CosaDmlCMGetLoopDiagnosticsStart
    (
        ANSC_HANDLE                 hContext,
        BOOL*                       pBool
    );

ANSC_STATUS
CosaDmlCMSetLoopDiagnosticsStart
    (
        ANSC_HANDLE                 hContext,
        BOOL                        pBool
    );

ANSC_STATUS
CosaDmlCMGetLoopDiagnosticsDetails
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

ANSC_STATUS
CosaDmlCMGetTelephonyDHCPStatus
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

ANSC_STATUS
CosaDmlCMGetTelephonyTftpStatus
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

ANSC_STATUS
CosaDmlCMGetTelephonyRegistrationStatus
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

ANSC_STATUS
CosaDmlCMGetDHCPInfo
    (
        ANSC_HANDLE                 hContext,
        PCOSA_CM_DHCP_INFO          pInfo
    );

ANSC_STATUS
CosaDmlCMGetDSChannel
    (
        ANSC_HANDLE                 hContext,
        PCOSA_CM_DS_CHANNEL         pInfo
    );

ANSC_STATUS
CosaDmlCMGetUSChannel
    (
        ANSC_HANDLE                 hContext,
        PCOSA_CM_US_CHANNEL         pInfo
    );

ANSC_STATUS
CosaDmlCMGetDOCSISInfo
    (
        ANSC_HANDLE                 hContext,
        PCOSA_CM_DOCSIS_INFO        pInfo
    );

ANSC_STATUS
CosaDmlCmGetLog
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_CM_LOG           pCfg
    );

ANSC_STATUS
CosaDmlCmSetLog
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_CM_LOG            pCfg
    );

ANSC_STATUS
CosaDmlCmGetDocsisLog
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
        PCOSA_DML_DOCSISLOG_FULL    *ppCfg
    );

ANSC_STATUS
CosaDmlCmGetDownstreamChannel
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
        PCOSA_CM_DS_CHANNEL         *ppConf        
    );    

#if defined (_XB6_PRODUCT_REQ_)
ANSC_STATUS
CosaDmlCmGetDownstreamOFDMChannel
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
        PCOSA_CM_OFDM_DS_CHANNEL    *ppConf
    );
#endif

ANSC_STATUS
CosaDmlCmGetUpstreamChannel
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
        PCOSA_CM_US_CHANNEL         *ppConf        
    );    

ANSC_STATUS
CosaDmlCMGetMarket
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

ANSC_STATUS
CosaDmlCMGetMDDIPOverride
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

ANSC_STATUS
CosaDmlCMSetMDDIPOverride
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

ANSC_STATUS
CosaDmlCmGetCMErrorCodewords
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
        PCOSA_DML_CMERRORCODEWORDS_FULL   *ppConf        
    );

ANSC_STATUS
CosaDmlCMGetIPv6DHCPInfo
    (
        ANSC_HANDLE                 hContext,
        PCOSA_CM_IPV6DHCP_INFO      pInfo
    );


ANSC_STATUS
CosaDmlCMGetLockedUpstreamChID
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pValue
    );

ANSC_STATUS
CosaDmlCMSetLockedUpstreamChID
    (
        ANSC_HANDLE                 hContext,
        ULONG                      value
    );

ANSC_STATUS
CosaDmlCMGetStartDSFrequency
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pValue
    );

ANSC_STATUS
CosaDmlCMSetStartDSFrequency
    (
        ANSC_HANDLE                 hContext,
        ULONG                      value
    );
	
ANSC_STATUS
CosaDmlCmGetCMCert
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

ANSC_STATUS
CosaDmlCMGetProvType
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    );

ANSC_STATUS
CosaDmlCMGetResetCount
    (
        ANSC_HANDLE                 hContext,
		CM_RESET_TYPE         		type,
        ULONG                       *pValue
    );
	

#endif
