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


/**********************************************************************
                FUNCTION PROTOTYPES
**********************************************************************/

ANSC_STATUS
CosaDmlRDKCentralCMInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    );

ANSC_STATUS
CosaDmlRDKCentralCmGetDownstreamChannel
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
		PCOSA_X_RDKCENTRAL_COM_CM_DS_OFDM_CHAN *ppDsOfdmChannel 	   
    );

ANSC_STATUS
CosaDmlRDKCentralCmGetUpstreamChannel
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
		PCOSA_X_RDKCENTRAL_COM_CM_US_OFDMA_CHAN *ppUsOfdmChannel 	   
    );

ANSC_STATUS
CosaDmlRDKCentralCmGetCMStatusofUpstreamChannel
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
		PCOSA_X_RDKCENTRAL_COM_CMSTATUSOFDMA_US *ppCMStatusofUsChannel 	   
    );

ANSC_STATUS
CosaDmlRDKCentralCmGetMTUSettings
    (
        ANSC_HANDLE                 hContext,
        PULONG                      MaxMTU
    );

#endif /* _COSA_RDKCENTRAL_CM_APIS_H */
