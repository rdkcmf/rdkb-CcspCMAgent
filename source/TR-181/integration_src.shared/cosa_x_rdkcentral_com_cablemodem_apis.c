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
#if defined(_XB6_PRODUCT_REQ_)|| defined(_CBR_PRODUCT_REQ_)
	PDOCSIF31_CM_DS_OFDM_CHAN  pinfo 				   = NULL;
#endif
	int 					   output_NumberOfEntries  = 0;	

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

#if defined(_XB6_PRODUCT_REQ_)|| defined(_CBR_PRODUCT_REQ_)
//	Call CM HAL API to get DownStream channel tables information
	if ( ANSC_STATUS_SUCCESS != docsis_GetDsOfdmChanTable( &pinfo, &output_NumberOfEntries ) )
	{
		return ANSC_STATUS_FAILURE;
	}
		
	if( ( output_NumberOfEntries > 0 ) && \
		( NULL != pinfo ) 
	  )
	{
        PCOSA_X_RDKCENTRAL_COM_CM_DS_OFDM_CHAN pDsOfdmChannel = NULL;         
		int 								   iLoopCount 	  = 0;
	
		//Fill the required fields from HAL structure to local structure
		*pulCount			= output_NumberOfEntries;
		*ppDsOfdmChannel 	= (PCOSA_X_RDKCENTRAL_COM_CM_DS_OFDM_CHAN)AnscAllocateMemory( sizeof(COSA_X_RDKCENTRAL_COM_CM_DS_OFDM_CHAN) * output_NumberOfEntries );

		memset( *ppDsOfdmChannel, 0, sizeof(COSA_X_RDKCENTRAL_COM_CM_DS_OFDM_CHAN) * output_NumberOfEntries );

		pDsOfdmChannel = (PCOSA_X_RDKCENTRAL_COM_CM_DS_OFDM_CHAN)ppDsOfdmChannel[0];

		for( iLoopCount = 0; iLoopCount < output_NumberOfEntries; ++iLoopCount )
	 	{
			//Copy each and every members from HAL
			pDsOfdmChannel[ iLoopCount ].ChannelId 					= pinfo[iLoopCount].ChannelId; 				   
			pDsOfdmChannel[ iLoopCount ].ChanIndicator 				= pinfo[iLoopCount].ChanIndicator; 			   
			pDsOfdmChannel[ iLoopCount ].SubcarrierZeroFreq 		= pinfo[iLoopCount].SubcarrierZeroFreq;		   
			pDsOfdmChannel[ iLoopCount ].FirstActiveSubcarrierNum 	= pinfo[iLoopCount].FirstActiveSubcarrierNum;	   
			pDsOfdmChannel[ iLoopCount ].LastActiveSubcarrierNum	= pinfo[iLoopCount].LastActiveSubcarrierNum;	   
			pDsOfdmChannel[ iLoopCount ].NumActiveSubcarriers		= pinfo[iLoopCount].NumActiveSubcarriers;		   
			pDsOfdmChannel[ iLoopCount ].SubcarrierSpacing 			= pinfo[iLoopCount].SubcarrierSpacing; 		   
			pDsOfdmChannel[ iLoopCount ].CyclicPrefix 				= pinfo[iLoopCount].CyclicPrefix;	 
			pDsOfdmChannel[ iLoopCount ].RollOffPeriod 				= pinfo[iLoopCount].RollOffPeriod;  
			pDsOfdmChannel[ iLoopCount ].PlcFreq 					= pinfo[iLoopCount].PlcFreq;		 
			pDsOfdmChannel[ iLoopCount ].NumPilots					= pinfo[iLoopCount].NumPilots; 	 
			pDsOfdmChannel[ iLoopCount ].TimeInterleaverDepth		= pinfo[iLoopCount].TimeInterleaverDepth;
			pDsOfdmChannel[ iLoopCount ].PlcTotalCodewords			= pinfo[iLoopCount].PlcTotalCodewords;
			pDsOfdmChannel[ iLoopCount ].PlcUnreliableCodewords		= pinfo[iLoopCount].PlcUnreliableCodewords;
			pDsOfdmChannel[ iLoopCount ].NcpTotalFields				= pinfo[iLoopCount].NcpTotalFields; 
			pDsOfdmChannel[ iLoopCount ].NcpFieldCrcFailures		= pinfo[iLoopCount].NcpFieldCrcFailures;
	 	}
		
		//Free ppinfo allocated HAL structure 
		if( NULL != pinfo )
		{
			free( pinfo );
			pinfo = NULL;
		}
	}
#endif

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
#if defined(_XB6_PRODUCT_REQ_)|| defined(_CBR_PRODUCT_REQ_)
	PDOCSIF31_CM_US_OFDMA_CHAN pinfo 				     	   = NULL;
#endif
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

#if defined(_XB6_PRODUCT_REQ_)|| defined(_CBR_PRODUCT_REQ_)
//	Call CM HAL API to get UpStream channel tables information
   	if( ANSC_STATUS_SUCCESS != docsis_GetUsOfdmaChanTable( &pinfo, &output_NumberOfEntries ))
	{
		return ANSC_STATUS_FAILURE;
	}

	if( ( output_NumberOfEntries > 0 ) && \
		( NULL != pinfo ) 
	   )
	{
        PCOSA_X_RDKCENTRAL_COM_CM_US_OFDMA_CHAN pUsOfdmChannel = NULL;         
		int 									iLoopCount 	   = 0;
	
		//Fill the required fields from HAL structure to local structure
		*pulCount			= output_NumberOfEntries;
		*ppUsOfdmChannel 	= (PCOSA_X_RDKCENTRAL_COM_CM_US_OFDMA_CHAN)AnscAllocateMemory( sizeof(COSA_X_RDKCENTRAL_COM_CM_US_OFDMA_CHAN) * output_NumberOfEntries );

		memset( *ppUsOfdmChannel, 0, sizeof(COSA_X_RDKCENTRAL_COM_CM_US_OFDMA_CHAN) * output_NumberOfEntries );

		pUsOfdmChannel = (PCOSA_X_RDKCENTRAL_COM_CM_US_OFDMA_CHAN)ppUsOfdmChannel[0];

		for( iLoopCount = 0; iLoopCount < output_NumberOfEntries; ++iLoopCount )
	 	{
			//Copy each and every members from HAL
			pUsOfdmChannel[ iLoopCount ].ChannelId 					= pinfo[ iLoopCount ].ChannelId; 				   
			pUsOfdmChannel[ iLoopCount ].ConfigChangeCt 			= pinfo[ iLoopCount ].ConfigChangeCt; 			   
			pUsOfdmChannel[ iLoopCount ].SubcarrierZeroFreq 		= pinfo[ iLoopCount ].SubcarrierZeroFreq;		   
			pUsOfdmChannel[ iLoopCount ].FirstActiveSubcarrierNum   = pinfo[ iLoopCount ].FirstActiveSubcarrierNum;	   
			pUsOfdmChannel[ iLoopCount ].LastActiveSubcarrierNum 	= pinfo[ iLoopCount ].LastActiveSubcarrierNum;	   
			pUsOfdmChannel[ iLoopCount ].NumActiveSubcarriers 		= pinfo[ iLoopCount ].NumActiveSubcarriers;		   
			pUsOfdmChannel[ iLoopCount ].SubcarrierSpacing 			= pinfo[ iLoopCount ].SubcarrierSpacing; 		   
			pUsOfdmChannel[ iLoopCount ].CyclicPrefix 				= pinfo[ iLoopCount ].CyclicPrefix;	 
			pUsOfdmChannel[ iLoopCount ].RollOffPeriod				= pinfo[ iLoopCount ].RollOffPeriod;  
			pUsOfdmChannel[ iLoopCount ].NumSymbolsPerFrame 		= pinfo[ iLoopCount ].NumSymbolsPerFrame;		 
			pUsOfdmChannel[ iLoopCount ].TxPower					= pinfo[ iLoopCount ].TxPower; 	 
			pUsOfdmChannel[ iLoopCount ].PreEqEnabled				= pinfo[ iLoopCount ].PreEqEnabled;
	 	}

		//Free ppinfo allocated HAL structure 
		if( NULL != pinfo )
		{
			free(  pinfo );
			pinfo = NULL;
		}
	}
#endif

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
#if defined(_XB6_PRODUCT_REQ_)|| defined(_CBR_PRODUCT_REQ_)
	PDOCSIF31_CMSTATUSOFDMA_US 	 pinfo 				      = NULL;
#endif
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

#if defined(_XB6_PRODUCT_REQ_)|| defined(_CBR_PRODUCT_REQ_)
//	Call CM HAL API to get status of UpStream channel tables information
	if ( ANSC_STATUS_SUCCESS != docsis_GetStatusOfdmaUsTable( &pinfo, &output_NumberOfEntries ) )
	{
		return ANSC_STATUS_FAILURE;
	}

	if( ( output_NumberOfEntries > 0 ) && \
		( NULL != pinfo ) 
	  )
	{
        PCOSA_X_RDKCENTRAL_COM_CMSTATUSOFDMA_US pCMStatusofUsChannel = NULL;         
		int 									iLoopCount 	  		 = 0;
	
		//Fill the required fields from HAL structure to local structure
		*pulCount			= output_NumberOfEntries;
		*ppCMStatusofUsChannel 	= (PCOSA_X_RDKCENTRAL_COM_CMSTATUSOFDMA_US)AnscAllocateMemory( sizeof(COSA_X_RDKCENTRAL_COM_CMSTATUSOFDMA_US) * output_NumberOfEntries );

		memset( *ppCMStatusofUsChannel, 0, sizeof(COSA_X_RDKCENTRAL_COM_CMSTATUSOFDMA_US) * output_NumberOfEntries );

		pCMStatusofUsChannel = (PCOSA_X_RDKCENTRAL_COM_CMSTATUSOFDMA_US)ppCMStatusofUsChannel[0];

		for( iLoopCount = 0; iLoopCount < output_NumberOfEntries; ++iLoopCount )
	 	{
			//Copy each and every members from HAL
			pCMStatusofUsChannel[ iLoopCount ].ChannelId 		= pinfo[iLoopCount].ChannelId; 				   
			pCMStatusofUsChannel[ iLoopCount ].T3Timeouts 		= pinfo[iLoopCount].T3Timeouts; 			   
			pCMStatusofUsChannel[ iLoopCount ].T4Timeouts 		= pinfo[iLoopCount].T4Timeouts;		   
			pCMStatusofUsChannel[ iLoopCount ].RangingAborteds  = pinfo[iLoopCount].RangingAborteds;	   
			pCMStatusofUsChannel[ iLoopCount ].T3Exceededs 		= pinfo[iLoopCount].T3Exceededs;	   
			pCMStatusofUsChannel[ iLoopCount ].IsMuted 			= pinfo[iLoopCount].IsMuted;		   
			pCMStatusofUsChannel[ iLoopCount ].RangingStatus 	= pinfo[iLoopCount].RangingStatus; 		   
	 	}

		//Free ppinfo allocated HAL structure 
		if( NULL != pinfo )
		{
			free( pinfo );
			pinfo = NULL;
		}
	}
#endif

    return ANSC_STATUS_SUCCESS;
}
