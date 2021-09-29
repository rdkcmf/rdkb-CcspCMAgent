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

    module: cosa_x_cisco_com_cablemodem_dml.c

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
#include "cosa_x_cisco_com_cablemodem_apis.h"
#include "cosa_x_cisco_com_cablemodem_dml.h"
#include "cosa_x_cisco_com_cablemodem_internal.h"
#include "safec_lib_common.h"

static int g_DocsisLog_clean_flg=0;  
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

    X_CISCO_COM_CableModem.

    *  X_CISCO_COM_CableModem_GetParamBoolValue
    *  X_CISCO_COM_CableModem_GetParamIntValue
    *  X_CISCO_COM_CableModem_GetParamUlongValue
    *  X_CISCO_COM_CableModem_GetParamStringValue
    *  X_CISCO_COM_CableModem_SetParamBoolValue
    *  X_CISCO_COM_CableModem_SetParamIntValue
    *  X_CISCO_COM_CableModem_SetParamUlongValue
    *  X_CISCO_COM_CableModem_SetParamStringValue
    *  X_CISCO_COM_CableModem_Validate
    *  X_CISCO_COM_CableModem_Commit
    *  X_CISCO_COM_CableModem_Rollback

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_CableModem_GetParamBoolValue
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
X_CISCO_COM_CableModem_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_CABLEMODEM     pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)g_pCosaBEManager->hCM;
    PCOSA_DML_CM_LOG               pCfg      = &pMyObject->CmLog;
#if defined (FEATURE_RDKB_WAN_MANAGER)
    PCOSA_DML_CM_WANCFG               pWanCfg      = &pMyObject->CmWanCfg;
#endif   
    COSA_CM_DOCSIS_INFO            DInfo;
    memset(&DInfo, 0, sizeof(COSA_CM_DOCSIS_INFO));
    errno_t rc = -1;
    int ind = -1;  
    /* check the parameter name and return the corresponding value */
     rc = strcmp_s( "BPIState",strlen("BPIState"),ParamName, &ind);
     ERR_CHK(rc);
     if((!ind) && (rc == EOK))    
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *pBool = DInfo.BPIState;

        return TRUE;
    }

    rc = strcmp_s( "NetworkAccess",strlen("NetworkAccess"),ParamName,&ind );
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))  
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *pBool = DInfo.NetworkAccess;

        return TRUE;
    }

     rc = strcmp_s( "LoopDiagnosticsStart",strlen("LoopDiagnosticsStart"),ParamName, &ind);
     ERR_CHK(rc);
     if((!ind) && (rc == EOK))
    {
        if (CosaDmlCMGetLoopDiagnosticsStart(NULL, pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }
    
     rc = strcmp_s( "EnableLog",strlen("EnableLog"),ParamName, &ind);    
     ERR_CHK(rc);
     if((!ind) && (rc == EOK))
     
     {
        /* collect value */
        *pBool = pCfg->EnableLog;
        return TRUE;
     }

     rc =  strcmp_s("CleanDocsislog",strlen("CleanDocsislog"),ParamName, &ind);
     ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *pBool = pCfg->CleanDocsisLog;
        return TRUE;
    }

    rc = strcmp_s( "DOCSISEnableCert",strlen("DOCSISEnableCert"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCmGetCMCertStatus(NULL, pBool) == ANSC_STATUS_SUCCESS)
            return TRUE;
        return FALSE;
    }

#if defined (FEATURE_RDKB_WAN_MANAGER)
     rc =  strcmp_s("ConfigureWan",strlen("ConfigureWan"),ParamName, &ind);
     ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *pBool = pWanCfg->ConfigureWan;
        return TRUE;
    }

     rc =  strcmp_s("CustomWanConfigUpdate",strlen("CustomWanConfigUpdate"),ParamName, &ind);
     ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *pBool = pWanCfg->CustomWanConfigUpdate;
        return TRUE;
    }
#endif

    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_CableModem_GetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int*                        pInt
            );

    description:

        This function is called to retrieve integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                        pInt
                The buffer of returned integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
X_CISCO_COM_CableModem_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    COSA_CM_DHCP_INFO               Info;
    memset(&Info, 0, sizeof(COSA_CM_DHCP_INFO));
     errno_t rc = -1;
    int ind = -1; 
    /* check the parameter name and return the corresponding value */
     rc = strcmp_s("TimeOffset",strlen("TimeOffset"),ParamName, &ind);
     ERR_CHK(rc);
     if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetDHCPInfo(NULL, &Info) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *pInt = Info.TimeOffset;

        return TRUE;
    }

    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_CableModem_GetParamUlongValue
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
X_CISCO_COM_CableModem_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    COSA_CM_DHCP_INFO               Info;
    COSA_CM_IPV6DHCP_INFO           IPv6Info;
    COSA_CM_DOCSIS_INFO             DInfo;
     errno_t rc = -1;
    int ind = -1; 

    memset(&Info, 0, sizeof(COSA_CM_DHCP_INFO));
    memset(&IPv6Info, 0, sizeof(COSA_CM_IPV6DHCP_INFO));
    memset(&DInfo, 0, sizeof(COSA_CM_DOCSIS_INFO));
    /* check the parameter name and return the corresponding value */

     rc = strcmp_s( "DOCSISDHCPAttempts",strlen("DOCSISDHCPAttempts"),ParamName,&ind);
     ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = DInfo.DOCSISDHCPAttempts;

        return TRUE;
    }

    rc =  strcmp_s( "DOCSISTftpAttempts",strlen("DOCSISTftpAttempts"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = DInfo.DOCSISTftpAttempts;

        return TRUE;
    }

    rc =  strcmp_s("MaxCpeAllowed",strlen("MaxCpeAllowed"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = DInfo.MaxCpeAllowed;

        return TRUE;
    }

    rc =  strcmp_s( "UpgradeServerIP",strlen("UpgradeServerIP"),ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = DInfo.UpgradeServerIP.Value;

        return TRUE;
    }

    rc =  strcmp_s( "IPAddress",strlen("IPAddress"),ParamName, &ind); 
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetDHCPInfo(NULL, &Info) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = Info.IPAddress.Value;

        return TRUE;
    }

    rc =  strcmp_s( "SubnetMask",strlen("SubnetMask"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetDHCPInfo(NULL, &Info) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = Info.SubnetMask.Value;

        return TRUE;
    }

    rc =  strcmp_s("Gateway", strlen("Gateway"),ParamName, &ind); 
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetDHCPInfo(NULL, &Info) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = Info.Gateway.Value;

        return TRUE;
    }

    rc =  strcmp_s("TFTPServer", strlen("TFTPServer"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetDHCPInfo(NULL, &Info) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = Info.TFTPServer.Value;

        return TRUE;
    }

    rc =  strcmp_s( "TimeOffset",strlen("TimeOffset"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetDHCPInfo(NULL, &Info) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = Info.TimeOffset;

        return TRUE;
    }

    rc =  strcmp_s( "LeaseTimeRemaining",strlen("LeaseTimeRemaining"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetDHCPInfo(NULL, &Info) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = Info.LeaseTimeRemaining;

        return TRUE;
    }


    rc = strcmp_s( "IPv6LeaseTimeRemaining",strlen("IPv6LeaseTimeRemaining"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetIPv6DHCPInfo(NULL, &IPv6Info) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = IPv6Info.IPv6LeaseTimeRemaining;

        return TRUE;
    }

     rc =  strcmp_s( "IPv6RebindTimeRemaining",strlen("IPv6RebindTimeRemaining"),ParamName, &ind);
     ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetIPv6DHCPInfo(NULL, &IPv6Info) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = IPv6Info.IPv6RebindTimeRemaining;

        return TRUE;
    }

    rc =  strcmp_s( "IPv6RenewTimeRemaining",strlen("IPv6RenewTimeRemaining"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetIPv6DHCPInfo(NULL, &IPv6Info) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = IPv6Info.IPv6RenewTimeRemaining;

        return TRUE;
    }

       rc = strcmp_s( "LockedUpstreamChID", strlen("LockedUpstreamChID"),ParamName,&ind);
       ERR_CHK(rc);
       if((!ind) && (rc == EOK))
	{
		if (CosaDmlCMGetLockedUpstreamChID(NULL, puLong) != ANSC_STATUS_SUCCESS)
			return FALSE;

		return TRUE;
	}

	
	rc = strcmp_s( "StartDSFrequency",strlen("StartDSFrequency"),ParamName, &ind);
        ERR_CHK(rc);
        if((!ind) && (rc == EOK))
	{
		if (CosaDmlCMGetStartDSFrequency(NULL, puLong) != ANSC_STATUS_SUCCESS)
			return FALSE;

		return TRUE;
	}

	rc =  strcmp_s("CMResetCount",strlen("CMResetCount"),ParamName, &ind);
        ERR_CHK(rc);
        if((!ind) && (rc == EOK))
	{
		if (CosaDmlCMGetResetCount(NULL,CABLE_MODEM_RESET,puLong) != ANSC_STATUS_SUCCESS)
			return FALSE;

		return TRUE;
	}

	rc =  strcmp_s( "LocalResetCount",strlen("LocalResetCount"),ParamName, &ind);
        ERR_CHK(rc);
        if((!ind) && (rc == EOK))
	{
		if (CosaDmlCMGetResetCount(NULL,LOCAL_RESET, puLong) != ANSC_STATUS_SUCCESS)
			return FALSE;

		return TRUE;
	}

	rc =  strcmp_s( "DocsisResetCount",strlen("DocsisResetCount"),ParamName, &ind);
        ERR_CHK(rc);
        if((!ind) && (rc == EOK))
	{
		if (CosaDmlCMGetResetCount(NULL,DOCSIS_RESET,puLong) != ANSC_STATUS_SUCCESS)
			return FALSE;

		return TRUE;
	}

	rc = strcmp_s("ErouterResetCount",strlen("ErouterResetCount"),ParamName, &ind); 
        ERR_CHK(rc);
        if((!ind) && (rc == EOK))
	{
		if (CosaDmlCMGetResetCount(NULL,EROUTER_RESET,puLong) != ANSC_STATUS_SUCCESS)
			return FALSE;

		return TRUE;
	}

    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        X_CISCO_COM_CableModem_GetParamStringValue
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
X_CISCO_COM_CableModem_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    COSA_CM_DHCP_INFO               Info;
    memset(&Info, 0, sizeof(COSA_CM_DHCP_INFO));
    COSA_CM_DOCSIS_INFO             DInfo;
    memset(&DInfo, 0, sizeof(COSA_CM_DOCSIS_INFO));
    COSA_CM_IPV6DHCP_INFO           IPV6Info;
    memset(&IPV6Info, 0, sizeof(COSA_CM_IPV6DHCP_INFO));
    int ind                         = -1; 
    errno_t rc                      =     -1;
    

    /* check the parameter name and return the corresponding value */
    rc = strcmp_s( "MACAddress",strlen("MACAddress"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetDHCPInfo(NULL, &Info) != ANSC_STATUS_SUCCESS)
            return -1;

        rc = strcpy_s(pValue,*pUlSize,Info.MACAddress);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }
        

        return 0;
    }

    rc = strcmp_s("BootFileName",strlen("BootFileName"),ParamName ,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetDHCPInfo(NULL, &Info) != ANSC_STATUS_SUCCESS)
            return -1;

        rc = strcpy_s(pValue,*pUlSize,Info.BootFileName);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }
        

        return 0;
    }

    rc =  strcmp_s( "TimeServer",strlen("TimeServer"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetDHCPInfo(NULL, &Info) != ANSC_STATUS_SUCCESS)
            return -1;

        rc = strcpy_s(pValue,*pUlSize,Info.TimeServer);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }

        return 0;
    }

    rc = strcmp_s("RebindTimeRemaining",strlen("RebindTimeRemaining"),ParamName,&ind );
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetDHCPInfo(NULL, &Info) != ANSC_STATUS_SUCCESS)
            return -1;

        rc = strcpy_s(pValue,*pUlSize,Info.RebindTimeRemaining);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }


        return 0;
    }

    rc = strcmp_s("RenewTimeRemaining",strlen("RenewTimeRemaining"),ParamName,&ind); 
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetDHCPInfo(NULL, &Info) != ANSC_STATUS_SUCCESS)
            return -1;

        rc = strcpy_s(pValue,*pUlSize,Info.RenewTimeRemaining);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }


        return 0;
    }


    rc =  strcmp_s("DOCSISDHCPStatus", strlen("DOCSISDHCPStatus"),ParamName,&ind); 
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetDHCPInfo(NULL, &Info) != ANSC_STATUS_SUCCESS)
            return -1;

        rc = strcpy_s(pValue,*pUlSize,Info.DOCSISDHCPStatus);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }

        return 0;
    }

    rc = strcmp_s( "CMStatus",strlen("CMStatus"),ParamName, &ind); 
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
#ifdef ENABLE_RDK_WANMANAGER        
        if ( 0 == access( "/tmp/cmoff" , F_OK ) )
        {
            strcpy_s(pValue,*pUlSize,"CMOFF");
            return 0;
        }
#endif
        /* collect value */
        if (CosaDmlCMGetStatus(NULL, pValue) != ANSC_STATUS_SUCCESS)
            return -1;

        return 0;
    }

    rc = strcmp_s("DOCSISVersion",strlen("DOCSISVersion"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return -1;

        rc = strcpy_s(pValue,*pUlSize,DInfo.DOCSISVersion);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }


        return 0;
    }

    rc = strcmp_s("DOCSISDownstreamScanning",strlen("DOCSISDownstreamScanning"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return -1;

        rc = strcpy_s(pValue,*pUlSize,DInfo.DOCSISDownstreamScanning);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }


        return 0;
    }

    rc = strcmp_s("DOCSISDownstreamRanging",strlen("DOCSISDownstreamRanging"), ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return -1;

        rc = strcpy_s(pValue,*pUlSize,DInfo.DOCSISDownstreamRanging);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }


        return 0;
    }

    rc =  strcmp_s("DOCSISUpstreamScanning",strlen("DOCSISUpstreamScanning"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return -1;

        rc = strcpy_s(pValue,*pUlSize,DInfo.DOCSISUpstreamScanning);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }


        return 0;
    }

    rc = strcmp_s("DOCSISUpstreamRanging", strlen("DOCSISUpstreamRanging"),ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return -1;

        rc = strcpy_s(pValue,*pUlSize,DInfo.DOCSISUpstreamRanging);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }


        return 0;
    }

    rc =  strcmp_s( "DOCSISTftpStatus",strlen("DOCSISTftpStatus"),ParamName,&ind); 
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return -1;

        rc = strcpy_s(pValue,*pUlSize,DInfo.DOCSISTftpStatus);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }


        return 0;
    }

    rc = strcmp_s("DOCSISDataRegComplete", strlen("DOCSISDataRegComplete"),ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return -1;

        rc = strcpy_s(pValue,*pUlSize,DInfo.DOCSISDataRegComplete);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }


        return 0;
    }

    rc =  strcmp_s( "CoreVersion", strlen("CoreVersion"),ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return -1;

        rc = strcpy_s(pValue,*pUlSize,DInfo.CoreVersion);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }


        return 0;
    }

    rc = strcmp_s("DOCSISConfigFileName",strlen("DOCSISConfigFileName"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return -1;

        rc = strcpy_s(pValue,*pUlSize,DInfo.DOCSISConfigFileName);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }


        return 0;
    }

    rc = strcmp_s("ToDStatus",strlen("ToDStatus"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return -1;

        rc = strcpy_s(pValue,*pUlSize,DInfo.ToDStatus);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }


        return 0;
    }

    rc =  strcmp_s("UpstreamServiceFlowParams",strlen("UpstreamServiceFlowParams"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return -1;

        rc = strcpy_s(pValue,*pUlSize,DInfo.UpstreamServiceFlowParams);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }

        return 0;
    }

    rc = strcmp_s( "DownstreamServiceFlowParams",strlen("DownstreamServiceFlowParams"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return -1;

        rc = strcpy_s(pValue,*pUlSize,DInfo.DownstreamServiceFlowParams);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }

        return 0;
    }

    rc =  strcmp_s("DOCSISDownstreamDataRate",strlen("DOCSISDownstreamDataRate"),ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return -1;

        rc = strcpy_s(pValue,*pUlSize,DInfo.DOCSISDownstreamDataRate);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }

        return 0;
    }

    rc = strcmp_s("DOCSISUpstreamDataRate",strlen("DOCSISUpstreamDataRate"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return -1;

        rc = strcpy_s(pValue,*pUlSize,DInfo.DOCSISUpstreamDataRate);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }

        return 0;
    }

    rc = strcmp_s( "LoopDiagnosticsDetails",strlen("LoopDiagnosticsDetails"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetLoopDiagnosticsDetails(NULL, pValue) != ANSC_STATUS_SUCCESS)
            return -1;

        return 0;
    }

    rc = strcmp_s("Market",strlen("Market"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetMarket(NULL, pValue) != ANSC_STATUS_SUCCESS)
            return -1;

        return 0;
    }

    rc = strcmp_s("MDDIPOverride",strlen("MDDIPOverride"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetMDDIPOverride(NULL, pValue) != ANSC_STATUS_SUCCESS)
            return -1;

        return 0;
    }


    rc =  strcmp_s( "IPv6Address",strlen("IPv6Address"),ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetIPv6DHCPInfo(NULL, &IPV6Info) != ANSC_STATUS_SUCCESS)
            return -1;

        rc = strcpy_s(pValue,*pUlSize,IPV6Info.IPv6Address);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }

        return 0;
    }
    
    rc = strcmp_s("IPv6BootFileName",strlen("IPv6BootFileName"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetIPv6DHCPInfo(NULL, &IPV6Info) != ANSC_STATUS_SUCCESS)
            return -1;

        rc = strcpy_s(pValue,*pUlSize,IPV6Info.IPv6BootFileName);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }

        return 0;
    }

    rc =  strcmp_s("IPv6Prefix",strlen("IPv6Prefix"),ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetIPv6DHCPInfo(NULL, &IPV6Info) != ANSC_STATUS_SUCCESS)
            return -1;

        rc = strcpy_s(pValue,*pUlSize,IPV6Info.IPv6Prefix);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }

        return 0;
    }

    rc =  strcmp_s("IPv6Router",strlen("IPv6Router"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetIPv6DHCPInfo(NULL, &IPV6Info) != ANSC_STATUS_SUCCESS)
            return -1;

        rc = strcpy_s(pValue,*pUlSize,IPV6Info.IPv6Router);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }

        return 0;
    }

    rc =  strcmp_s("IPv6TFTPServer",strlen("IPv6TFTPServer"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetIPv6DHCPInfo(NULL, &IPV6Info) != ANSC_STATUS_SUCCESS)
            return -1;

        rc = strcpy_s(pValue,*pUlSize,IPV6Info.IPv6TFTPServer);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }

        return 0;
    }

    rc =  strcmp_s("IPv6TimeServer",strlen("IPv6TimeServer"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetIPv6DHCPInfo(NULL, &IPV6Info) != ANSC_STATUS_SUCCESS)
            return -1;

        rc = strcpy_s(pValue,*pUlSize,IPV6Info.IPv6TimeServer);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }

        return 0;
    }

    rc = strcmp_s("TelephonyDHCPStatus",strlen("TelephonyDHCPStatus"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetTelephonyDHCPStatus(NULL, pValue) != ANSC_STATUS_SUCCESS)
            return -1;

        return 0;
    }

    rc = strcmp_s("TelephonyTftpStatus",strlen("TelephonyTftpStatus"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetTelephonyTftpStatus(NULL, pValue) != ANSC_STATUS_SUCCESS)
            return -1;

        return 0;
    }

    rc =  strcmp_s("TelephonyRegistrationStatus",strlen("TelephonyRegistrationStatus"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCMGetTelephonyRegistrationStatus(NULL, pValue) != ANSC_STATUS_SUCCESS)
            return -1;

        return 0;
    }
    rc =  strcmp_s("DOCSISCertificate",strlen("DOCSISCertificate"), ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if (CosaDmlCmGetCMCert(NULL, pValue) != ANSC_STATUS_SUCCESS)
            return -1;

        return 0;
    }

    rc = strcmp_s("ProvIpType",strlen("ProvIpType"),ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
		/* collect value */
        if (CosaDmlCMGetProvType(NULL, pValue) != ANSC_STATUS_SUCCESS)
            return -1;

        return 0;
    }
    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_CableModem_SetParamBoolValue
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
X_CISCO_COM_CableModem_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)g_pCosaBEManager->hCM;
    PCOSA_DML_CM_LOG                pCfg      = &pMyObject->CmLog;
#if defined (FEATURE_RDKB_WAN_MANAGER)
    PCOSA_DML_CM_WANCFG             pWanCfg      = &pMyObject->CmWanCfg;
#endif
    errno_t                         rc               = -1;
    int                             ind              = -1;

    /* check the parameter name and set the corresponding value */
    rc = strcmp_s("LoopDiagnosticsStart",strlen("LoopDiagnosticsStart"), ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        pMyObject->LoopDiagnosticsStart = bValue;
        return TRUE;
    }

    /* check the parameter name and set the corresponding value */
    rc = strcmp_s( "EnableLog",strlen("EnableLog"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* save update to backup */
        pCfg->EnableLog = bValue;
        return TRUE;
    }

    rc =  strcmp_s( "CleanDocsisLog",strlen("CleanDocsisLog"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* save update to backup */
        pCfg->CleanDocsisLog = bValue;
        return TRUE;
    }
#if defined (FEATURE_RDKB_WAN_MANAGER)
    rc =  strcmp_s( "ConfigureWan",strlen("ConfigureWan"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* save update to backup */
        pWanCfg->ConfigureWan = bValue;
        return TRUE;
    }

    rc =  strcmp_s( "CustomWanConfigUpdate",strlen("CustomWanConfigUpdate"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        pWanCfg->CustomWanConfigUpdate = bValue;
        CosaDmlCMWanUpdateCustomConfig(pMyObject,bValue);
        return TRUE;
    }
#endif
    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_CableModem_SetParamIntValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                int                         iValue
            );

    description:

        This function is called to set integer parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int                         iValue
                The updated integer value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
X_CISCO_COM_CableModem_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         iValue
    )
{
    /* check the parameter name and set the corresponding value */
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(iValue);

    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_CableModem_SetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG                       uValue
            );

    description:

        This function is called to set ULONG parameter value; 

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uValue
                The updated ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL
X_CISCO_COM_CableModem_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    /* check the parameter name and set the corresponding value */
    UNREFERENCED_PARAMETER(hInsContext);
    errno_t                         rc               = -1;
    int                             ind              = -1;
    rc = strcmp_s( "LockedUpstreamChID",strlen( "LockedUpstreamChID"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
    	CosaDmlCMSetLockedUpstreamChID(NULL, uValue);
        return TRUE;
    }
	
	/* check the parameter name and set the corresponding value */
	rc =  strcmp_s("StartDSFrequency",strlen("StartDSFrequency"),ParamName, &ind);
        ERR_CHK(rc);
      if((!ind) && (rc == EOK))
    {
    	CosaDmlCMSetStartDSFrequency(NULL, uValue);
        return TRUE;
    }

    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_CableModem_SetParamStringValue
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
X_CISCO_COM_CableModem_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)g_pCosaBEManager->hCM;
#if defined (FEATURE_RDKB_WAN_MANAGER)
    PCOSA_DML_CM_WANCFG             pWanCfg      = &pMyObject->CmWanCfg;
#endif
    errno_t        rc = -1;
    int ind = -1;

    /* check the parameter name and set the corresponding value */
    rc =  strcmp_s("MDDIPOverride",strlen("MDDIPOverride") ,ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    { 
        rc = strcpy_s(pMyObject->MDDIPOverride,sizeof(pMyObject->MDDIPOverride),pString);
        if(rc != EOK)
        {
             ERR_CHK(rc);
             return FALSE;
        }
        return TRUE;
    }
#if defined (FEATURE_RDKB_WAN_MANAGER)
    rc =  strcmp_s( "RequestPhyStatus",strlen("RequestPhyStatus"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))    
    {
        rc = strcpy_s(pWanCfg->wanInstanceNumber,sizeof(pWanCfg->wanInstanceNumber),pString);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }

        /* save update to backup */
        if (pWanCfg->MonitorPhyStatusAndNotify != TRUE)
        {
             pWanCfg->MonitorPhyStatusAndNotify = TRUE;
            CosaDmlCMWanMonitorPhyStatusAndNotify(pMyObject);
        }
        return TRUE;
    }

    rc =  strcmp_s( "RequestOperationalStatus",strlen("RequestOperationalStatus"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* save update to backup */
        if (pWanCfg->MonitorOperStatusAndNotify  != TRUE)
        {
            rc = strcpy_s(pWanCfg->wanInstanceNumber,sizeof(pWanCfg->wanInstanceNumber),pString);
            if(rc != EOK)
            {
                ERR_CHK(rc);
                return FALSE;
            }

            pWanCfg->MonitorOperStatusAndNotify = TRUE;
            CosaDmlCMWanMonitorOperStatusAndNotify(pMyObject);
        }
        return TRUE;
    }
    rc =  strcmp_s( "PostCfgWanFinalize",strlen("PostCfgWanFinalize"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        if ( 0 != access( "/tmp/autowan_iface_finalized" , F_OK ) )
        {
            system("touch /tmp/autowan_iface_finalized"); 
        }
        return TRUE;
    }
#endif
    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        X_CISCO_COM_CableModem_Validate
            (
                ANSC_HANDLE                 hInsContext,
                char*                       pReturnParamName,
                ULONG*                      puLength
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       pReturnParamName,
                The buffer (128 bytes) of parameter name if there's a validation. 

                ULONG*                      puLength
                The output length of the param name. 

    return:     TRUE if there's no validation.

**********************************************************************/
BOOL
X_CISCO_COM_CableModem_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(pReturnParamName);
    UNREFERENCED_PARAMETER(puLength);
    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        X_CISCO_COM_CableModem_Commit
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
X_CISCO_COM_CableModem_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    ANSC_STATUS                     returnStatus  = ANSC_STATUS_SUCCESS;        
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)g_pCosaBEManager->hCM;
    PCOSA_DML_CM_LOG                pCfg      = &pMyObject->CmLog;

    if(pCfg->CleanDocsisLog == 1)
    {
        CosaDmlCmSetLog(NULL,pCfg);
        g_DocsisLog_clean_flg = 1;
        return 0;
    }

    CosaDmlCMSetMDDIPOverride(NULL, pMyObject->MDDIPOverride);

    returnStatus = CosaDmlCMSetLoopDiagnosticsStart(NULL, pMyObject->LoopDiagnosticsStart);
    
    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        CosaDmlCmSetLog(NULL, pCfg);

        return 0;
    }
    else
    {
        return -1;
    }    
}


/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        X_CISCO_COM_CableModem_Rollback
            (
                ANSC_HANDLE                 hInsContext
            );

    description:

        This function is called to roll back the update whenever there's a 
        validation found.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG
X_CISCO_COM_CableModem_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    ANSC_STATUS                     reStatus  = ANSC_STATUS_SUCCESS;     
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)g_pCosaBEManager->hCM;
    PCOSA_DML_CM_LOG                pCfg      = &pMyObject->CmLog;

    reStatus = CosaDmlCMGetLoopDiagnosticsStart(NULL, &pMyObject->LoopDiagnosticsStart);
    
    if ( reStatus == ANSC_STATUS_SUCCESS )
    {        
        CosaDmlCmGetLog(NULL, pCfg);

        return 0;
    }
    else
    {
        return -1;
    }
}


/***********************************************************************

 APIs for Object:

    Device.X_CISCO_COM_CableModem.CMErrorCodewords.

    *  CMErrorCodewords_GetEntryCount
    *  CMErrorCodewords_GetEntry
    *  CMErrorCodewords_IsUpdated
    *  CMErrorCodewords_Synchronize
    *  CMErrorCodewords_GetParamUlongValue
    *  CMErrorCodewords_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        CMErrorCodewords_GetEntryCount
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
CMErrorCodewords_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)g_pCosaBEManager->hCM;
    
    return pMyObject->CMErrorCodewordsNumber;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        CMErrorCodewords_GetEntry
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
CMErrorCodewords_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)g_pCosaBEManager->hCM;

    if (nIndex < pMyObject->CMErrorCodewordsNumber)
    {
        *pInsNumber  = nIndex + 1;

        return &pMyObject->pCMErrorCodewords[nIndex];
    }

    return NULL; /* return the handle */
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        CMErrorCodewords_IsUpdated
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
CMErrorCodewords_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)g_pCosaBEManager->hCM;

    if ( !pMyObject->CMErrorCodewordsUpdateTime ) 
    {
        pMyObject->CMErrorCodewordsUpdateTime = AnscGetTickInSeconds();

        return TRUE;
    }
    
    if ( pMyObject->CMErrorCodewordsUpdateTime >= TIME_NO_NEGATIVE(AnscGetTickInSeconds() - CM_REFRESH_INTERVAL) )
    {
        return FALSE;
    }
    else 
    {
        pMyObject->CMErrorCodewordsUpdateTime = AnscGetTickInSeconds();

        return TRUE;
    }
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        CMErrorCodewords_Synchronize
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
CMErrorCodewords_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)g_pCosaBEManager->hCM;
    ANSC_STATUS                     ret           = ANSC_STATUS_SUCCESS;

    if ( pMyObject->pCMErrorCodewords )
    {
        AnscFreeMemory(pMyObject->pCMErrorCodewords);
        pMyObject->pCMErrorCodewords = NULL;
    }
    
    pMyObject->CMErrorCodewordsNumber = 0;
    
    ret = CosaDmlCmGetCMErrorCodewords
        (
            (ANSC_HANDLE)NULL, 
            &pMyObject->CMErrorCodewordsNumber,
            &pMyObject->pCMErrorCodewords
        );

    if ( ret != ANSC_STATUS_SUCCESS )
    {
        pMyObject->pCMErrorCodewords = NULL;
        pMyObject->CMErrorCodewordsNumber = 0;
    }
    
    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        CMErrorCodewords_GetParamUlongValue
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
CMErrorCodewords_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DML_CMERRORCODEWORDS_FULL        pConf        = (PCOSA_DML_CMERRORCODEWORDS_FULL)hInsContext;
    errno_t rc = -1;
    int ind = -1;
    
    /* check the parameter name and return the corresponding value */
    rc = strcmp_s( "UnerroredCodewords",strlen("UnerroredCodewords"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *puLong = pConf->UnerroredCodewords; 

        return TRUE;
    }

    rc =  strcmp_s("CorrectableCodewords",strlen("CorrectableCodewords"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))

    {
        /* collect value */
        *puLong = pConf->CorrectableCodewords; 

        return TRUE;
    }

    rc =  strcmp_s( "UncorrectableCodewords",strlen("UncorrectableCodewords"),ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *puLong = pConf->UncorrectableCodewords; 

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        CMErrorCodewords_GetParamStringValue
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
CMErrorCodewords_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{

    UNREFERENCED_PARAMETER(hInsContext);
    UNREFERENCED_PARAMETER(ParamName);
    UNREFERENCED_PARAMETER(pValue);
    UNREFERENCED_PARAMETER(pUlSize);
    /* check the parameter name and return the corresponding value */    

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/***********************************************************************

 APIs for Object:

    X_CISCO_COM_MLD.Group.{i}.

    *  DocsisLog_GetEntryCount
    *  DocsisLog_GetEntry
    *  DocsisLog_IsUpdated
    *  DocsisLog_Synchronize
    *  DocsisLog_GetParamUlongValue
    *  DocsisLog_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        DocsisLog_GetEntryCount
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
DocsisLog_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)g_pCosaBEManager->hCM;
    
    return pMyObject->DocsisLogNumber;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        DocsisLog_GetEntry
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
DocsisLog_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)g_pCosaBEManager->hCM;

    if (nIndex < pMyObject->DocsisLogNumber)
    {
        *pInsNumber  = nIndex + 1;

        return &pMyObject->pDocsisLog[nIndex];
    }

    return NULL; /* return the handle */
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DocsisLog_IsUpdated
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
DocsisLog_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)g_pCosaBEManager->hCM;

    if ( !pMyObject->DocsisLogUpdateTime || g_DocsisLog_clean_flg == 1 ) 
    {
        pMyObject->DocsisLogUpdateTime = AnscGetTickInSeconds();
        g_DocsisLog_clean_flg = 0;
        return TRUE;
    }
    
    if ( pMyObject->DocsisLogUpdateTime >= TIME_NO_NEGATIVE(AnscGetTickInSeconds() - CM_REFRESH_INTERVAL) )
    {
        return FALSE;
    }
    else 
    {
        pMyObject->DocsisLogUpdateTime = AnscGetTickInSeconds();

        return TRUE;
    }
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        DocsisLog_Synchronize
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
DocsisLog_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)g_pCosaBEManager->hCM;
    ANSC_STATUS                     ret           = ANSC_STATUS_SUCCESS;

    if ( pMyObject->pDocsisLog )
    {
        AnscFreeMemory(pMyObject->pDocsisLog);
        pMyObject->pDocsisLog = NULL;
    }
    
    pMyObject->DocsisLogNumber = 0;
    
    ret = CosaDmlCmGetDocsisLog
        (
            (ANSC_HANDLE)NULL, 
            &pMyObject->DocsisLogNumber,
            &pMyObject->pDocsisLog
        );

    if ( ret != ANSC_STATUS_SUCCESS )
    {
        pMyObject->pDocsisLog = NULL;
        pMyObject->DocsisLogNumber = 0;
    }
    
    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DocsisLog_GetParamUlongValue
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
DocsisLog_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_DML_DOCSISLOG_FULL        pConf        = (PCOSA_DML_DOCSISLOG_FULL)hInsContext;
    errno_t rc = -1;
    int ind = -1;
    
    /* check the parameter name and return the corresponding value */
    rc = strcmp_s( "Index",strlen("Index"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *puLong = pConf->Index; 

        return TRUE;
    }

    rc = strcmp_s( "EventID",strlen("EventID"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *puLong = pConf->EventID; 

        return TRUE;
    }

    rc = strcmp_s("EventLevel",strlen("EventLevel"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *puLong = pConf->EventLevel; 

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        DocsisLog_GetParamStringValue
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
DocsisLog_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_DML_DOCSISLOG_FULL        pConf        = (PCOSA_DML_DOCSISLOG_FULL)hInsContext;
    errno_t        rc = -1;
    int ind = -1;
    
    /* check the parameter name and return the corresponding value */    
    rc =  strcmp_s("Description",strlen("Description"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))

    {
        /* collect value */
        if ( _ansc_strlen(pConf->Description) >= *pUlSize )
        {
            *pUlSize = _ansc_strlen(pConf->Description);
            return 1;
        }
        
        rc = strcpy_s(pValue,*pUlSize , pConf->Description);
         if(rc != EOK)
         {
             ERR_CHK(rc);
             return -1;
         }
        return 0;
    }

    rc = strcmp_s( "Time",strlen("Time"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))

    {
        /* collect value */
        if ( _ansc_strlen(pConf->Time) >= *pUlSize )
        {
            *pUlSize = _ansc_strlen(pConf->Time);
            return 1;
        }
        
        rc = strcpy_s(pValue,*pUlSize, pConf->Time);
         if(rc != EOK)
         {
             ERR_CHK(rc);
             return -1;
         }
        
        return 0;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/***********************************************************************

 APIs for Object:

    X_CISCO_COM_CableModem.DownstreamChannel.{i}.

    *  DownstreamChannel_GetEntryCount
    *  DownstreamChannel_GetEntry
    *  DownstreamChannel_IsUpdated
    *  DownstreamChannel_Synchronize
    *  DownstreamChannel_GetParamUlongValue
    *  DownstreamChannel_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        DownstreamChannel_GetEntryCount
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
DownstreamChannel_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)g_pCosaBEManager->hCM;
    
    return pMyObject->DownstreamChannelNumber;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        DownstreamChannel_GetEntry
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
DownstreamChannel_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)g_pCosaBEManager->hCM;

    if ((nIndex < pMyObject->DownstreamChannelNumber) && (pMyObject->pDownstreamChannel != NULL))
    {
        *pInsNumber  = nIndex + 1;

        return &pMyObject->pDownstreamChannel[nIndex];
    }

    return NULL; /* return the handle */
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DownstreamChannel_IsUpdated
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
DownstreamChannel_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)g_pCosaBEManager->hCM;

    if ( !pMyObject->DownstreamChannelUpdateTime ) 
    {
        pMyObject->DownstreamChannelUpdateTime = AnscGetTickInSeconds();

        return TRUE;
    }
    
    if ( pMyObject->DownstreamChannelUpdateTime >= TIME_NO_NEGATIVE(AnscGetTickInSeconds() - CM_REFRESH_INTERVAL) )
    {
        return FALSE;
    }
    else 
    {
        pMyObject->DownstreamChannelUpdateTime = AnscGetTickInSeconds();

        return TRUE;
    }
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        DownstreamChannel_Synchronize
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
DownstreamChannel_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)g_pCosaBEManager->hCM;
    ANSC_STATUS                     ret       = ANSC_STATUS_SUCCESS;

    Ccsp_cm_clnt_lock();
    if ( pMyObject->pDownstreamChannel )
    {
        AnscFreeMemory(pMyObject->pDownstreamChannel);
        pMyObject->pDownstreamChannel = NULL;
    }
    
    pMyObject->DownstreamChannelNumber = 0;
    
    ret = CosaDmlCmGetDownstreamChannel
        (
            (ANSC_HANDLE)NULL, 
            &pMyObject->DownstreamChannelNumber,
            &pMyObject->pDownstreamChannel
        );

    if ( ret != ANSC_STATUS_SUCCESS )
    {
        pMyObject->pDownstreamChannel = NULL;
        pMyObject->DownstreamChannelNumber = 0;
    }
    
    Ccsp_cm_clnt_unlock();
    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        DownstreamChannel_GetParamUlongValue
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
DownstreamChannel_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CM_DS_CHANNEL             pConf        = (PCOSA_CM_DS_CHANNEL)hInsContext;
    errno_t rc = -1;
    int ind =-1;
    
    /* check the parameter name and return the corresponding value */
    rc =  strcmp_s("ChannelID",strlen("ChannelID"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *puLong = pConf->ChannelID;

        return TRUE;
    }

    rc =  strcmp_s("Octets",strlen("Octets"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *puLong = pConf->Octets;

        return TRUE;
    }

    rc = strcmp_s("Correcteds", strlen("Correcteds"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))

    {
        /* collect value */
        *puLong = pConf->Correcteds;

        return TRUE;
    }

    rc =  strcmp_s("Uncorrectables",strlen("Uncorrectables"),ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        *puLong = pConf->Uncorrectables;

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        DownstreamChannel_GetParamStringValue
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
DownstreamChannel_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_CM_DS_CHANNEL             pConf        = (PCOSA_CM_DS_CHANNEL)hInsContext;
    errno_t        rc = -1;
    int ind = -1;    
    /* check the parameter name and return the corresponding value */    
    rc = strcmp_s( "Frequency",strlen("Frequency"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))

    {
        /* collect value */
        if ( _ansc_strlen(pConf->Frequency) >= *pUlSize )
        {
            *pUlSize = _ansc_strlen(pConf->Frequency);
            return 1;
        }
        
        rc = strcpy_s(pValue,*pUlSize, pConf->Frequency);
         if(rc != EOK)
         {
             ERR_CHK(rc);
             return -1;
         }

        return 0;
    }

    rc = strcmp_s("PowerLevel",strlen("PowerLevel"),ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))

    {
        /* collect value */
        if ( _ansc_strlen(pConf->PowerLevel) >= *pUlSize )
        {
            *pUlSize = _ansc_strlen(pConf->PowerLevel);
            return 1;
        }
        
        rc = strcpy_s(pValue,*pUlSize , pConf->PowerLevel);
         if(rc != EOK)
         {
             ERR_CHK(rc);
             return -1;
         }

        return 0;
    }

    rc = strcmp_s("SNRLevel",strlen("SNRLevel"),ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if ( _ansc_strlen(pConf->SNRLevel) >= *pUlSize )
        {
            *pUlSize = _ansc_strlen(pConf->SNRLevel);
            return 1;
        }
        
        rc = strcpy_s(pValue,*pUlSize , pConf->SNRLevel);
         if(rc != EOK)
         {
             ERR_CHK(rc);
             return -1;
         }

        return 0;
    }

    rc = strcmp_s("Modulation", strlen("Modulation"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))
    {
        /* collect value */
        if ( _ansc_strlen(pConf->Modulation) >= *pUlSize )
        {
            *pUlSize = _ansc_strlen(pConf->Modulation);
            return 1;
        }
        
        rc = strcpy_s(pValue,*pUlSize , pConf->Modulation);
         if(rc != EOK)
         {
             ERR_CHK(rc);
             return -1;
         }

        return 0;
    }

    rc =  strcmp_s( "LockStatus",strlen( "LockStatus") ,ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))

    {
        /* collect value */
        if ( _ansc_strlen(pConf->LockStatus) >= *pUlSize )
        {
            *pUlSize = _ansc_strlen(pConf->LockStatus);
            return 1;
        }
        
        rc = strcpy_s(pValue,*pUlSize , pConf->LockStatus);
         if(rc != EOK)
         {
             ERR_CHK(rc);
             return -1;
         }

        return 0;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/***********************************************************************

 APIs for Object:

    X_CISCO_COM_CableModem.UpstreamChannel.{i}.

    *  UpstreamChannel_GetEntryCount
    *  UpstreamChannel_GetEntry
    *  UpstreamChannel_IsUpdated
    *  UpstreamChannel_Synchronize
    *  UpstreamChannel_GetParamUlongValue
    *  UpstreamChannel_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        UpstreamChannel_GetEntryCount
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
UpstreamChannel_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)g_pCosaBEManager->hCM;
    
    return pMyObject->UpstreamChannelNumber;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        UpstreamChannel_GetEntry
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
UpstreamChannel_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)g_pCosaBEManager->hCM;

    if ((nIndex < pMyObject->UpstreamChannelNumber) && (pMyObject->pUpstreamChannel != NULL))
    {
        *pInsNumber  = nIndex + 1;

        return &pMyObject->pUpstreamChannel[nIndex];
    }

    return NULL; /* return the handle */
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        UpstreamChannel_IsUpdated
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
UpstreamChannel_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)g_pCosaBEManager->hCM;

    if ( !pMyObject->UpstreamChannelUpdateTime ) 
    {
        pMyObject->UpstreamChannelUpdateTime = AnscGetTickInSeconds();

        return TRUE;
    }
    
    if ( pMyObject->UpstreamChannelUpdateTime >= TIME_NO_NEGATIVE(AnscGetTickInSeconds() - CM_REFRESH_INTERVAL) )
    {
        return FALSE;
    }
    else 
    {
        pMyObject->UpstreamChannelUpdateTime = AnscGetTickInSeconds();

        return TRUE;
    }
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        UpstreamChannel_Synchronize
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
UpstreamChannel_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)g_pCosaBEManager->hCM;
    ANSC_STATUS                     ret       = ANSC_STATUS_SUCCESS;

    Ccsp_cm_clnt_lock();
    if ( pMyObject->pUpstreamChannel )
    {
        AnscFreeMemory(pMyObject->pUpstreamChannel);
        pMyObject->pUpstreamChannel = NULL;
    }
    
    pMyObject->UpstreamChannelNumber = 0;
    
    ret = CosaDmlCmGetUpstreamChannel
        (
            (ANSC_HANDLE)NULL, 
            &pMyObject->UpstreamChannelNumber,
            &pMyObject->pUpstreamChannel
        );

    if ( ret != ANSC_STATUS_SUCCESS )
    {
        pMyObject->pUpstreamChannel = NULL;
        pMyObject->UpstreamChannelNumber = 0;
    }
    
    Ccsp_cm_clnt_unlock();
    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        UpstreamChannel_GetParamUlongValue
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
UpstreamChannel_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CM_US_CHANNEL             pConf        = (PCOSA_CM_US_CHANNEL)hInsContext;
    errno_t rc = -1;
    int ind =-1;
    
    /* check the parameter name and return the corresponding value */
    rc = strcmp_s("ChannelID",strlen("ChannelID"), ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))

    {
        /* collect value */
        *puLong = pConf->ChannelID;

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        UpstreamChannel_GetParamStringValue
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
UpstreamChannel_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_CM_US_CHANNEL             pConf        = (PCOSA_CM_US_CHANNEL)hInsContext;
    errno_t        rc = -1;
    int ind = -1;    
    /* check the parameter name and return the corresponding value */    
    rc = strcmp_s( "Frequency",strlen("Frequency"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))

    {
        /* collect value */
        if ( _ansc_strlen(pConf->Frequency) >= *pUlSize )
        {
            *pUlSize = _ansc_strlen(pConf->Frequency);
            return 1;
        }
        
         rc = strcpy_s(pValue,*pUlSize , pConf->Frequency);
         if(rc != EOK)
         {
             ERR_CHK(rc);
             return -1;
         }

        return 0;
    }

    rc = strcmp_s( "PowerLevel",strlen("PowerLevel"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))

    {
        /* collect value */
        if ( _ansc_strlen(pConf->PowerLevel) >= *pUlSize )
        {
            *pUlSize = _ansc_strlen(pConf->PowerLevel);
            return 1;
        }
        
        rc = strcpy_s(pValue,*pUlSize , pConf->PowerLevel);
         if(rc != EOK)
         {
             ERR_CHK(rc);
             return -1;
         }

        return 0;
    }

    rc = strcmp_s( "ChannelType",strlen("ChannelType"), ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))

    {
        /* collect value */
        if ( _ansc_strlen(pConf->ChannelType) >= *pUlSize )
        {
            *pUlSize = _ansc_strlen(pConf->ChannelType);
            return 1;
        }
        
        rc = strcpy_s(pValue,*pUlSize , pConf->ChannelType);
         if(rc != EOK)
         {
             ERR_CHK(rc);
             return -1;
         }

        return 0;
    }

    rc = strcmp_s("SymbolRate", strlen("SymbolRate"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))

    {
        /* collect value */
        if ( _ansc_strlen(pConf->SymbolRate) >= *pUlSize )
        {
            *pUlSize = _ansc_strlen(pConf->SymbolRate);
            return 1;
        }
        
         rc = strcpy_s(pValue,*pUlSize , pConf->SymbolRate);
         if(rc != EOK)
         {
             ERR_CHK(rc);
             return -1;
         }

        return 0;
    }

    rc = strcmp_s("Modulation",strlen("Modulation"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))

    {
        /* collect value */
        if ( _ansc_strlen(pConf->Modulation) >= *pUlSize )
        {
            *pUlSize = _ansc_strlen(pConf->Modulation);
            return 1;
        }
        
        rc = strcpy_s(pValue,*pUlSize , pConf->Modulation);
         if(rc != EOK)
         {
             ERR_CHK(rc);
             return -1;
         }

        return 0;
    }

    rc =  strcmp_s( "LockStatus",strlen("LockStatus"),ParamName, &ind);
     ERR_CHK(rc);
    if((!ind) && (rc == EOK))

    {
        /* collect value */
        if ( _ansc_strlen(pConf->LockStatus) >= *pUlSize )
        {
            *pUlSize = _ansc_strlen(pConf->LockStatus);
            return 1;
        }
        
        rc = strcpy_s(pValue,*pUlSize , pConf->LockStatus);
         if(rc != EOK)
         {
             ERR_CHK(rc);
             return -1;
         }

        return 0;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

/***********************************************************************

 APIs for Object:

    X_CISCO_COM_CableModem.CPEList.{i}.

    *  CPEList_GetEntryCount
    *  CPEList_GetEntry
    *  CPEList_IsUpdated
    *  CPEList_Synchronize
    *  CPEList_GetParamStringValue

***********************************************************************/
/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        CPEList_GetEntryCount
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
CPEList_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)g_pCosaBEManager->hCM;
    
    return pMyObject->CPEListNumber;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ANSC_HANDLE
        CPEList_GetEntry
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
CPEList_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)g_pCosaBEManager->hCM;

    if (nIndex < pMyObject->CPEListNumber)
    {
        *pInsNumber  = nIndex + 1;

        return &pMyObject->pCPEList[nIndex];
    }

    return NULL; /* return the handle */
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL
        CPEList_IsUpdated
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
CPEList_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)g_pCosaBEManager->hCM;

    if ( !pMyObject->CPEListUpdateTime ) 
    {
        pMyObject->CPEListUpdateTime = AnscGetTickInSeconds();

        return TRUE;
    }
    
    if ( pMyObject->CPEListUpdateTime >= TIME_NO_NEGATIVE(AnscGetTickInSeconds() - CM_REFRESH_INTERVAL) )
    {
        return FALSE;
    }
    else 
    {
        pMyObject->CPEListUpdateTime = AnscGetTickInSeconds();

        return TRUE;
    }
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        CPEList_Synchronize
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
CPEList_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{
    UNREFERENCED_PARAMETER(hInsContext);
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)g_pCosaBEManager->hCM;
    ANSC_STATUS                     ret       = ANSC_STATUS_SUCCESS;

    pMyObject->CPEListNumber = 0;
    ret = CosaDmlCmGetCPEList
        (
         (ANSC_HANDLE)NULL, 
         &pMyObject->CPEListNumber,
         &pMyObject->pCPEList
        );

    if ( ret != ANSC_STATUS_SUCCESS )
    {
        pMyObject->pCPEList = NULL;
        pMyObject->CPEListNumber = 0;
    }

    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        ULONG
        CPEList_GetParamStringValue
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
CPEList_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_DML_CPE_LIST             pConf        = (PCOSA_DML_CPE_LIST)hInsContext;
    errno_t        rc = -1;
    int ind = -1;
    
    /* check the parameter name and return the corresponding value */    
    rc = strcmp_s( "IPAddress",strlen("IPAddress"),ParamName, &ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))

    {
        /* collect value */
        if ( _ansc_strlen(pConf->IPAddress) >= *pUlSize )
        {
            *pUlSize = _ansc_strlen(pConf->IPAddress);
            return 1;
        }
        
        rc = strcpy_s(pValue,*pUlSize,pConf->IPAddress);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }

        return 0;
    }

    rc = strcmp_s( "MACAddress", strlen("MACAddress"),ParamName,&ind);
    ERR_CHK(rc);
    if((!ind) && (rc == EOK))

    {
        /* collect value */
        if ( _ansc_strlen(pConf->MACAddress) >= *pUlSize )
        {
            *pUlSize = _ansc_strlen(pConf->MACAddress);
            return 1;
        }
        
         rc = strcpy_s(pValue,*pUlSize,pConf->MACAddress);
        if(rc != EOK)
        {
           ERR_CHK(rc);
           return -1;
        }

        return 0;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

