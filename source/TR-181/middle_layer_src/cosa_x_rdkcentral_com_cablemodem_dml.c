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

    module: cosa_x_rdkcentral_com_cablemodem_dml.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

    -------------------------------------------------------------------

    environment:

        platform independent

    -------------------------------------------------------------------

    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

       08/09/2017   initial revision.

**************************************************************************/

#include "ansc_platform.h"
#include "plugin_main_apis.h"
#include "cosa_x_rdkcentral_com_cablemodem_apis.h"
#include "cosa_x_rdkcentral_com_cablemodem_dml.h"
#include "cosa_x_rdkcentral_com_cablemodem_internal.h"

/***********************************************************************
 IMPORTANT NOTE:

 According to TR69 spec:
 On successful receipt of a SetParameterValues RPC, the CPE MUST apply 
 the changes to all of the specified Parameters atomically. That is, either 
 all of the value changes are applied together, or none of the changes are 
 applied at all. In the latter case, the CPE MUST return a fault response 
 indicating the reason for the failure to apply the changes. 
 
 The CPE MUST NOT apply any of the specified changes without applying all 
 of them.

 In order to set parameter values correctly, the back-end is required to
 hold the updated values until "Validate" and "Commit" are called. Only after
 all the "Validate" passed in different objects, the "Commit" will be called.
 Otherwise, "Rollback" will be called instead.

 The sequence in COSA Data Model will be:

 SetParamBoolValue/SetParamIntValue/SetParamUlongValue/SetParamStringValue
 -- Backup the updated values;

 if( Validate_XXX())
 {
     Commit_XXX();    -- Commit the update all together in the same object
 }
 else
 {
     Rollback_XXX();  -- Remove the update at backup;
 }
 
***********************************************************************/
/***********************************************************************

 APIs for Object:

    X_RDKCENTRAL-COM_CableModem

    *  X_RDKCENTRAL_COM_CableModem_GetParamUlongValue

***********************************************************************/

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_RDKCENTRAL_COM_CableModem_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

    description:

        This function is called to retrieve ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
X_RDKCENTRAL_COM_CableModem_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM)g_pCosaBEManager->hRDKCM;

    /* check the parameter name and return the corresponding value */
    AnscTraceWarning(("%s %d Entry '%s'\n",__FUNCTION__,__LINE__, ParamName));

    if( AnscEqualString(ParamName, "MaxMTU", TRUE) )
    {
        //Get MTU settings
        if ( CosaDmlRDKCentralCmGetMTUSettings( NULL, &pMyObject->MaxMTU ) != ANSC_STATUS_SUCCESS )
        {
            AnscTraceWarning(("CosaDmlRDKCentralCmGetMTUSettings failed!\n"));
            return FALSE;
        }
        /* collect value */
        *puLong = pMyObject->MaxMTU;

        return TRUE;
    }

    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/***********************************************************************

 APIs for Object:

    X_RDKCENTRAL-COM_CableModem.DsOfdmChan

    *  X_RDKCENTRAL_COM_CableModem_DsOfdmChan_IsUpdated
    *  X_RDKCENTRAL_COM_CableModem_DsOfdmChan_Synchronize    
    *  X_RDKCENTRAL_COM_CableModem_DsOfdmChan_GetEntryCount    
    *  X_RDKCENTRAL_COM_CableModem_DsOfdmChan_GetEntry        
    *  X_RDKCENTRAL_COM_CableModem_DsOfdmChan_GetParamUlongValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_RDKCENTRAL_COM_CableModem_DsOfdmChan_IsUpdated
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/
BOOL
X_RDKCENTRAL_COM_CableModem_DsOfdmChan_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM)g_pCosaBEManager->hRDKCM;

    if ( !pMyObject->DsOfdmChannelUpdateTime ) 
    {
        pMyObject->DsOfdmChannelUpdateTime = AnscGetTickInSeconds();

        return TRUE;
    }
    
    if ( pMyObject->DsOfdmChannelUpdateTime >= TIME_NO_NEGATIVE(AnscGetTickInSeconds() - RDKCM_REFRESH_INTERVAL) )
    {
        return FALSE;
    }
    else 
    {
        pMyObject->DsOfdmChannelUpdateTime = AnscGetTickInSeconds();

        return TRUE;
    }
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        X_RDKCENTRAL_COM_CableModem_DsOfdmChan_Synchronize
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to synchronize the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
X_RDKCENTRAL_COM_CableModem_DsOfdmChan_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM)g_pCosaBEManager->hRDKCM;
    ANSC_STATUS                     		   ret       = ANSC_STATUS_SUCCESS;

	//Get DS OFDM channel information by HAL API's
	CosaDmlRDKCentralCmGetDownstreamChannel( NULL,
											 &pMyObject->DsOfdmChannelTotalNumbers,
											 &pMyObject->pDsOfdmChannel );

    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        X_RDKCENTRAL_COM_CableModem_DsOfdmChan_GetEntryCount
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG
X_RDKCENTRAL_COM_CableModem_DsOfdmChan_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM)g_pCosaBEManager->hRDKCM;
    
    return pMyObject->DsOfdmChannelTotalNumbers;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        X_RDKCENTRAL_COM_CableModem_DsOfdmChan_GetEntry
            (
                ANSC_HANDLE                 hInsContext,
                ULONG                       nIndex,
                ULONG*                      pInsNumber
            );

    description:

        This function is called to retrieve the entry specified by the index.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ULONG                       nIndex,
                The index of this entry;

                ULONG*                      pInsNumber
                The output instance number;

    return:     The handle to identify the entry

**********************************************************************/
ANSC_HANDLE
X_RDKCENTRAL_COM_CableModem_DsOfdmChan_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM)g_pCosaBEManager->hRDKCM;

    if (nIndex < pMyObject->DsOfdmChannelTotalNumbers)
    {
        *pInsNumber  = nIndex + 1;

        return &pMyObject->pDsOfdmChannel[nIndex];
    }

    return NULL; /* return the handle */
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_RDKCENTRAL_COM_CableModem_DsOfdmChan_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

    description:

        This function is called to retrieve ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
X_RDKCENTRAL_COM_CableModem_DsOfdmChan_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM)g_pCosaBEManager->hRDKCM;
	PCOSA_X_RDKCENTRAL_COM_CM_DS_OFDM_CHAN	   pDsOfdmChannel = (PCOSA_X_RDKCENTRAL_COM_CM_DS_OFDM_CHAN)hInsContext;

    /* check the parameter name and return the corresponding value */
    AnscTraceWarning(("%s %d Entry '%s'\n",__FUNCTION__,__LINE__, ParamName));

    if( AnscEqualString(ParamName, "ChannelID", TRUE) )
    {
        /* collect value */
        *puLong = pDsOfdmChannel->ChannelId;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "ChanIndicator", TRUE) )
    {
        /* collect value */
        *puLong = pDsOfdmChannel->ChanIndicator;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "SubcarrierZeroFreq", TRUE) )
    {
        /* collect value */
        *puLong = pDsOfdmChannel->SubcarrierZeroFreq;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "FirstActiveSubcarrierNum", TRUE) )
    {
        /* collect value */
        *puLong = pDsOfdmChannel->FirstActiveSubcarrierNum;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "LastActiveSubcarrierNum", TRUE) )
    {
        /* collect value */
        *puLong = pDsOfdmChannel->LastActiveSubcarrierNum;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "NumActiveSubcarriers", TRUE) )
    {
        /* collect value */
        *puLong = pDsOfdmChannel->NumActiveSubcarriers;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "SubcarrierSpacing", TRUE) )
    {
        /* collect value */
        *puLong = pDsOfdmChannel->SubcarrierSpacing;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "CyclicPrefix", TRUE) )
    {
        /* collect value */
        *puLong = pDsOfdmChannel->CyclicPrefix;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "RollOffPeriod", TRUE) )
    {
        /* collect value */
        *puLong = pDsOfdmChannel->RollOffPeriod;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "PlcFreq", TRUE) )
    {
        /* collect value */
        *puLong = pDsOfdmChannel->PlcFreq;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "NumPilots", TRUE) )
    {
        /* collect value */
        *puLong = pDsOfdmChannel->NumPilots;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "TimeInterleaverDepth", TRUE) )
    {
        /* collect value */
        *puLong = pDsOfdmChannel->TimeInterleaverDepth;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "PlcTotalCodewords", TRUE) )
    {
        /* collect value */
        *puLong = ( ULONG )pDsOfdmChannel->PlcTotalCodewords;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "PlcUnreliableCodewords", TRUE) )
    {
        /* collect value */
        *puLong = ( ULONG )pDsOfdmChannel->PlcUnreliableCodewords;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "NcpTotalFields", TRUE) )
    {
        /* collect value */
        *puLong = ( ULONG )pDsOfdmChannel->NcpTotalFields;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "NcpFieldCrcFailures", TRUE) )
    {
        /* collect value */
        *puLong = ( ULONG )pDsOfdmChannel->NcpFieldCrcFailures;

        return TRUE;
    }

    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/***********************************************************************

 APIs for Object:

    X_RDKCENTRAL-COM_CableModem.UsOfdmaChan

    *  X_RDKCENTRAL_COM_CableModem_UsOfdmaChan_IsUpdated
    *  X_RDKCENTRAL_COM_CableModem_UsOfdmaChan_Synchronize        
    *  X_RDKCENTRAL_COM_CableModem_UsOfdmaChan_GetEntryCount    
    *  X_RDKCENTRAL_COM_CableModem_UsOfdmaChan_GetEntry    
    *  X_RDKCENTRAL_COM_CableModem_UsOfdmaChan_GetParamBoolValue
    *  X_RDKCENTRAL_COM_CableModem_UsOfdmaChan_GetParamUlongValue    

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_RDKCENTRAL_COM_CableModem_UsOfdmaChan_IsUpdated
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/
BOOL
X_RDKCENTRAL_COM_CableModem_UsOfdmaChan_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM)g_pCosaBEManager->hRDKCM;

    if ( !pMyObject->UsOfdmChannelUpdateTime ) 
    {
        pMyObject->UsOfdmChannelUpdateTime = AnscGetTickInSeconds();

        return TRUE;
    }
    
    if ( pMyObject->UsOfdmChannelUpdateTime >= TIME_NO_NEGATIVE(AnscGetTickInSeconds() - RDKCM_REFRESH_INTERVAL) )
    {
        return FALSE;
    }
    else 
    {
        pMyObject->UsOfdmChannelUpdateTime = AnscGetTickInSeconds();

        return TRUE;
    }
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        X_RDKCENTRAL_COM_CableModem_UsOfdmaChan_Synchronize
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to synchronize the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
X_RDKCENTRAL_COM_CableModem_UsOfdmaChan_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM)g_pCosaBEManager->hRDKCM;

	//Get US OFDM channel information by HAL API's
	CosaDmlRDKCentralCmGetUpstreamChannel( NULL,
											 &pMyObject->UsOfdmChannelTotalNumbers,
											 &pMyObject->pUsOfdmChannel );
    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        X_RDKCENTRAL_COM_CableModem_UsOfdmaChan_GetEntryCount
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG
X_RDKCENTRAL_COM_CableModem_UsOfdmaChan_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM)g_pCosaBEManager->hRDKCM;
    
    return pMyObject->UsOfdmChannelTotalNumbers;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        X_RDKCENTRAL_COM_CableModem_UsOfdmaChan_GetEntry
            (
                ANSC_HANDLE                 hInsContext,
                ULONG                       nIndex,
                ULONG*                      pInsNumber
            );

    description:

        This function is called to retrieve the entry specified by the index.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ULONG                       nIndex,
                The index of this entry;

                ULONG*                      pInsNumber
                The output instance number;

    return:     The handle to identify the entry

**********************************************************************/
ANSC_HANDLE
X_RDKCENTRAL_COM_CableModem_UsOfdmaChan_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM)g_pCosaBEManager->hRDKCM;

    if (nIndex < pMyObject->UsOfdmChannelTotalNumbers)
    {
        *pInsNumber  = nIndex + 1;

	    return &pMyObject->pUsOfdmChannel[nIndex];
    }

    return NULL; /* return the handle */
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_RDKCENTRAL_COM_CableModem_UsOfdmaChan_GetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL*                       pBool
            );

    description:

        This function is called to retrieve Boolean parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL*                       pBool
                The buffer of returned boolean value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
X_RDKCENTRAL_COM_CableModem_UsOfdmaChan_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM)g_pCosaBEManager->hRDKCM;
	PCOSA_X_RDKCENTRAL_COM_CM_US_OFDMA_CHAN	   pUsOfdmChannel = (PCOSA_X_RDKCENTRAL_COM_CM_US_OFDMA_CHAN)hInsContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "PreEqEnabled", TRUE) )
    {
	    *pBool = pUsOfdmChannel->PreEqEnabled;
        return TRUE;
    }

    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_RDKCENTRAL_COM_CableModem_UsOfdmaChan_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

    description:

        This function is called to retrieve ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
X_RDKCENTRAL_COM_CableModem_UsOfdmaChan_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM)g_pCosaBEManager->hRDKCM;
	PCOSA_X_RDKCENTRAL_COM_CM_US_OFDMA_CHAN	   pUsOfdmChannel = (PCOSA_X_RDKCENTRAL_COM_CM_US_OFDMA_CHAN)hInsContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "ChannelId", TRUE) )
    {
        /* collect value */
        *puLong = pUsOfdmChannel->ChannelId;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "ConfigChangeCt", TRUE) )
    {
        /* collect value */
        *puLong = pUsOfdmChannel->ConfigChangeCt;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "SubcarrierZeroFreq", TRUE) )
    {
        /* collect value */
        *puLong = pUsOfdmChannel->SubcarrierZeroFreq;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "FirstActiveSubcarrierNum", TRUE) )
    {
        /* collect value */
        *puLong = pUsOfdmChannel->FirstActiveSubcarrierNum;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "LastActiveSubcarrierNum", TRUE) )
    {
        /* collect value */
        *puLong = pUsOfdmChannel->LastActiveSubcarrierNum;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "NumActiveSubcarriers", TRUE) )
    {
        /* collect value */
        *puLong = pUsOfdmChannel->NumActiveSubcarriers;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "SubcarrierSpacing", TRUE) )
    {
        /* collect value */
        *puLong = pUsOfdmChannel->SubcarrierSpacing;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "CyclicPrefix", TRUE) )
    {
        /* collect value */
        *puLong = pUsOfdmChannel->CyclicPrefix;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "RollOffPeriod", TRUE) )
    {
        /* collect value */
        *puLong = pUsOfdmChannel->RollOffPeriod;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "NumSymbolsPerFrame", TRUE) )
    {
        /* collect value */
        *puLong = pUsOfdmChannel->NumSymbolsPerFrame;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "TxPower", TRUE) )
    {
        /* collect value */
        *puLong = pUsOfdmChannel->TxPower;

        return TRUE;
    }

    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/***********************************************************************

 APIs for Object:

    X_RDKCENTRAL-COM_CableModem.StatusOfdma

    *  X_RDKCENTRAL_COM_CableModem_StatusOfdma_IsUpdated
    *  X_RDKCENTRAL_COM_CableModem_StatusOfdma_Synchronize        
    *  X_RDKCENTRAL_COM_CableModem_StatusOfdma_GetEntryCount    
    *  X_RDKCENTRAL_COM_CableModem_StatusOfdma_GetEntry    
    *  X_RDKCENTRAL_COM_CableModem_StatusOfdma_GetParamBoolValue
    *  X_RDKCENTRAL_COM_CableModem_StatusOfdma_GetParamUlongValue    

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_RDKCENTRAL_COM_CableModem_StatusOfdma_IsUpdated
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/
BOOL
X_RDKCENTRAL_COM_CableModem_StatusOfdma_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM)g_pCosaBEManager->hRDKCM;

    if ( !pMyObject->CMStatusofUsChannelUpdateTime ) 
    {
        pMyObject->CMStatusofUsChannelUpdateTime = AnscGetTickInSeconds();

        return TRUE;
    }
    
    if ( pMyObject->CMStatusofUsChannelUpdateTime >= TIME_NO_NEGATIVE(AnscGetTickInSeconds() - RDKCM_REFRESH_INTERVAL) )
    {
        return FALSE;
    }
    else 
    {
        pMyObject->CMStatusofUsChannelUpdateTime = AnscGetTickInSeconds();

        return TRUE;
    }
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        X_RDKCENTRAL_COM_CableModem_StatusOfdma_Synchronize
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to synchronize the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
X_RDKCENTRAL_COM_CableModem_StatusOfdma_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM)g_pCosaBEManager->hRDKCM;

	//Get Status of US OFDM channel information by HAL API's
	CosaDmlRDKCentralCmGetCMStatusofUpstreamChannel( NULL,
													 &pMyObject->CMStatusofUsChannelTotalNumbers,
													 &pMyObject->pCMStatusofUsChannel );
    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        X_RDKCENTRAL_COM_CableModem_StatusOfdma_GetEntryCount
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG
X_RDKCENTRAL_COM_CableModem_StatusOfdma_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM)g_pCosaBEManager->hRDKCM;
    
    return pMyObject->CMStatusofUsChannelTotalNumbers;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        X_RDKCENTRAL_COM_CableModem_StatusOfdma_GetEntry
            (
                ANSC_HANDLE                 hInsContext,
                ULONG                       nIndex,
                ULONG*                      pInsNumber
            );

    description:

        This function is called to retrieve the entry specified by the index.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ULONG                       nIndex,
                The index of this entry;

                ULONG*                      pInsNumber
                The output instance number;

    return:     The handle to identify the entry

**********************************************************************/
ANSC_HANDLE
X_RDKCENTRAL_COM_CableModem_StatusOfdma_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM)g_pCosaBEManager->hRDKCM;

    if (nIndex < pMyObject->CMStatusofUsChannelTotalNumbers)
    {
        *pInsNumber  = nIndex + 1;

	    return &pMyObject->pCMStatusofUsChannel[nIndex];
    }

    return NULL; /* return the handle */
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_RDKCENTRAL_COM_CableModem_StatusOfdma_GetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL*                       pBool
            );

    description:

        This function is called to retrieve Boolean parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL*                       pBool
                The buffer of returned boolean value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
X_RDKCENTRAL_COM_CableModem_StatusOfdma_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM      pMyObject 			= (PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM)g_pCosaBEManager->hRDKCM;
	PCOSA_X_RDKCENTRAL_COM_CMSTATUSOFDMA_US	   pCMStatusofUsChannel = (PCOSA_X_RDKCENTRAL_COM_CMSTATUSOFDMA_US)hInsContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "IsMuted", TRUE) )
    {
	    *pBool = pCMStatusofUsChannel->IsMuted;
        return TRUE;
    }

    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_RDKCENTRAL_COM_CableModem_StatusOfdma_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

    description:

        This function is called to retrieve ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
X_RDKCENTRAL_COM_CableModem_StatusOfdma_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM      pMyObject 			= (PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM)g_pCosaBEManager->hRDKCM;
	PCOSA_X_RDKCENTRAL_COM_CMSTATUSOFDMA_US    pCMStatusofUsChannel = (PCOSA_X_RDKCENTRAL_COM_CMSTATUSOFDMA_US)hInsContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "ChannelId", TRUE) )
    {
        /* collect value */
        *puLong = pCMStatusofUsChannel->ChannelId;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "T3Timeouts", TRUE) )
    {
        /* collect value */
        *puLong = pCMStatusofUsChannel->T3Timeouts;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "T4Timeouts", TRUE) )
    {
        /* collect value */
        *puLong = pCMStatusofUsChannel->T4Timeouts;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "RangingAborteds", TRUE) )
    {
        /* collect value */
        *puLong = pCMStatusofUsChannel->RangingAborteds;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "T3Exceededs", TRUE) )
    {
        /* collect value */
        *puLong = pCMStatusofUsChannel->T3Exceededs;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "RangingStatus", TRUE) )
    {
        /* collect value */
        *puLong = pCMStatusofUsChannel->RangingStatus;

        return TRUE;
    }

    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

