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

    module: cosa_x_rdkcentral_com_cablemodem_apis.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        08/09/2017    initial revision.

**************************************************************************/


#ifndef  _COSA_RDKCENTRAL_CM_APIS_H
#define  _COSA_RDKCENTRAL_CM_APIS_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"

/**********************************************************************
                STRUCTURE AND CONSTANT DEFINITIONS
**********************************************************************/

/**
 * @addtogroup CM_AGENT_TYPES
 * @{
 */

typedef struct COSA_X_RDKCENTRAL_COM_CM_DS_OFDM_CHAN {
    unsigned int ChannelId;                    
    unsigned int ChanIndicator;                
    unsigned int SubcarrierZeroFreq;           
    unsigned int FirstActiveSubcarrierNum;     
    unsigned int LastActiveSubcarrierNum;      
    unsigned int NumActiveSubcarriers;         
    unsigned int SubcarrierSpacing;            
	unsigned int CyclicPrefix;   
	unsigned int RollOffPeriod;  
    unsigned int PlcFreq;        
    unsigned int NumPilots;      
    unsigned int TimeInterleaverDepth;
    char          averageSNR[64];                    //The averageSNR value of this downstream channel
    char          PowerLevel[64];                           //The power level of this downstream channel
    unsigned long long PlcTotalCodewords;
    unsigned long long PlcUnreliableCodewords;
    unsigned long long NcpTotalFields; 
    unsigned long long NcpFieldCrcFailures;
} COSA_X_RDKCENTRAL_COM_CM_DS_OFDM_CHAN, *PCOSA_X_RDKCENTRAL_COM_CM_DS_OFDM_CHAN;

typedef struct COSA_X_RDKCENTRAL_COM_CM_US_OFDMA_CHAN {
    unsigned int ChannelId;                
    unsigned int ConfigChangeCt;           
    unsigned int SubcarrierZeroFreq;       
    unsigned int FirstActiveSubcarrierNum; 
    unsigned int LastActiveSubcarrierNum;  
    unsigned int NumActiveSubcarriers;     
    unsigned int SubcarrierSpacing;        
    unsigned int CyclicPrefix;             
    unsigned int RollOffPeriod;            
    unsigned int NumSymbolsPerFrame;       
    unsigned int TxPower;                  
    BOOL		 PreEqEnabled;                     

 } COSA_X_RDKCENTRAL_COM_CM_US_OFDMA_CHAN, *PCOSA_X_RDKCENTRAL_COM_CM_US_OFDMA_CHAN;

typedef struct COSA_X_RDKCENTRAL_COM_CMSTATUSOFDMA_US {
	unsigned int ChannelId;             
	unsigned int T3Timeouts;            
    unsigned int T4Timeouts;            
    unsigned int RangingAborteds;       
    unsigned int T3Exceededs;           
    BOOL 		 IsMuted;                       
    unsigned int RangingStatus;         
} COSA_X_RDKCENTRAL_COM_CMSTATUSOFDMA_US, *PCOSA_X_RDKCENTRAL_COM_CMSTATUSOFDMA_US;

/** @} */  //END OF GROUP CM_AGENT_TYPES

/**********************************************************************
                FUNCTION PROTOTYPES
**********************************************************************/
/**
 * @addtogroup CM_AGENT_APIS
 * @{
 */

/**
 * @brief This function is used to Initialize middle layer for Device.X_RDKCENTRAL-COM_CableModem.
 * Device.X_RDKCENTRAL-COM_CableModem is a CMAgent object in its DML layer.
 *
 * @param[in] hDml       Handle of Data Model Layer.
 * @param[out] phContext  DM Object.
 */
ANSC_STATUS
CosaDmlRDKCentralCMInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

/**
 * @brief This function is used to retrive DownStream channel tables information from CM HAL API.
 *
 * @param[in] hContext         Handle of Data Model Layer.
 * @param[in] pulCount         Total number of DS channel entries.
 * @param[in] ppDsOfdmChannel  Downstream channel information.
 *
 * @return  Returns ANSC_STATUS_SUCCESS when successfully gets the DS channel info, Otherwise returns ANSC_STATUS_FAILURE.
 */
ANSC_STATUS
CosaDmlRDKCentralCmGetDownstreamChannel
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
		PCOSA_X_RDKCENTRAL_COM_CM_DS_OFDM_CHAN *ppDsOfdmChannel 	   
    );

/**
 * @brief This function is used to retrive Upstream channel tables information from CM HAL API.
 *
 * @param[in] hContext         Handle of Data Model Layer.
 * @param[in] pulCount         Total number of US channel entries.
 * @param[in] ppUsOfdmChannel  Upstream channel information.
 *
 * @return  Returns ANSC_STATUS_SUCCESS when successfully gets the US channel info, Otherwise returns ANSC_STATUS_FAILURE.
 */
ANSC_STATUS
CosaDmlRDKCentralCmGetUpstreamChannel
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
		PCOSA_X_RDKCENTRAL_COM_CM_US_OFDMA_CHAN *ppUsOfdmChannel 	   
    );

/**
 * @brief This function is used to get status of Upstream channel table information from CM HAL API.
 *
 * @param[in] hContext                 Handle of Data Model Layer.
 * @param[in] pulCount                 Total number of US channel entries.
 * @param[in] ppCMStatusofUsChannel    Buffer to receive status of Upstream channel information.
 *
 * @return  Returns ANSC_STATUS_SUCCESS when successfully gets the status of US channel info, Otherwise returns ANSC_STATUS_FAILURE.
 */
ANSC_STATUS
CosaDmlRDKCentralCmGetCMStatusofUpstreamChannel
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
		PCOSA_X_RDKCENTRAL_COM_CMSTATUSOFDMA_US *ppCMStatusofUsChannel 	   
    );

/** @} */  //END OF GROUP CM_AGENT_APIS
#endif /* _COSA_RDKCENTRAL_CM_APIS_H */
