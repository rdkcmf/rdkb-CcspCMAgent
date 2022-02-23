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

    module: cosa_x_rdkcentral_com_cablemodem_apis.c

        For COSA Data Model Library Development

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
#include "safec_lib_common.h"

#if defined (_CM_HIGHSPLIT_SUPPORTED_)
#include <unistd.h>
#include <sysevent/sysevent.h>

static int sysevent_fd;
static token_t sysevent_token;

extern  ANSC_HANDLE   bus_handle;
extern char           g_Subsystem[32];

static VOID CosaDmlRDKCmCheckAndDisableMoCA( VOID );
#endif/* _CM_HIGHSPLIT_SUPPORTED_ */

ANSC_STATUS
CosaDmlRDKCentralCMInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    UNREFERENCED_PARAMETER(hDml);
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
	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlRDKCentralCmGetDownstreamChannel
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
        PCOSA_X_RDKCENTRAL_COM_CM_DS_OFDM_CHAN *ppDsOfdmChannel        
    )    
{
	UNREFERENCED_PARAMETER(hContext);
#if defined(_XB6_PRODUCT_REQ_)|| defined(_CBR_PRODUCT_REQ_)
	PDOCSIF31_CM_DS_OFDM_CHAN  pinfo 				   = NULL;
#endif

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
        int                                        output_NumberOfEntries  = 0;
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
        if(*ppDsOfdmChannel == NULL)
        {
           return ANSC_STATUS_FAILURE;
        }

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
			if(_ansc_strlen(pinfo[iLoopCount].averageSNR))
				AnscCopyString(pDsOfdmChannel[ iLoopCount ].averageSNR, pinfo[iLoopCount].averageSNR);
			if(_ansc_strlen(pinfo[iLoopCount].PowerLevel))
				AnscCopyString(pDsOfdmChannel[ iLoopCount ].PowerLevel, pinfo[iLoopCount].PowerLevel);
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
	UNREFERENCED_PARAMETER(hContext);
#if defined(_XB6_PRODUCT_REQ_)|| defined(_CBR_PRODUCT_REQ_)
	PDOCSIF31_CM_US_OFDMA_CHAN pinfo 				     	   = NULL;
#endif

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
        int                                               output_NumberOfEntries  = 0;

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
                if(*ppUsOfdmChannel == NULL)
                {
                    return ANSC_STATUS_FAILURE;
                }


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
	UNREFERENCED_PARAMETER(hContext);
#if defined(_XB6_PRODUCT_REQ_)|| defined(_CBR_PRODUCT_REQ_)
	PDOCSIF31_CMSTATUSOFDMA_US 	 pinfo 				      = NULL;
#endif

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
        int                                               output_NumberOfEntries  = 0;

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
              if(*ppCMStatusofUsChannel == NULL)
              {
                 return ANSC_STATUS_FAILURE;
              }

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

#if defined (_CM_HIGHSPLIT_SUPPORTED_)
ANSC_STATUS
CosaDmlRDKCMInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    UNREFERENCED_PARAMETER(hDml);
    UNREFERENCED_PARAMETER(phContext);

    //Initialize sysevent
	sysevent_fd = sysevent_open("127.0.0.1", SE_SERVER_WELL_KNOWN_PORT, SE_VERSION, "CMAgent", &sysevent_token);

    if ( sysevent_fd < 0 )
	{
        CcspTraceError(("Failed to open sysevent for CMAgent '%s'\n", __FUNCTION__));
	}

    CcspTraceInfo(("%s Done\n",__FUNCTION__));

	return ANSC_STATUS_SUCCESS;
}

VOID
CosaDmlRDKCmRegisterDiplexerVariationCallback
    (
        VOID
    )
{ 
	//Register Diplexer variation callback
	cm_hal_Register_DiplexerVariationCallback(CosaDmlRDKCmDiplexerVariationCallback);
}

ANSC_STATUS
CosaDmlRDKCmGetDiplexerSettings( UINT *pUSValue, UINT *pDSValue )
{
    CM_DIPLEXER_SETTINGS stCMDiplexer = { 0 };

    //Get CM Diplexer Value
    if( ANSC_STATUS_SUCCESS == cm_hal_get_DiplexerSettings( &stCMDiplexer ) )
    {
        *pUSValue = stCMDiplexer.usDiplexerSetting;
        *pDSValue = stCMDiplexer.dsDiplexerSetting;

        return ANSC_STATUS_SUCCESS;
    }

    return ANSC_STATUS_FAILURE;
}

/* CosaDmlRDKCmCheckAndDisableMoCA() */
static VOID CosaDmlRDKCmCheckAndDisableMoCA( VOID )
{
	parameterValStruct_t    value 		 = { "Device.MoCA.Interface.1.Enable", "false", ccsp_boolean};
	char  					*paramNames[]= { "Device.MoCA.Interface.1.Enable" };
	parameterValStruct_t    **valStrMoCAEnable;
	char  compo[ 256 ] 			  = "eRT.com.cisco.spvtg.ccsp.moca";
	char  bus[ 256 ]   			  = "/com/cisco/spvtg/ccsp/moca";
	char* faultParam 			  = NULL;
	int   ret 					  = 0,
		  nval					  = 0;
	BOOL  bNeedtoDisableMoCA	  = FALSE;

	ret = CcspBaseIf_getParameterValues ( bus_handle,
									      compo,
										  bus,
										  paramNames,
										  1,
										  &nval,
										  &valStrMoCAEnable
										 );
	if( ret != CCSP_Message_Bus_OK )
	{
		CcspTraceError(("%s: MoCA-Get Failed ret %d\n", __FUNCTION__, ret));
		return;
	}

	if( strcmp( "true", valStrMoCAEnable[0]->parameterValue ) == 0 )
	{
		bNeedtoDisableMoCA = TRUE;
	}
	else
	{
		CcspTraceInfo(("CM_HIGH_SPLIT: Ignoring MoCA disable since already disabled\n"));
	}

	free_parameterValStruct_t ( bus_handle, nval, valStrMoCAEnable );

	/* If MoCA enabled then we have to disable when this case */
	if( bNeedtoDisableMoCA )
	{
		ret = CcspBaseIf_setParameterValues(  bus_handle,
											  compo,
											  bus,
											  0,
											  0,
											  &value,
											  1,
											  TRUE,
											  &faultParam );

		if( ret != CCSP_Message_Bus_OK )
		{
			CcspTraceWarning(("%s: MoCA-Set Failed ret %d\n",__FUNCTION__,ret));
		}
		else
		{
			CcspTraceInfo(("CM_HIGH_SPLIT: Disabling MoCA due to CM High Split Detected\n"));
		}
	}
}

INT CosaDmlRDKCmDiplexerVariationCallback(CM_DIPLEXER_SETTINGS stCMDiplexerValue)
{
	INT ret = 0;

    /*  
	 *  Upstream (Upper Edge)
	 *  Mid-split - 85MHz 
	 *  High-split - 204MHz
	 * 
	 *  Downstream (Lower Edge) 
	 *  Mid-split - 108MHz 
	 *  High-split - 258MHz
	 * 
	 *  Downstream (Upper Edge) 
	 *  Mid-split - 1002MHz 
	 *  High-split - 1218MHz
	 * 
	 */

    AnscTraceInfo(("CM_HIGH_SPLIT_CB: Upstream:%d Downstream:%d\n", stCMDiplexerValue.usDiplexerSetting, stCMDiplexerValue.dsDiplexerSetting));

	if( stCMDiplexerValue.usDiplexerSetting > 85 )
	{
		//High Split Detected
		AnscTraceInfo(("CM_HIGH_SPLIT: High Split Detected from driver\n"));

		if( 0 == sysevent_set(sysevent_fd, sysevent_token, "cm_diplexer_mode", "high_split", 0)) 
        {
			CcspTraceInfo(("CM_HIGH_SPLIT: Updated cm_diplexer_mode as high_split\n"));	
		}
		else
		{
			CcspTraceInfo(("CM_HIGH_SPLIT: Failed to update cm_diplexer_mode as high_split\n"));	
		}

        /* 
		 * Check whether MoCA process initialized or not. 
		 * If Initialized then configure via DBUS call
		 * If not initialized then MoCA process initialization will take care of high split check
		 */
        if ( access("/tmp/moca_initialized", F_OK) == 0 )
		{
			//Check and Disable MoCA if enable
			CosaDmlRDKCmCheckAndDisableMoCA();
		}
        else
        {   
            CcspTraceInfo(("CM_HIGH_SPLIT: MoCA process is not up so unable to disable MoCA by using IPC\n"));
        }
	}
	else
	{
		//Mid Split Detected
		AnscTraceInfo(("CM_MID_SPLIT: Mid Split Detected from driver\n"));

		//Update sysevent value for mid split mode
        if( 0 == sysevent_set(sysevent_fd, sysevent_token, "cm_diplexer_mode", "mid_split", 0))
		{
			CcspTraceInfo(("CM_HIGH_SPLIT: Updated cm_diplexer_mode as mid_split\n"));	
		}
		else
		{
			CcspTraceInfo(("CM_HIGH_SPLIT: Failed to update cm_diplexer_mode as mid_split\n"));	
		}
	}

	return ret;
}
#endif /* _CM_HIGHSPLIT_SUPPORTED_ */
