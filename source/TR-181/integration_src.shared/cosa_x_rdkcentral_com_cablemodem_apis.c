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

    module: cosa_x_rdkcentral_com_cablemodem_apis.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    copyright:

        Cisco Systems, Inc.
        All Rights Reserved.

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaDmlDiInit
        *  CosaDmlDiGetManufacturer
    -------------------------------------------------------------------

    environment:

        platform independent

    -------------------------------------------------------------------

    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        18/09/2017    initial revision.

**************************************************************************/

//!!!  This code assumes that all data structures are the SAME in middle-layer APIs and HAL layer APIs
//!!!  So it uses casting from one to the other
#include "cosa_x_rdkcentral_com_cablemodem_apis.h"
#include "cosa_x_rdkcentral_com_cablemodem_internal.h"
#include "cm_hal.h" 

ANSC_STATUS
CosaDmlRDKCentralCMInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM  pMyObject      = (PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM)phContext;

	//Get DS OFDM channel information by HAL API's
	CosaDmlRDKCentralCmGetDownstreamChannel( NULL,
											 &pMyObject->DsOfdmChannelTotalNumbers,
											 &pMyObject->pDsOfdmChannel );

	//Get US OFDM channel information by HAL API's
	CosaDmlRDKCentralCmGetUpstreamChannel( NULL,
											 &pMyObject->UsOfdmChannelTotalNumbers,
											 &pMyObject->pUsOfdmChannel );

	//Get Status of US OFDM channel information by HAL API's
	CosaDmlRDKCentralCmGetCMStatusofUpstreamChannel( NULL,
													 &pMyObject->CMStatusofUsChannelTotalNumbers,
													 &pMyObject->pCMStatusofUsChannel );
}

ANSC_STATUS
CosaDmlRDKCentralCmGetDownstreamChannel
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
        PCOSA_X_RDKCENTRAL_COM_CM_DS_OFDM_CHAN *ppDsOfdmChannel        
    )    
{
//	PDOCSIF31_CM_DS_OFDM_CHAN *ppinfo 				   = NULL;
    u_long output_NumberOfEntries  = 0;

    if( NULL == pulCount )
    {
        AnscTraceWarning(("Input parameter is NULL %s, %d\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    //Free previously allocated memory
    if( NULL != *ppDsOfdmChannel )
    {
        AnscFreeMemory(  *ppDsOfdmChannel );
        *ppDsOfdmChannel = NULL;
    }

    *pulCount = 0;

    //Call CM HAL API to get Number of OFDM locked to allocate memory accordingly
    docsis_GetNumOfActiveRxOfdmChannels(&output_NumberOfEntries );

    if( output_NumberOfEntries > 0 )
    {
        PCOSA_X_RDKCENTRAL_COM_CM_DS_OFDM_CHAN pDsOfdmChannel = NULL;         
        int iLoopCount = 0;

        //Fill the required fields from HAL structure to local structure
        *pulCount = output_NumberOfEntries;
        *ppDsOfdmChannel = (PCOSA_X_RDKCENTRAL_COM_CM_DS_OFDM_CHAN)
                            AnscAllocateMemory( sizeof(COSA_X_RDKCENTRAL_COM_CM_DS_OFDM_CHAN) * output_NumberOfEntries );
        memset( *ppDsOfdmChannel, 0, sizeof(COSA_X_RDKCENTRAL_COM_CM_DS_OFDM_CHAN) * output_NumberOfEntries );
        pDsOfdmChannel = (PCOSA_X_RDKCENTRAL_COM_CM_DS_OFDM_CHAN)ppDsOfdmChannel[0];
        docsis_GetDsOfdmChanTable((PDOCSIF31_CM_DS_OFDM_CHAN*)ppDsOfdmChannel, &output_NumberOfEntries);

        for( iLoopCount = 0; iLoopCount < output_NumberOfEntries; ++iLoopCount )
        {
            //Copy each and every members from HAL
            pDsOfdmChannel[ iLoopCount ].ChannelId = (*ppDsOfdmChannel)[ iLoopCount ].ChannelId;
            pDsOfdmChannel[ iLoopCount ].ChanIndicator = (*ppDsOfdmChannel)[ iLoopCount ].ChanIndicator;
            pDsOfdmChannel[ iLoopCount ].SubcarrierZeroFreq = (*ppDsOfdmChannel)[ iLoopCount ].SubcarrierZeroFreq;
            pDsOfdmChannel[ iLoopCount ].FirstActiveSubcarrierNum = (*ppDsOfdmChannel)[ iLoopCount ].FirstActiveSubcarrierNum;
            pDsOfdmChannel[ iLoopCount ].LastActiveSubcarrierNum =(*ppDsOfdmChannel)[ iLoopCount ].LastActiveSubcarrierNum;
            pDsOfdmChannel[ iLoopCount ].NumActiveSubcarriers =(*ppDsOfdmChannel)[ iLoopCount ].NumActiveSubcarriers;
            pDsOfdmChannel[ iLoopCount ].SubcarrierSpacing = (*ppDsOfdmChannel)[ iLoopCount ].SubcarrierSpacing;
            pDsOfdmChannel[ iLoopCount ].CyclicPrefix = (*ppDsOfdmChannel)[ iLoopCount ].CyclicPrefix;
            pDsOfdmChannel[ iLoopCount ].RollOffPeriod = (*ppDsOfdmChannel)[ iLoopCount ].RollOffPeriod;
            pDsOfdmChannel[ iLoopCount ].PlcFreq = (*ppDsOfdmChannel)[ iLoopCount ].PlcFreq;
            pDsOfdmChannel[ iLoopCount ].NumPilots= (*ppDsOfdmChannel)[ iLoopCount ].NumPilots;
            pDsOfdmChannel[ iLoopCount ].TimeInterleaverDepth = (*ppDsOfdmChannel)[ iLoopCount ].TimeInterleaverDepth;
            pDsOfdmChannel[ iLoopCount ].PlcTotalCodewords= (*ppDsOfdmChannel)[ iLoopCount ].PlcTotalCodewords;
            pDsOfdmChannel[ iLoopCount ].PlcUnreliableCodewords = (*ppDsOfdmChannel)[ iLoopCount ].PlcUnreliableCodewords;
            pDsOfdmChannel[ iLoopCount ].NcpTotalFields=(*ppDsOfdmChannel)[ iLoopCount ].NcpTotalFields;
            pDsOfdmChannel[ iLoopCount ].NcpFieldCrcFailures=(*ppDsOfdmChannel)[ iLoopCount ].NcpFieldCrcFailures;
        }
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlRDKCentralCmGetUpstreamChannel
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
		PCOSA_X_RDKCENTRAL_COM_CM_US_OFDMA_CHAN *ppUsOfdmChannel 	   
    )
{
//	PDOCSIF31_CM_US_OFDMA_CHAN *ppinfo 				     	   = NULL;
	int 					   	  output_NumberOfEntries  = 0;	

    if( NULL == pulCount )
	{
		AnscTraceWarning(("Input parameter is NULL %s, %d\n", __FUNCTION__, __LINE__));
		return ANSC_STATUS_FAILURE;
	}

	//Free previously allocated memory
	if( NULL != *ppUsOfdmChannel )
	{
		AnscFreeMemory(  *ppUsOfdmChannel );
		*ppUsOfdmChannel = NULL;
	}

	*pulCount = 0;

//	Call CM HAL API to get UpStream channel tables information
//   docsis_GetUsOfdmaChanTable(PDOCSIF31_CM_US_OFDMA_CHAN *ppinfo, int *output_NumberOfEntries);

	if( output_NumberOfEntries > 0 )
	{
        PCOSA_X_RDKCENTRAL_COM_CM_US_OFDMA_CHAN pUsOfdmChannel = NULL;         
		int 									iLoopCount 	   = 0;
	
		//Fill the required fields from HAL structure to local structure
		*pulCount			= output_NumberOfEntries;
		*ppUsOfdmChannel 	= (PCOSA_X_RDKCENTRAL_COM_CM_US_OFDMA_CHAN)AnscAllocateMemory( sizeof(COSA_X_RDKCENTRAL_COM_CM_US_OFDMA_CHAN) * output_NumberOfEntries );

		memset( *ppUsOfdmChannel, 0, sizeof(COSA_X_RDKCENTRAL_COM_CM_US_OFDMA_CHAN) * output_NumberOfEntries );

		pUsOfdmChannel = (PCOSA_X_RDKCENTRAL_COM_CM_US_OFDMA_CHAN)ppUsOfdmChannel[0];

//After hook HAL we have to copy each member
#if 0
		for( iLoopCount = 0; iLoopCount < output_NumberOfEntries; ++iLoopCount )
	 	{
			//Copy each and every members from HAL
			pUsOfdmChannel[ iLoopCount ].ChannelId = 1; 				   
			pUsOfdmChannel[ iLoopCount ].ConfigChangeCt = 2; 			   
			pUsOfdmChannel[ iLoopCount ].SubcarrierZeroFreq = 3;		   
			pUsOfdmChannel[ iLoopCount ].FirstActiveSubcarrierNum = 4;	   
			pUsOfdmChannel[ iLoopCount ].LastActiveSubcarrierNum =5;	   
			pUsOfdmChannel[ iLoopCount ].NumActiveSubcarriers =6;		   
			pUsOfdmChannel[ iLoopCount ].SubcarrierSpacing = 7; 		   
			pUsOfdmChannel[ iLoopCount ].CyclicPrefix = 8;	 
			pUsOfdmChannel[ iLoopCount ].RollOffPeriod =9;  
			pUsOfdmChannel[ iLoopCount ].NumSymbolsPerFrame = 10;		 
			pUsOfdmChannel[ iLoopCount ].TxPower=11; 	 
			pUsOfdmChannel[ iLoopCount ].PreEqEnabled=1;
	 	}
#endif /* 0 */
	}

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlRDKCentralCmGetCMStatusofUpstreamChannel
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
		PCOSA_X_RDKCENTRAL_COM_CMSTATUSOFDMA_US *ppCMStatusofUsChannel 	   
    )
{
//	PDOCSIF31_CMSTATUSOFDMA_US *ppinfo 				     	   = NULL;
	int 					   	  output_NumberOfEntries  = 0;	

    if( NULL == pulCount )
	{
		AnscTraceWarning(("Input parameter is NULL %s, %d\n", __FUNCTION__, __LINE__));
		return ANSC_STATUS_FAILURE;
	}

	//Free previously allocated memory
	if( NULL != *ppCMStatusofUsChannel )
	{
		AnscFreeMemory(  *ppCMStatusofUsChannel );
		*ppCMStatusofUsChannel = NULL;
	}

	*pulCount = 0;

//	Call CM HAL API to get status of UpStream channel tables information
//INT docsis_GetStatusOfdmaUsTable(PDOCSIF31_CMSTATUSOFDMA_US *ppinfo, int *output_NumberOfEntries);

	if( output_NumberOfEntries > 0 )
	{
        PCOSA_X_RDKCENTRAL_COM_CMSTATUSOFDMA_US pCMStatusofUsChannel = NULL;         
		int 									iLoopCount 	  		 = 0;
	
		//Fill the required fields from HAL structure to local structure
		*pulCount			= output_NumberOfEntries;
		*ppCMStatusofUsChannel 	= (PCOSA_X_RDKCENTRAL_COM_CMSTATUSOFDMA_US)AnscAllocateMemory( sizeof(COSA_X_RDKCENTRAL_COM_CMSTATUSOFDMA_US) * output_NumberOfEntries );

		memset( *ppCMStatusofUsChannel, 0, sizeof(COSA_X_RDKCENTRAL_COM_CMSTATUSOFDMA_US) * output_NumberOfEntries );

		pCMStatusofUsChannel = (PCOSA_X_RDKCENTRAL_COM_CMSTATUSOFDMA_US)ppCMStatusofUsChannel[0];

//After hook HAL we have to copy each member
#if 0
		for( iLoopCount = 0; iLoopCount < output_NumberOfEntries; ++iLoopCount )
	 	{
			//Copy each and every members from HAL
			pCMStatusofUsChannel[ iLoopCount ].ChannelId = 1; 				   
			pCMStatusofUsChannel[ iLoopCount ].T3Timeouts = 2; 			   
			pCMStatusofUsChannel[ iLoopCount ].T4Timeouts = 3;		   
			pCMStatusofUsChannel[ iLoopCount ].RangingAborteds = 4;	   
			pCMStatusofUsChannel[ iLoopCount ].T3Exceededs =5;	   
			pCMStatusofUsChannel[ iLoopCount ].IsMuted = FALSE;		   
			pCMStatusofUsChannel[ iLoopCount ].RangingStatus = 4; 		   
	 	}
#endif /* 0 */
	}

    return ANSC_STATUS_SUCCESS;
}
