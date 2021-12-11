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

    module: cosa_device_info_dml.c

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

        01/05/2012    initial revision.

**************************************************************************/

#include "ansc_platform.h"
#include "plugin_main_apis.h"
#include "cosa_device_info_apis.h"
#include "cosa_device_info_dml.h"
#include "cosa_device_info_internal.h"
#include "safec_lib_common.h"

/***********************************************************************

 APIs for Object:

    DeviceInfo.

    *  DeviceInfo_GetParamBoolValue
    *  DeviceInfo_GetParamStringValue
    *  DeviceInfo_SetParamBoolValue
    *  DeviceInfo_SetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DeviceInfo_GetParamBoolValue
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
DeviceInfo_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO     pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDI;
    errno_t        rc = -1;
    int ind = -1;
    /* check the parameter name and return the corresponding value */
     rc = strcmp_s( "X_RDKCENTRAL-COM_FirmwareDownloadNow",strlen("X_RDKCENTRAL-COM_FirmwareDownloadNow"),ParamName,&ind);
     ERR_CHK(rc);
     if((!ind) && (rc == EOK))   
    {
	 if(pMyObject->Download_Control_Flag)
	 {
        	*pBool = FALSE;
	 }
        return TRUE;
    }

     rc =  strcmp_s( "X_RDKCENTRAL-COM_CableRfSignalStatus", strlen("X_RDKCENTRAL-COM_CableRfSignalStatus"),ParamName,&ind);
     ERR_CHK(rc);
     if((!ind) && (rc == EOK))
    {
#if defined (_XB6_PRODUCT_REQ_)
        BOOLEAN rfSignalStatus = TRUE; // default is TRUE.
#ifdef ENABLE_RDK_WANMANAGER
         if ( 0 == access( "/tmp/rfoff" , F_OK ) )
         {
             *pBool = FALSE;
             return TRUE;
         }
#endif

        CosaDmlDIGetRfSignalStatus(&rfSignalStatus);
        if(rfSignalStatus) {
            pMyObject->RfSignalStatus = TRUE;
            *pBool = TRUE;
            AnscTraceWarning(("Cable RF Signal Detected\n"));
        } else {
            pMyObject->RfSignalStatus = FALSE;
            *pBool = FALSE;
            AnscTraceWarning(("Cable RF Signal Not Detected\n"));
        }
#else
        *pBool = FALSE;
#endif

        return TRUE;
    }

    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        DeviceInfo_GetParamStringValue
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
                Usually size of 1023 will be used.
                If it's not big enough, put required size here and return 1;

    return:     0 if succeeded;
                1 if short of buffer size; (*pUlSize = required size)
                -1 if not supported.

**********************************************************************/
ULONG
DeviceInfo_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO     pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDI;
    char DL_Status[128]={0};
    char Protocol[16]={0};
    errno_t rc = -1;
    int ind = -1;
    /* check the parameter name and return the corresponding value */
    rc = strcmp_s("X_RDKCENTRAL-COM_FirmwareDownloadStatus",strlen("X_RDKCENTRAL-COM_FirmwareDownloadStatus"),ParamName,&ind);
    ERR_CHK(rc);
    if( (rc == EOK) && (!ind))
    {
    	 if(pMyObject->Download_Control_Flag)
    	 {
	        /* collect value */
	        CosaDmlDIGetDLStatus((ANSC_HANDLE)pMyObject, DL_Status);
	        if ( _ansc_strlen(DL_Status) >= *pUlSize )
	        {
	            *pUlSize = _ansc_strlen(DL_Status);
	            return 1;
	        }

                char *pDL_Status = DL_Status;
	        rc =  strcpy_s(pValue,*pUlSize, pDL_Status);
                if(rc != EOK)
                {
	          ERR_CHK(rc);
	          return -1;
                }
    	 }
        return 0;
    }

    rc = strcmp_s("X_RDKCENTRAL-COM_FirmwareDownloadProtocol",strlen("X_RDKCENTRAL-COM_FirmwareDownloadProtocol"),ParamName,&ind);
    ERR_CHK(rc);
    if( (rc == EOK) && (!ind))

    {
    	 if(pMyObject->Download_Control_Flag)
   	 {
	        /* collect value */
	        CosaDmlDIGetProtocol(Protocol);
	        if ( _ansc_strlen(Protocol) >= *pUlSize )
	        {
	            *pUlSize = _ansc_strlen(Protocol);
	            return 1;
	       }

                char *pProtocol = Protocol;
	        rc = strcpy_s(pValue,*pUlSize, pProtocol);
                if(rc != EOK)
                {
                    ERR_CHK(rc);
                    return -1;
                 }
    	 }
        return 0;
    }

    rc = strcmp_s("X_RDKCENTRAL-COM_FirmwareDownloadURL",strlen("X_RDKCENTRAL-COM_FirmwareDownloadURL"),ParamName,&ind);
    ERR_CHK(rc);
    if( (rc == EOK) && (!ind))

    {
    	 if(pMyObject->Download_Control_Flag)
    	 {
                //sometimes syscfg db init not done during bootup call of CosaDmlDIInit.So recalling CosaDmlDIGetURL
                if(_ansc_strlen(pMyObject->DownloadURL) == 0)
                    CosaDmlDIGetURL((ANSC_HANDLE)pMyObject);
	        /* collect value */
	        if ( _ansc_strlen(pMyObject->DownloadURL) >= *pUlSize )
	        {
	            *pUlSize = _ansc_strlen(pMyObject->DownloadURL);
	            return 1;
	        }
	        
                rc = strcpy_s(pValue,*pUlSize, pMyObject->DownloadURL);
                if(rc != EOK)
                {
                    ERR_CHK(rc);
                    return -1;
                 }

    	 }
        return 0;
    }

    rc = strcmp_s("X_RDKCENTRAL-COM_FirmwareToDownload",strlen("X_RDKCENTRAL-COM_FirmwareToDownload"),ParamName,&ind);
    ERR_CHK(rc);
    if( (rc == EOK) && (!ind))

    {
    	 if(pMyObject->Download_Control_Flag)
    	 {
                //sometimes syscfg db init not done during bootup call of CosaDmlDIInit.So recalling CosaDmlDIGetURL
                if(_ansc_strlen(pMyObject->Firmware_To_Download) == 0)
                    CosaDmlDIGetImage((ANSC_HANDLE)pMyObject); 
	        /* collect value */
	        if ( _ansc_strlen(pMyObject->Firmware_To_Download) >= *pUlSize )
	        {
	            *pUlSize = _ansc_strlen(pMyObject->Firmware_To_Download);
	            return 1;
	        }
	        
                rc = strcpy_s(pValue,*pUlSize,  pMyObject->Firmware_To_Download);
                if(rc != EOK)
                {
                    ERR_CHK(rc);
                    return -1;
                 }

    	 }
        return 0;
    }
    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DeviceInfo_SetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL                        bValue
            );

    description:

        This function is called to set BOOL parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL                        bValue
                The updated BOOL value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
DeviceInfo_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDI;
    errno_t        rc = -1;
    int ind = -1;
    /* check the parameter name and set the corresponding value */
    rc = strcmp_s( "X_RDKCENTRAL-COM_FirmwareDownloadNow",strlen("X_RDKCENTRAL-COM_FirmwareDownloadNow"),ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    
    {
    	 if(pMyObject->Download_Control_Flag)
    	 {
    	 	if(bValue)
    	 	{
        		CosaDmlDIDownloadNow((ANSC_HANDLE)pMyObject);
    	 	}	
    	 }
	 else
    	 {
		CcspTraceError(("FW DL is not allowed for this image type \n"));
    	 }
        return TRUE;
    }

    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DeviceInfo_SetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pString
            );

    description:

        This function is called to set string parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pString
                The updated string value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
DeviceInfo_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)g_pCosaBEManager->hDI;
    errno_t        rc = -1;
    int ind = -1;
    /* check the parameter name and set the corresponding value */
    rc = strcmp_s( "X_RDKCENTRAL-COM_FirmwareDownloadURL",strlen("X_RDKCENTRAL-COM_FirmwareDownloadURL"),ParamName,&ind);
    ERR_CHK(rc);
     if((!ind) && (rc == EOK))
    {
    	 if(pMyObject->Download_Control_Flag)
    	 {
        	CosaDmlDISetURL((ANSC_HANDLE)pMyObject, pString);
    	 }
	 else
    	 {
		CcspTraceError(("FW DL is not allowed for this image type \n"));
    	 }
        return TRUE;
    }

    rc = strcmp_s( "X_RDKCENTRAL-COM_FirmwareToDownload",strlen("X_RDKCENTRAL-COM_FirmwareToDownload"),ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    
    {
    	 if(pMyObject->Download_Control_Flag)
    	 {
        	CosaDmlDISetImage((ANSC_HANDLE)pMyObject, pString);
    	 }
	 else
    	 {
		CcspTraceError(("FW DL is not allowed for this image type \n"));
    	 }    	 
        return TRUE;
    }
    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}
