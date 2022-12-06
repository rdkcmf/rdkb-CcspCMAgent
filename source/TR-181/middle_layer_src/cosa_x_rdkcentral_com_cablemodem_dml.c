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
#include "safec_lib_common.h"

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

    X_RDKCENTRAL-COM_CableModem.DsOfdmChan

    *  X_RDKCENTRAL_COM_CableModem_DsOfdmChan_IsUpdated
    *  X_RDKCENTRAL_COM_CableModem_DsOfdmChan_Synchronize    
    *  X_RDKCENTRAL_COM_CableModem_DsOfdmChan_GetEntryCount    
    *  X_RDKCENTRAL_COM_CableModem_DsOfdmChan_GetEntry        
    *  X_RDKCENTRAL_COM_CableModem_DsOfdmChan_GetParamUlongValue
    *  X_RDKCENTRAL_COM_CableModem_DsOfdmChan_GetParamStringValue 
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
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM)g_pCosaBEManager->hRDKCM;

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
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
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
	PCOSA_X_RDKCENTRAL_COM_CM_DS_OFDM_CHAN	   pDsOfdmChannel = (PCOSA_X_RDKCENTRAL_COM_CM_DS_OFDM_CHAN)hInsContext;
        errno_t rc =-1;
        int ind =-1;

    /* check the parameter name and return the corresponding value */
    AnscTraceWarning(("%s %d Entry '%s'\n",__FUNCTION__,__LINE__, ParamName));

    rc = strcmp_s("ChannelID",strlen("ChannelID"),ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))

    
    {
        /* collect value */
        *puLong = pDsOfdmChannel->ChannelId;

        return TRUE;
    }

    rc =  strcmp_s("ChanIndicator",strlen("ChanIndicator"), ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *puLong = pDsOfdmChannel->ChanIndicator;

        return TRUE;
    }

    rc= strcmp_s("SubcarrierZeroFreq",strlen( "SubcarrierZeroFreq"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *puLong = pDsOfdmChannel->SubcarrierZeroFreq;

        return TRUE;
    }

    rc = strcmp_s("FirstActiveSubcarrierNum",strlen("FirstActiveSubcarrierNum"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *puLong = pDsOfdmChannel->FirstActiveSubcarrierNum;

        return TRUE;
    }

    rc =  strcmp_s("LastActiveSubcarrierNum",strlen("LastActiveSubcarrierNum"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *puLong = pDsOfdmChannel->LastActiveSubcarrierNum;

        return TRUE;
    }

    rc  = strcmp_s("NumActiveSubcarriers", strlen("NumActiveSubcarriers"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *puLong = pDsOfdmChannel->NumActiveSubcarriers;

        return TRUE;
    }

    rc = strcmp_s("SubcarrierSpacing",strlen("SubcarrierSpacing"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *puLong = pDsOfdmChannel->SubcarrierSpacing;

        return TRUE;
    }

    rc = strcmp_s("CyclicPrefix",strlen("CyclicPrefix"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *puLong = pDsOfdmChannel->CyclicPrefix;

        return TRUE;
    }

    rc = strcmp_s("RollOffPeriod", strlen("RollOffPeriod"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *puLong = pDsOfdmChannel->RollOffPeriod;

        return TRUE;
    }

    rc = strcmp_s("PlcFreq", strlen("PlcFreq"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *puLong = pDsOfdmChannel->PlcFreq;

        return TRUE;
    }

    rc =  strcmp_s( "NumPilots",strlen("NumPilots"),ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *puLong = pDsOfdmChannel->NumPilots;

        return TRUE;
    }

    rc = strcmp_s("TimeInterleaverDepth", strlen("TimeInterleaverDepth"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *puLong = pDsOfdmChannel->TimeInterleaverDepth;

        return TRUE;
    }

    rc = strcmp_s("PlcTotalCodewords", strlen("PlcTotalCodewords"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *puLong = ( ULONG )pDsOfdmChannel->PlcTotalCodewords;

        return TRUE;
    }

    rc =  strcmp_s("PlcUnreliableCodewords",strlen("PlcUnreliableCodewords"), ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *puLong = ( ULONG )pDsOfdmChannel->PlcUnreliableCodewords;

        return TRUE;
    }

    rc =  strcmp_s("NcpTotalFields",strlen("NcpTotalFields"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *puLong = ( ULONG )pDsOfdmChannel->NcpTotalFields;

        return TRUE;
    }

    rc =  strcmp_s("NcpFieldCrcFailures", strlen("NcpFieldCrcFailures"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *puLong = ( ULONG )pDsOfdmChannel->NcpFieldCrcFailures;

        return TRUE;
    }

    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        X_RDKCENTRAL_COM_CableModem_DsOfdmChan_GetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pValue,
                ULONG*                      pUlSize
            );

    description:

        This function is called to retrieve string parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue,
                The string value buffer;

                ULONG*                      pUlSize
                The buffer of length of string value;
                Usually size of 4095 will be used.
                If it's not big enough, put required size here and return 1;

    return:     0 if succeeded;
                1 if short of buffer size; (*pUlSize = required size)
                -1 if not supported.

**********************************************************************/
ULONG
X_RDKCENTRAL_COM_CableModem_DsOfdmChan_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_X_RDKCENTRAL_COM_CM_DS_OFDM_CHAN     pDsOfdmChannel = (PCOSA_X_RDKCENTRAL_COM_CM_DS_OFDM_CHAN)hInsContext;
    
    /* check the parameter name and return the corresponding value */
    if (strcmp(ParamName, "PowerLevel") == 0)
    {
	    /* collect value */
	    if ( _ansc_strlen(pDsOfdmChannel->PowerLevel) >= *pUlSize )
	    {
		    *pUlSize = _ansc_strlen(pDsOfdmChannel->PowerLevel);
		    return 1;
	    }

	    AnscCopyString(pValue, pDsOfdmChannel->PowerLevel);
	    return 0;
    }

    if (strcmp(ParamName, "SNRLevel") == 0)
    {

	    /* collect value */
	    if ( _ansc_strlen(pDsOfdmChannel->averageSNR) >= *pUlSize )
	    {
		    *pUlSize = _ansc_strlen(pDsOfdmChannel->averageSNR);
		    return 1;
	    }

	    AnscCopyString(pValue, pDsOfdmChannel->averageSNR);
	    return 0;
    }
    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
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
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
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
	PCOSA_X_RDKCENTRAL_COM_CM_US_OFDMA_CHAN	   pUsOfdmChannel = (PCOSA_X_RDKCENTRAL_COM_CM_US_OFDMA_CHAN)hInsContext;
        errno_t rc = -1;
        int ind = -1;
    /* check the parameter name and return the corresponding value */
    rc =  strcmp_s( "PreEqEnabled",strlen("PreEqEnabled"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
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
	PCOSA_X_RDKCENTRAL_COM_CM_US_OFDMA_CHAN	   pUsOfdmChannel = (PCOSA_X_RDKCENTRAL_COM_CM_US_OFDMA_CHAN)hInsContext;

    /* check the parameter name and return the corresponding value */
    errno_t rc = -1;
    int ind =-1;
    
    rc =  strcmp_s("ChannelId",strlen("ChannelId"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *puLong = pUsOfdmChannel->ChannelId;

        return TRUE;
    }

    rc =  strcmp_s("ConfigChangeCt", strlen("ConfigChangeCt"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *puLong = pUsOfdmChannel->ConfigChangeCt;

        return TRUE;
    }

    rc = strcmp_s("SubcarrierZeroFreq",strlen("SubcarrierZeroFreq"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *puLong = pUsOfdmChannel->SubcarrierZeroFreq;

        return TRUE;
    }

    rc =  strcmp_s("FirstActiveSubcarrierNum", strlen("FirstActiveSubcarrierNum"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *puLong = pUsOfdmChannel->FirstActiveSubcarrierNum;

        return TRUE;
    }

    rc = strcmp_s("LastActiveSubcarrierNum", strlen("LastActiveSubcarrierNum"),ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *puLong = pUsOfdmChannel->LastActiveSubcarrierNum;

        return TRUE;
    }

    rc = strcmp_s("NumActiveSubcarriers", strlen("NumActiveSubcarriers"),ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *puLong = pUsOfdmChannel->NumActiveSubcarriers;

        return TRUE;
    }

    rc = strcmp_s("SubcarrierSpacing",strlen("SubcarrierSpacing"),ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *puLong = pUsOfdmChannel->SubcarrierSpacing;

        return TRUE;
    }

    rc = strcmp_s( "CyclicPrefix",strlen("CyclicPrefix"),ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *puLong = pUsOfdmChannel->CyclicPrefix;

        return TRUE;
    }

    rc =  strcmp_s( "RollOffPeriod",strlen("RollOffPeriod"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *puLong = pUsOfdmChannel->RollOffPeriod;

        return TRUE;
    }

    rc = strcmp_s("NumSymbolsPerFrame",strlen("NumSymbolsPerFrame"),ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *puLong = pUsOfdmChannel->NumSymbolsPerFrame;

        return TRUE;
    }

    rc =  strcmp_s("TxPower", strlen("TxPower"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
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
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
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
    UNREFERENCED_PARAMETER(hInsContext);
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
	PCOSA_X_RDKCENTRAL_COM_CMSTATUSOFDMA_US	   pCMStatusofUsChannel = (PCOSA_X_RDKCENTRAL_COM_CMSTATUSOFDMA_US)hInsContext;
        errno_t rc =-1;
        int ind = -1;

    /* check the parameter name and return the corresponding value */
    rc = strcmp_s("IsMuted",strlen("IsMuted"),ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
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
	PCOSA_X_RDKCENTRAL_COM_CMSTATUSOFDMA_US    pCMStatusofUsChannel = (PCOSA_X_RDKCENTRAL_COM_CMSTATUSOFDMA_US)hInsContext;
        errno_t rc = -1;
        int ind = -1;

    /* check the parameter name and return the corresponding value */
    rc =  strcmp_s( "ChannelId",strlen("ChannelId"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *puLong = pCMStatusofUsChannel->ChannelId;

        return TRUE;
    }

    rc =  strcmp_s("T3Timeouts",strlen("T3Timeouts"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))

    {
        /* collect value */
        *puLong = pCMStatusofUsChannel->T3Timeouts;

        return TRUE;
    }

    rc = strcmp_s("T4Timeouts", strlen("T4Timeouts"),ParamName, &ind);
     ERR_CHK(rc);
    if((!ind) && (rc == EOK))

    {
        /* collect value */
        *puLong = pCMStatusofUsChannel->T4Timeouts;

        return TRUE;
    }

    rc = strcmp_s("RangingAborteds",strlen("RangingAborteds"),ParamName, &ind);
     ERR_CHK(rc);
    if((!ind) && (rc == EOK))

    {
        /* collect value */
        *puLong = pCMStatusofUsChannel->RangingAborteds;

        return TRUE;
    }

    rc =  strcmp_s("T3Exceededs", strlen("T3Exceededs"),ParamName, &ind);
     ERR_CHK(rc);
    if((!ind) && (rc == EOK))

    {
        /* collect value */
        *puLong = pCMStatusofUsChannel->T3Exceededs;

        return TRUE;
    }

    rc =  strcmp_s("RangingStatus",strlen("RangingStatus"),ParamName, &ind);
     ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *puLong = pCMStatusofUsChannel->RangingStatus;

        return TRUE;
    }

    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

#if defined(_CM_HIGHSPLIT_SUPPORTED_)
/***********************************************************************

 APIs for Object:

    X_RDK_CableModem.

    *  X_RDK_CableModem_GetParamUlongValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_RDK_CableModem_GetParamUlongValue
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
X_RDK_CableModem_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    errno_t rc = -1;
    int ind = -1; 
    UINT uiUSValue = 0, uiDSValue = 0;

    //Get US and DS Diplexer values from HAL
    CosaDmlRDKCmGetDiplexerSettings( &uiUSValue, &uiDSValue );

    /* check the parameter name and return the corresponding value */
    rc =  strcmp_s( "DownstreamDiplexerSetting",strlen("DownstreamDiplexerSetting"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *puLong = uiDSValue;

        return TRUE;
    }

    rc =  strcmp_s("UpstreamDiplexerSetting",strlen("UpstreamDiplexerSetting"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *puLong = uiUSValue;

        return TRUE;
    }

    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}
#endif /* * _CM_HIGHSPLIT_SUPPORTED_ */
