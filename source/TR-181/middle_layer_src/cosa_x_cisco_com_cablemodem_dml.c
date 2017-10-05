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

    module: cosa_x_cisco_com_cablemodem_dml.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    copyright:

        Cisco Systems, Inc.
        All Rights Reserved.

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
    PCOSA_DATAMODEL_CABLEMODEM     pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)g_pCosaBEManager->hCM;
    PCOSA_DML_CM_LOG               pCfg      = &pMyObject->CmLog;

    COSA_CM_DOCSIS_INFO            DInfo= {0};

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "BPIState", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *pBool = DInfo.BPIState;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "NetworkAccess", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *pBool = DInfo.NetworkAccess;

        return TRUE;
    }

    if (AnscEqualString(ParamName, "LoopDiagnosticsStart", TRUE))
    {
        if (CosaDmlCMGetLoopDiagnosticsStart(NULL, pBool) != ANSC_STATUS_SUCCESS)
            return FALSE;
        return TRUE;
    }
    
    if( AnscEqualString(ParamName, "EnableLog", TRUE))
    {
        /* collect value */
        *pBool = pCfg->EnableLog;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "CleanDocsislog", TRUE))
    {
        /* collect value */
        *pBool = pCfg->CleanDocsisLog;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "DOCSISEnableCert", TRUE))
    {
        /* collect value */
        if (CosaDmlCmGetCMCertStatus(NULL, pBool) == ANSC_STATUS_SUCCESS)
            return TRUE;
        return FALSE;
    }

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
    COSA_CM_DHCP_INFO               Info = {0};

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "TimeOffset", TRUE) )
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
    COSA_CM_DHCP_INFO               Info = {0};
    COSA_CM_IPV6DHCP_INFO           IPv6Info = {0};
    COSA_CM_DS_CHANNEL              Dsc  = {0};
    COSA_CM_US_CHANNEL              Usc  = {0};
    COSA_CM_DOCSIS_INFO             DInfo= {0};

    /* check the parameter name and return the corresponding value */

    if( AnscEqualString(ParamName, "DOCSISDHCPAttempts", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = DInfo.DOCSISDHCPAttempts;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "DOCSISTftpAttempts", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = DInfo.DOCSISTftpAttempts;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "MaxCpeAllowed", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = DInfo.MaxCpeAllowed;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "UpgradeServerIP", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = DInfo.UpgradeServerIP.Value;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "IPAddress", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetDHCPInfo(NULL, &Info) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = Info.IPAddress.Value;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "SubnetMask", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetDHCPInfo(NULL, &Info) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = Info.SubnetMask.Value;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "Gateway", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetDHCPInfo(NULL, &Info) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = Info.Gateway.Value;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "TFTPServer", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetDHCPInfo(NULL, &Info) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = Info.TFTPServer.Value;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "TimeOffset", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetDHCPInfo(NULL, &Info) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = Info.TimeOffset;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "LeaseTimeRemaining", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetDHCPInfo(NULL, &Info) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = Info.LeaseTimeRemaining;

        return TRUE;
    }


    if( AnscEqualString(ParamName, "IPv6LeaseTimeRemaining", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetIPv6DHCPInfo(NULL, &IPv6Info) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = IPv6Info.IPv6LeaseTimeRemaining;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "IPv6RebindTimeRemaining", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetIPv6DHCPInfo(NULL, &IPv6Info) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = IPv6Info.IPv6RebindTimeRemaining;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "IPv6RenewTimeRemaining", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetIPv6DHCPInfo(NULL, &IPv6Info) != ANSC_STATUS_SUCCESS)
            return FALSE;

        *puLong = IPv6Info.IPv6RenewTimeRemaining;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "LockedUpstreamChID", TRUE) )
	{
		if (CosaDmlCMGetLockedUpstreamChID(NULL, puLong) != ANSC_STATUS_SUCCESS)
			return FALSE;

		return TRUE;
	}

	
	if( AnscEqualString(ParamName, "StartDSFrequency", TRUE) )
	{
		if (CosaDmlCMGetStartDSFrequency(NULL, puLong) != ANSC_STATUS_SUCCESS)
			return FALSE;

		return TRUE;
	}

	if( AnscEqualString(ParamName, "CMResetCount", TRUE) )
	{
		if (CosaDmlCMGetResetCount(NULL,CABLE_MODEM_RESET,puLong) != ANSC_STATUS_SUCCESS)
			return FALSE;

		return TRUE;
	}

	if( AnscEqualString(ParamName, "LocalResetCount", TRUE) )
	{
		if (CosaDmlCMGetResetCount(NULL,LOCAL_RESET, puLong) != ANSC_STATUS_SUCCESS)
			return FALSE;

		return TRUE;
	}

	if( AnscEqualString(ParamName, "DocsisResetCount", TRUE) )
	{
		if (CosaDmlCMGetResetCount(NULL,DOCSIS_RESET,puLong) != ANSC_STATUS_SUCCESS)
			return FALSE;

		return TRUE;
	}

	if( AnscEqualString(ParamName, "ErouterResetCount", TRUE) )
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
    COSA_CM_DHCP_INFO               Info = {0};
    COSA_CM_DS_CHANNEL              Dsc  = {0};
    COSA_CM_US_CHANNEL              Usc  = {0};
    COSA_CM_DOCSIS_INFO             DInfo= {0};
    COSA_CM_IPV6DHCP_INFO           IPV6Info= {0};

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "MACAddress", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetDHCPInfo(NULL, &Info) != ANSC_STATUS_SUCCESS)
            return -1;

        AnscCopyString(pValue, Info.MACAddress);

        return 0;
    }

    if( AnscEqualString(ParamName, "BootFileName", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetDHCPInfo(NULL, &Info) != ANSC_STATUS_SUCCESS)
            return -1;

        AnscCopyString(pValue, Info.BootFileName);

        return 0;
    }

    if( AnscEqualString(ParamName, "TimeServer", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetDHCPInfo(NULL, &Info) != ANSC_STATUS_SUCCESS)
            return -1;

        AnscCopyString(pValue, Info.TimeServer);

        return 0;
    }

    if( AnscEqualString(ParamName, "RebindTimeRemaining", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetDHCPInfo(NULL, &Info) != ANSC_STATUS_SUCCESS)
            return -1;

        AnscCopyString(pValue, Info.RebindTimeRemaining);

        return 0;
    }

    if( AnscEqualString(ParamName, "RenewTimeRemaining", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetDHCPInfo(NULL, &Info) != ANSC_STATUS_SUCCESS)
            return -1;

        AnscCopyString(pValue, Info.RenewTimeRemaining);

        return 0;
    }


    if( AnscEqualString(ParamName, "DOCSISDHCPStatus", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetDHCPInfo(NULL, &Info) != ANSC_STATUS_SUCCESS)
            return -1;

        AnscCopyString(pValue, Info.DOCSISDHCPStatus);

        return 0;
    }

    if( AnscEqualString(ParamName, "CMStatus", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetStatus(NULL, pValue) != ANSC_STATUS_SUCCESS)
            return -1;

        return 0;
    }

    if( AnscEqualString(ParamName, "DOCSISVersion", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return -1;

        AnscCopyString(pValue, DInfo.DOCSISVersion);

        return 0;
    }

    if( AnscEqualString(ParamName, "DOCSISDownstreamScanning", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return -1;

        AnscCopyString(pValue, DInfo.DOCSISDownstreamScanning);

        return 0;
    }

    if( AnscEqualString(ParamName, "DOCSISDownstreamRanging", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return -1;

        AnscCopyString(pValue, DInfo.DOCSISDownstreamRanging);

        return 0;
    }

    if( AnscEqualString(ParamName, "DOCSISUpstreamScanning", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return -1;

        AnscCopyString(pValue, DInfo.DOCSISUpstreamScanning);

        return 0;
    }

    if( AnscEqualString(ParamName, "DOCSISUpstreamRanging", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return -1;

        AnscCopyString(pValue, DInfo.DOCSISUpstreamRanging);

        return 0;
    }

    if( AnscEqualString(ParamName, "DOCSISTftpStatus", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return -1;

        AnscCopyString(pValue, DInfo.DOCSISTftpStatus);

        return 0;
    }

    if( AnscEqualString(ParamName, "DOCSISDataRegComplete", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return -1;

        AnscCopyString(pValue, DInfo.DOCSISDataRegComplete);

        return 0;
    }

    if( AnscEqualString(ParamName, "CoreVersion", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return -1;

        AnscCopyString(pValue, DInfo.CoreVersion);

        return 0;
    }

    if( AnscEqualString(ParamName, "DOCSISConfigFileName", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return -1;

        AnscCopyString(pValue, DInfo.DOCSISConfigFileName);

        return 0;
    }

    if( AnscEqualString(ParamName, "ToDStatus", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return -1;

        AnscCopyString(pValue, DInfo.ToDStatus);

        return 0;
    }

    if( AnscEqualString(ParamName, "UpstreamServiceFlowParams", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return -1;

        AnscCopyString(pValue, DInfo.UpstreamServiceFlowParams);

        return 0;
    }

    if( AnscEqualString(ParamName, "DownstreamServiceFlowParams", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return -1;

        AnscCopyString(pValue, DInfo.DownstreamServiceFlowParams);

        return 0;
    }

    if( AnscEqualString(ParamName, "DOCSISDownstreamDataRate", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return -1;

        AnscCopyString(pValue, DInfo.DOCSISDownstreamDataRate);

        return 0;
    }

    if( AnscEqualString(ParamName, "DOCSISUpstreamDataRate", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetDOCSISInfo(NULL, &DInfo) != ANSC_STATUS_SUCCESS)
            return -1;

        AnscCopyString(pValue, DInfo.DOCSISUpstreamDataRate);

        return 0;
    }

    if( AnscEqualString(ParamName, "LoopDiagnosticsDetails", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetLoopDiagnosticsDetails(NULL, pValue) != ANSC_STATUS_SUCCESS)
            return -1;

        return 0;
    }

    if( AnscEqualString(ParamName, "Market", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetMarket(NULL, pValue) != ANSC_STATUS_SUCCESS)
            return -1;

        return 0;
    }

    if( AnscEqualString(ParamName, "MDDIPOverride", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetMDDIPOverride(NULL, pValue) != ANSC_STATUS_SUCCESS)
            return -1;

        return 0;
    }


    if( AnscEqualString(ParamName, "IPv6Address", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetIPv6DHCPInfo(NULL, &IPV6Info) != ANSC_STATUS_SUCCESS)
            return -1;

        AnscCopyString(pValue, IPV6Info.IPv6Address);

        return 0;
    }
    
    if( AnscEqualString(ParamName, "IPv6BootFileName", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetIPv6DHCPInfo(NULL, &IPV6Info) != ANSC_STATUS_SUCCESS)
            return -1;

        AnscCopyString(pValue, IPV6Info.IPv6BootFileName);

        return 0;
    }

    if( AnscEqualString(ParamName, "IPv6Prefix", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetIPv6DHCPInfo(NULL, &IPV6Info) != ANSC_STATUS_SUCCESS)
            return -1;

        AnscCopyString(pValue, IPV6Info.IPv6Prefix);

        return 0;
    }

    if( AnscEqualString(ParamName, "IPv6Router", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetIPv6DHCPInfo(NULL, &IPV6Info) != ANSC_STATUS_SUCCESS)
            return -1;

        AnscCopyString(pValue, IPV6Info.IPv6Router);

        return 0;
    }

    if( AnscEqualString(ParamName, "IPv6TFTPServer", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetIPv6DHCPInfo(NULL, &IPV6Info) != ANSC_STATUS_SUCCESS)
            return -1;

        AnscCopyString(pValue, IPV6Info.IPv6TFTPServer);

        return 0;
    }

    if( AnscEqualString(ParamName, "IPv6TimeServer", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetIPv6DHCPInfo(NULL, &IPV6Info) != ANSC_STATUS_SUCCESS)
            return -1;

        AnscCopyString(pValue, IPV6Info.IPv6TimeServer);

        return 0;
    }

    if( AnscEqualString(ParamName, "TelephonyDHCPStatus", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetTelephonyDHCPStatus(NULL, pValue) != ANSC_STATUS_SUCCESS)
            return -1;

        return 0;
    }

    if( AnscEqualString(ParamName, "TelephonyTftpStatus", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetTelephonyTftpStatus(NULL, pValue) != ANSC_STATUS_SUCCESS)
            return -1;

        return 0;
    }

    if( AnscEqualString(ParamName, "TelephonyRegistrationStatus", TRUE) )
    {
        /* collect value */
        if (CosaDmlCMGetTelephonyRegistrationStatus(NULL, pValue) != ANSC_STATUS_SUCCESS)
            return -1;

        return 0;
    }
    if( AnscEqualString(ParamName, "DOCSISCertificate", TRUE) )
    {
        /* collect value */
        if (CosaDmlCmGetCMCert(NULL, pValue) != ANSC_STATUS_SUCCESS)
            return -1;

        return 0;
    }

	if( AnscEqualString(ParamName, "ProvIpType", TRUE))
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
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)g_pCosaBEManager->hCM;
    PCOSA_DML_CM_LOG                pCfg      = &pMyObject->CmLog;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "LoopDiagnosticsStart", TRUE))
    {
        pMyObject->LoopDiagnosticsStart = bValue;
        return TRUE;
    }

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "EnableLog", TRUE))
    {
        /* save update to backup */
        pCfg->EnableLog = bValue;
        return TRUE;
    }

    if( AnscEqualString(ParamName, "CleanDocsisLog", TRUE))
    {
        /* save update to backup */
        pCfg->CleanDocsisLog = bValue;
        return TRUE;
    }

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
    if( AnscEqualString(ParamName, "LockedUpstreamChID", TRUE))
    {
    	CosaDmlCMSetLockedUpstreamChID(NULL, uValue);
        return TRUE;
    }
	
	/* check the parameter name and set the corresponding value */
	if( AnscEqualString(ParamName, "StartDSFrequency", TRUE))
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
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)g_pCosaBEManager->hCM;

    /* check the parameter name and set the corresponding value */
    if( AnscEqualString(ParamName, "MDDIPOverride", TRUE))
    {
        AnscCopyString(pMyObject->MDDIPOverride, pString);
        return TRUE;
    }

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
    
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "UnerroredCodewords", TRUE))
    {
        /* collect value */
        *puLong = pConf->UnerroredCodewords; 

        return TRUE;
    }

    if( AnscEqualString(ParamName, "CorrectableCodewords", TRUE))
    {
        /* collect value */
        *puLong = pConf->CorrectableCodewords; 

        return TRUE;
    }

    if( AnscEqualString(ParamName, "UncorrectableCodewords", TRUE))
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
    PCOSA_DML_CMERRORCODEWORDS_FULL        pConf        = (PCOSA_DML_CMERRORCODEWORDS_FULL)hInsContext;
    
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
    
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Index", TRUE))
    {
        /* collect value */
        *puLong = pConf->Index; 

        return TRUE;
    }

    if( AnscEqualString(ParamName, "EventID", TRUE))
    {
        /* collect value */
        *puLong = pConf->EventID; 

        return TRUE;
    }

    if( AnscEqualString(ParamName, "EventLevel", TRUE))
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
    
    /* check the parameter name and return the corresponding value */    
    if( AnscEqualString(ParamName, "Description", TRUE))
    {
        /* collect value */
        if ( _ansc_strlen(pConf->Description) >= *pUlSize )
        {
            *pUlSize = _ansc_strlen(pConf->Description);
            return 1;
        }
        
        AnscCopyString(pValue, pConf->Description);
        return 0;
    }

    if( AnscEqualString(ParamName, "Time", TRUE))
    {
        /* collect value */
        if ( _ansc_strlen(pConf->Time) >= *pUlSize )
        {
            *pUlSize = _ansc_strlen(pConf->Time);
            return 1;
        }
        
        AnscCopyString(pValue, pConf->Time);
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
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)g_pCosaBEManager->hCM;
    
    return pMyObject->DownstreamChannelNumber;
}

#if defined (INTEL_PUMA7)
ULONG
DownstreamOFDMChannel_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)g_pCosaBEManager->hCM;

    return pMyObject->DownstreamChannelNumber;
}
#endif

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
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)g_pCosaBEManager->hCM;

    if (nIndex < pMyObject->DownstreamChannelNumber)
    {
        *pInsNumber  = nIndex + 1;

        return &pMyObject->pDownstreamChannel[nIndex];
    }

    return NULL; /* return the handle */
}

#if defined (INTEL_PUMA7)
ANSC_HANDLE
DownstreamOFDMChannel_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    )
{
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)g_pCosaBEManager->hCM;

    if (nIndex < pMyObject->DownstreamChannelNumber)
    {
        *pInsNumber  = nIndex + 1;

        return &pMyObject->pDownstreamOfdmChannel[nIndex];
    }

    return NULL; /* return the handle */
}
#endif


#if defined (INTEL_PUMA7)
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        DownstreamOFDMChannel_IsUpdated
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
DownstreamOFDMChannel_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)g_pCosaBEManager->hCM;

    if ( !pMyObject->DownstreamOFDMChannelUpdateTime )
    {
        pMyObject->DownstreamOFDMChannelUpdateTime = AnscGetTickInSeconds();

        return TRUE;
    }

    if ( pMyObject->DownstreamOFDMChannelUpdateTime >= TIME_NO_NEGATIVE(AnscGetTickInSeconds() - CM_REFRESH_INTERVAL) )
    {
        return FALSE;
    }
    else
    {
        pMyObject->DownstreamOFDMChannelUpdateTime = AnscGetTickInSeconds();

        return TRUE;
    }
}
#endif

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

#if defined (INTEL_PUMA7)
ULONG
DownstreamOFDMChannel_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)g_pCosaBEManager->hCM;
    ANSC_STATUS                     ret       = ANSC_STATUS_SUCCESS;

    Ccsp_cm_clnt_lock();
    if ( pMyObject->pDownstreamOfdmChannel )
    {
        AnscFreeMemory(pMyObject->pDownstreamOfdmChannel);
        pMyObject->pDownstreamOfdmChannel = NULL;
    }

    pMyObject->DownstreamChannelNumber = 0;

    ret = CosaDmlCmGetDownstreamOFDMChannel
        (
            (ANSC_HANDLE)NULL,
            &pMyObject->DownstreamChannelNumber,
            &pMyObject->pDownstreamOfdmChannel
        );

    if ( ret != ANSC_STATUS_SUCCESS )
    {
        pMyObject->pDownstreamOfdmChannel = NULL;
        pMyObject->DownstreamChannelNumber = 0;
    }

    Ccsp_cm_clnt_unlock();
    return 0;
}
#endif

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
    
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "ChannelID", TRUE) )
    {
        /* collect value */
        *puLong = pConf->ChannelID;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "Octets", TRUE) )
    {
        /* collect value */
        *puLong = pConf->Octets;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "Correcteds", TRUE) )
    {
        /* collect value */
        *puLong = pConf->Correcteds;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "Uncorrectables", TRUE) )
    {
        /* collect value */
        *puLong = pConf->Uncorrectables;

        return TRUE;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

#if defined (INTEL_PUMA7)
BOOL
DownstreamOFDMChannel_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    )
{
    PCOSA_CM_OFDM_DS_CHANNEL             pConf        = (PCOSA_CM_OFDM_DS_CHANNEL)hInsContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "fftType", TRUE) )
    {
        /* collect value */
        *pInt = pConf->fftType;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "cpSize", TRUE) )
    {
        /* collect value */
        *pInt = pConf->cpSize;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "NumberOfSubDataCarriers", TRUE) )
    {
        /* collect value */
        *pInt = pConf->numOfDataSubcarr;

        return TRUE;
    }

    return FALSE;
}
#endif

#if defined (INTEL_PUMA7)
BOOL
DownstreamOFDMChannel_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PCOSA_CM_OFDM_DS_CHANNEL             pConf        = (PCOSA_CM_OFDM_DS_CHANNEL)hInsContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "ChannelID", TRUE) )
    {
        /* collect value */
        *puLong = pConf->dcid;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "rollOff", TRUE) )
    {
        /* collect value */
        *puLong = pConf->rollOff;

        return TRUE;
    }

    if( AnscEqualString(ParamName, "TimeInterleavingDepth", TRUE) )
    {
        /* collect value */
        *puLong = pConf->timeInterleavingDepth;

        return TRUE;
    }

    return FALSE;
}
#endif

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
    
    /* check the parameter name and return the corresponding value */    
    if( AnscEqualString(ParamName, "Frequency", TRUE))
    {
        /* collect value */
        if ( _ansc_strlen(pConf->Frequency) >= *pUlSize )
        {
            *pUlSize = _ansc_strlen(pConf->Frequency);
            return 1;
        }
        
        AnscCopyString(pValue, pConf->Frequency);
        return 0;
    }

    if( AnscEqualString(ParamName, "PowerLevel", TRUE))
    {
        /* collect value */
        if ( _ansc_strlen(pConf->PowerLevel) >= *pUlSize )
        {
            *pUlSize = _ansc_strlen(pConf->PowerLevel);
            return 1;
        }
        
        AnscCopyString(pValue, pConf->PowerLevel);
        return 0;
    }

    if( AnscEqualString(ParamName, "SNRLevel", TRUE))
    {
        /* collect value */
        if ( _ansc_strlen(pConf->SNRLevel) >= *pUlSize )
        {
            *pUlSize = _ansc_strlen(pConf->SNRLevel);
            return 1;
        }
        
        AnscCopyString(pValue, pConf->SNRLevel);
        return 0;
    }

    if( AnscEqualString(ParamName, "Modulation", TRUE))
    {
        /* collect value */
        if ( _ansc_strlen(pConf->Modulation) >= *pUlSize )
        {
            *pUlSize = _ansc_strlen(pConf->Modulation);
            return 1;
        }
        
        AnscCopyString(pValue, pConf->Modulation);
        return 0;
    }

    if( AnscEqualString(ParamName, "LockStatus", TRUE))
    {
        /* collect value */
        if ( _ansc_strlen(pConf->LockStatus) >= *pUlSize )
        {
            *pUlSize = _ansc_strlen(pConf->LockStatus);
            return 1;
        }
        
        AnscCopyString(pValue, pConf->LockStatus);
        return 0;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

#if defined (INTEL_PUMA7)
ULONG
DownstreamOFDMChannel_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PCOSA_CM_OFDM_DS_CHANNEL             pConf        = (PCOSA_CM_OFDM_DS_CHANNEL)hInsContext;

    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "Frequency", TRUE))
    {
        /* collect value */
        if ( _ansc_strlen(pConf->OFDMFrequency) >= *pUlSize )
        {
            *pUlSize = _ansc_strlen(pConf->OFDMFrequency);
            return 1;
        }

        AnscCopyString(pValue, pConf->OFDMFrequency);
        return 0;
    }

    if( AnscEqualString(ParamName, "TunerFrequency", TRUE))
    {
        /* collect value */
        if ( _ansc_strlen(pConf->tunerFrequency) >= *pUlSize )
        {
            *pUlSize = _ansc_strlen(pConf->tunerFrequency);
            return 1;
        }

        AnscCopyString(pValue, pConf->tunerFrequency);
        return 0;
    }

    if( AnscEqualString(ParamName, "PlcCenterFrequency", TRUE))
    {
        /* collect value */
        if ( _ansc_strlen(pConf->plcCenterFrequency) >= *pUlSize )
        {
            *pUlSize = _ansc_strlen(pConf->plcCenterFrequency);
            return 1;
        }

        AnscCopyString(pValue, pConf->plcCenterFrequency);
        return 0;
    }

    if( AnscEqualString(ParamName, "PowerLevel", TRUE))
    {
        /* collect value */
        if ( _ansc_strlen(pConf->rxPower) >= *pUlSize )
        {
            *pUlSize = _ansc_strlen(pConf->rxPower);
            return 1;
        }

        AnscCopyString(pValue, pConf->rxPower);
        return 0;
    }

    if( AnscEqualString(ParamName, "Modulation", TRUE))
    {
        /* collect value */
        if ( _ansc_strlen(pConf->Modulation) >= *pUlSize )
        {
            *pUlSize = _ansc_strlen(pConf->Modulation);
            return 1;
        }

        AnscCopyString(pValue, pConf->Modulation);
        return 0;
    }

    if( AnscEqualString(ParamName, "LockStatus", TRUE))
    {
        /* collect value */
        if ( _ansc_strlen(pConf->LockStatus) >= *pUlSize )
        {
            *pUlSize = _ansc_strlen(pConf->LockStatus);
            return 1;
        }

        AnscCopyString(pValue, pConf->LockStatus);
        return 0;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}
#endif

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
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)g_pCosaBEManager->hCM;

    if (nIndex < pMyObject->UpstreamChannelNumber)
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
    
    /* check the parameter name and return the corresponding value */
    if( AnscEqualString(ParamName, "ChannelID", TRUE) )
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
    
    /* check the parameter name and return the corresponding value */    
    if( AnscEqualString(ParamName, "Frequency", TRUE))
    {
        /* collect value */
        if ( _ansc_strlen(pConf->Frequency) >= *pUlSize )
        {
            *pUlSize = _ansc_strlen(pConf->Frequency);
            return 1;
        }
        
        AnscCopyString(pValue, pConf->Frequency);
        return 0;
    }

    if( AnscEqualString(ParamName, "PowerLevel", TRUE))
    {
        /* collect value */
        if ( _ansc_strlen(pConf->PowerLevel) >= *pUlSize )
        {
            *pUlSize = _ansc_strlen(pConf->PowerLevel);
            return 1;
        }
        
        AnscCopyString(pValue, pConf->PowerLevel);
        return 0;
    }

    if( AnscEqualString(ParamName, "ChannelType", TRUE))
    {
        /* collect value */
        if ( _ansc_strlen(pConf->ChannelType) >= *pUlSize )
        {
            *pUlSize = _ansc_strlen(pConf->ChannelType);
            return 1;
        }
        
        AnscCopyString(pValue, pConf->ChannelType);
        return 0;
    }

    if( AnscEqualString(ParamName, "SymbolRate", TRUE))
    {
        /* collect value */
        if ( _ansc_strlen(pConf->SymbolRate) >= *pUlSize )
        {
            *pUlSize = _ansc_strlen(pConf->SymbolRate);
            return 1;
        }
        
        AnscCopyString(pValue, pConf->SymbolRate);
        return 0;
    }

    if( AnscEqualString(ParamName, "Modulation", TRUE))
    {
        /* collect value */
        if ( _ansc_strlen(pConf->Modulation) >= *pUlSize )
        {
            *pUlSize = _ansc_strlen(pConf->Modulation);
            return 1;
        }
        
        AnscCopyString(pValue, pConf->Modulation);
        return 0;
    }

    if( AnscEqualString(ParamName, "LockStatus", TRUE))
    {
        /* collect value */
        if ( _ansc_strlen(pConf->LockStatus) >= *pUlSize )
        {
            *pUlSize = _ansc_strlen(pConf->LockStatus);
            return 1;
        }
        
        AnscCopyString(pValue, pConf->LockStatus);
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
    
    /* check the parameter name and return the corresponding value */    
    if( AnscEqualString(ParamName, "IPAddress", TRUE))
    {
        /* collect value */
        if ( _ansc_strlen(pConf->IPAddress) >= *pUlSize )
        {
            *pUlSize = _ansc_strlen(pConf->IPAddress);
            return 1;
        }
        
        AnscCopyString(pValue, pConf->IPAddress);
        return 0;
    }

    if( AnscEqualString(ParamName, "MACAddress", TRUE))
    {
        /* collect value */
        if ( _ansc_strlen(pConf->MACAddress) >= *pUlSize )
        {
            *pUlSize = _ansc_strlen(pConf->MACAddress);
            return 1;
        }
        
        AnscCopyString(pValue, pConf->MACAddress);
        return 0;
    }

    /* CcspTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return -1;
}

