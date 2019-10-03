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

    module: cosa_deviceinfo_api.c

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

        01/11/2011    initial revision.

**************************************************************************/

//!!!  This code assumes that all data structures are the SAME in middle-layer APIs and HAL layer APIs
//!!!  So it uses casting from one to the other
#include "cosa_x_cisco_com_cablemodem_apis.h"
#include "cm_hal.h" 

#include "cosa_x_cisco_com_cablemodem_internal.h"

static pthread_mutex_t __gw_cm_client_lock = PTHREAD_MUTEX_INITIALIZER;

extern int Ccsp_cm_clnt_lock(void)
{
    return pthread_mutex_lock(&__gw_cm_client_lock);
}

extern int Ccsp_cm_clnt_unlock(void)
{
    return pthread_mutex_unlock(&__gw_cm_client_lock);
}


// Below function will poll the Docsis diagnostic information
void PollDocsisInformations()
{

  system("touch /nvram/docsispolltime.txt");

  FILE *fp;
  char buff[30];
  int pollinterval=4*3600;
  int i,retValue;
  while(1)
  { 
	   memset(buff,0,sizeof(buff));
           fp = fopen("/nvram/docsispolltime.txt", "r");
	   if (!fp)
	   {
		CcspTraceError(("Error while opening the file.\n"));
	   }
	   else
	   {
           	retValue = fscanf(fp, "%29s", buff);
           	if( (retValue != -1) && (buff != NULL ) )
           	{
        		pollinterval = atoi(buff);
           	}

           	fclose(fp);
    	   }
	   
	   
// Fetching docsis gateway info
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)g_pCosaBEManager->hCM;
    ANSC_STATUS                     ret       = ANSC_STATUS_SUCCESS;


    Ccsp_cm_clnt_lock();
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
        Ccsp_cm_clnt_unlock();
	goto EXIT;
    }
    docsis_GetNumOfActiveRxChannels(&pMyObject->DownstreamChannelNumber);
    CcspTraceWarning(("Number of Active Rxchannel is %lu\n",pMyObject->DownstreamChannelNumber));
    for (i=0 ; i < pMyObject->DownstreamChannelNumber ; i ++)
    {
	if (strcmp(pMyObject->pDownstreamChannel[i].Frequency,""))
	{
    		CcspTraceWarning(("RDKB_DOCSIS_DIAG_INFO: CM Downstream frequency is %s and Downstream is %s on channel %d\n",pMyObject->pDownstreamChannel[i].Frequency,pMyObject->pDownstreamChannel[i].LockStatus,i));
	}
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
        Ccsp_cm_clnt_unlock();
	goto EXIT;
    }

    docsis_GetNumOfActiveTxChannels(&pMyObject->UpstreamChannelNumber);
    CcspTraceWarning(("RDKB_DOCSIS_DIAG_INFO: Number of Active Txchannel is %lu\n",pMyObject->UpstreamChannelNumber));
    for (i=0 ; i < pMyObject->UpstreamChannelNumber ; i ++)
    {
	if (strcmp(pMyObject->pUpstreamChannel[i].Frequency,""))
	{
    		CcspTraceWarning(("RDKB_DOCSIS_DIAG_INFO: CM Upstream frequency is %s and Upstream is %s on channel %d\n",pMyObject->pUpstreamChannel[i].Frequency,pMyObject->pUpstreamChannel[i].LockStatus,i));
	}

    }
    Ccsp_cm_clnt_unlock();
    CcspTraceWarning(("pollinterval to fetch Docsis diag is= %d\n",pollinterval));
    sleep (pollinterval);

EXIT:

    Ccsp_cm_clnt_lock();
    if ( pMyObject->pDownstreamChannel )
    {
        AnscFreeMemory(pMyObject->pDownstreamChannel);
        pMyObject->pDownstreamChannel = NULL;
    }

    if ( pMyObject->pUpstreamChannel )
    {
        AnscFreeMemory(pMyObject->pUpstreamChannel);
        pMyObject->pUpstreamChannel = NULL;
    }
    Ccsp_cm_clnt_unlock();
  }
}

ANSC_STATUS
CosaDmlCMInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject    = (PCOSA_DATAMODEL_CABLEMODEM)phContext;
	if(pMyObject) 
    	CosaDmlCmGetLog( NULL, &pMyObject->CmLog);
    cm_hal_InitDB();
    pthread_t docsisinfo;
    pthread_create(&docsisinfo, NULL, &PollDocsisInformations, NULL); 
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlCMGetStatus
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    if(!pValue){
	AnscTraceWarning(("Input parameter is NULL  %s, %d\n", __FUNCTION__, __LINE__));
	return ANSC_STATUS_FAILURE;
	}
    if( docsis_getCMStatus(pValue) == RETURN_OK )
        return ANSC_STATUS_SUCCESS;
    else
        return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
CosaDmlCMGetLoopDiagnosticsStart
    (
        ANSC_HANDLE                 hContext,
        BOOL*                       pBool
    )
{
   if(!pBool){
	AnscTraceWarning(("Input parameter is NULL  %s, %d\n", __FUNCTION__, __LINE__));
	return ANSC_STATUS_FAILURE;
	}
    *pBool = FALSE;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlCMSetLoopDiagnosticsStart
    (
        ANSC_HANDLE                 hContext,
        BOOL                        pBool
    )
{
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlCMGetLoopDiagnosticsDetails
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    if(!pValue){
	AnscTraceWarning(("Input parameter is NULL  %s, %d\n", __FUNCTION__, __LINE__));
	return ANSC_STATUS_FAILURE;
	}
    AnscCopyString(pValue, "Dummy");
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlCMGetTelephonyDHCPStatus
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    if(!pValue){
	AnscTraceWarning(("Input parameter is NULL  %s, %d\n", __FUNCTION__, __LINE__));
	return ANSC_STATUS_FAILURE;
	}
    AnscCopyString(pValue, "Dummy-InProgress");
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlCMGetTelephonyTftpStatus
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    if(!pValue){
	AnscTraceWarning(("Input parameter is NULL  %s, %d\n", __FUNCTION__, __LINE__));
	return ANSC_STATUS_FAILURE;
	}
    AnscCopyString(pValue, "Dummy-InProgress");
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlCMGetTelephonyRegistrationStatus
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    if(!pValue){
	AnscTraceWarning(("Input parameter is NULL  %s, %d\n", __FUNCTION__, __LINE__));
	return ANSC_STATUS_FAILURE;
	}
    AnscCopyString(pValue, "Dummy-InProgress");
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlCMGetDHCPInfo
    (
        ANSC_HANDLE                 hContext,
        PCOSA_CM_DHCP_INFO          pInfo
    )
{
    if (cm_hal_GetDHCPInfo((PCMMGMT_CM_DHCP_INFO)pInfo) == RETURN_OK)
        return ANSC_STATUS_SUCCESS;
    else 
        return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
CosaDmlCMGetDOCSISInfo
    (
        ANSC_HANDLE                 hContext,
        PCOSA_CM_DOCSIS_INFO        pInfo
    )
{
    if( docsis_GetDOCSISInfo((PCMMGMT_CM_DOCSIS_INFO)pInfo) == RETURN_OK)
        return ANSC_STATUS_SUCCESS;
    else 
        return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
CosaDmlCmGetLog
    (
        ANSC_HANDLE                hContext,
        PCOSA_DML_CM_LOG           pCfg
    )
{   
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlCmSetLog
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_CM_LOG            pCfg
    )
{
    if(!pCfg){
	AnscTraceWarning(("Input parameter is NULL  %s, %d\n", __FUNCTION__, __LINE__));
	return ANSC_STATUS_FAILURE;
	}
    if(pCfg->CleanDocsisLog == 1){
        docsis_ClearDocsisEventLog();
        pCfg->CleanDocsisLog = 0; 
    }
    return ANSC_STATUS_SUCCESS;
}

#define DOCSIS_EVENT_LOG_SIZE 50

ANSC_STATUS
CosaDmlCmGetDocsisLog
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
        PCOSA_DML_DOCSISLOG_FULL    *ppConf
    )    
{
    CMMGMT_CM_EventLogEntry_t entries[DOCSIS_EVENT_LOG_SIZE];
    int count = 0;
    int i;
    PCOSA_DML_DOCSISLOG_FULL p;

    if((!pulCount) || (!ppConf)){
	AnscTraceWarning(("Input parameter is NULL  pulCount = %d , ppConf = %d , %s, %d\n",pulCount, ppConf, __FUNCTION__, __LINE__));
	return ANSC_STATUS_FAILURE;
	}

    memset(entries, 0, DOCSIS_EVENT_LOG_SIZE*sizeof(CMMGMT_CM_EventLogEntry_t));

    count = docsis_GetDocsisEventLogItems(entries, DOCSIS_EVENT_LOG_SIZE);

    p = (PCOSA_DML_DOCSISLOG_FULL)AnscAllocateMemory(count * sizeof(COSA_DML_DOCSISLOG_FULL));
    if(p == NULL) {
        AnscTraceWarning(("ReAllocateMemory error %s, %d\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }
    for(i=0;i<count;i++) {
        memcpy(p[i].Time, ctime(&(entries[i].docsDevEvFirstTime.tv_sec)), 64);
        p[i].EventID = entries[i].docsDevEvId;
        p[i].EventLevel = entries[i].docsDevEvLevel;
        memcpy(p[i].Description, entries[i].docsDevEvText, 255);
    }

    *pulCount = count;
    *ppConf = p;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlCmGetDownstreamChannel
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
        PCOSA_CM_DS_CHANNEL         *ppConf        
    )    
{
    if((!pulCount) || (!ppConf)){
	AnscTraceWarning(("Input parameter is NULL  pulCount = %d , ppConf = %d , %s, %d\n",pulCount, ppConf, __FUNCTION__, __LINE__));
	return ANSC_STATUS_FAILURE;
	}
    docsis_GetNumOfActiveRxChannels(pulCount);
    if(*pulCount) {

        *ppConf = (PCOSA_CM_DS_CHANNEL)AnscAllocateMemory( sizeof(COSA_CM_DS_CHANNEL) * (*pulCount) );
    
        docsis_GetDSChannel((PCMMGMT_CM_DS_CHANNEL *)ppConf);
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlCmGetUpstreamChannel
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
        PCOSA_CM_US_CHANNEL         *ppConf        
    )    
{
    if((!pulCount) || (!ppConf)){
	AnscTraceWarning(("Input parameter is NULL  pulCount = %d , ppConf = %d , %s, %d\n",pulCount, ppConf, __FUNCTION__, __LINE__));
	return ANSC_STATUS_FAILURE;
	}
    docsis_GetNumOfActiveTxChannels(pulCount);

    if(*pulCount) {

        *ppConf = (PCOSA_CM_US_CHANNEL)AnscAllocateMemory( sizeof(COSA_CM_US_CHANNEL) * (*pulCount) );
    
        docsis_GetUSChannel((PCMMGMT_CM_US_CHANNEL *)ppConf);
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlCMGetMarket
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{

    if( cm_hal_GetMarket(pValue) == RETURN_OK)
        return ANSC_STATUS_SUCCESS;
    else
        return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
CosaDmlCMGetMDDIPOverride
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    if ( docsis_GetMddIpModeOverride(pValue) == RETURN_OK)
        return ANSC_STATUS_SUCCESS; 
    else
        return ANSC_STATUS_FAILURE;
    
}

ANSC_STATUS
CosaDmlCMSetMDDIPOverride
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    if ( docsis_SetMddIpModeOverride(pValue) == RETURN_OK)
        return ANSC_STATUS_SUCCESS; 
    else
        return ANSC_STATUS_FAILURE;
}


ANSC_STATUS
CosaDmlCmGetCMErrorCodewords
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulCount,
        PCOSA_DML_CMERRORCODEWORDS_FULL   *ppConf
    )
{
    if((!pulCount) || (!ppConf)){
	AnscTraceWarning(("Input parameter is NULL  pulCount = %d , ppConf = %d , %s, %d\n",pulCount, ppConf, __FUNCTION__, __LINE__));
	return ANSC_STATUS_FAILURE;
	}
    docsis_GetNumOfActiveRxChannels(pulCount);

    if(*pulCount) {

        *ppConf = (PCOSA_DML_CMERRORCODEWORDS_FULL)AnscAllocateMemory( sizeof(COSA_DML_CMERRORCODEWORDS_FULL) * (*pulCount) );
    
        docsis_GetErrorCodewords((PCMMGMT_CM_ERROR_CODEWORDS *)ppConf);
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlCmGetCMCert
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    if(!pValue)
    {
	AnscTraceWarning(("Input parameter is NULL  %s, %d\n", __FUNCTION__, __LINE__));
	return ANSC_STATUS_FAILURE;
    }

    if( docsis_GetCert(pValue) == RETURN_OK )
    {
        return ANSC_STATUS_SUCCESS;
    }
    else
    {
        AnscTraceWarning(("Can not get CM Cert %s, %d\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }
}

ANSC_STATUS
CosaDmlCmGetCMCertStatus
    (
        ANSC_HANDLE                 hContext,
        BOOL*                       pBool
    )
{
    ULONG ulcertStatus = 0;
    if(!pBool){
	AnscTraceWarning(("Input parameter is NULL  %s, %d\n", __FUNCTION__, __LINE__));
	return ANSC_STATUS_FAILURE;
	}
    if (docsis_GetCertStatus(&ulcertStatus) != ANSC_STATUS_SUCCESS)
        return ANSC_STATUS_FAILURE;
    else {
        if(ulcertStatus) {
            *pBool = TRUE;
        } else {
            *pBool = FALSE;
        }
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlCMGetIPv6DHCPInfo
    (
        ANSC_HANDLE                 hContext,
        PCOSA_CM_IPV6DHCP_INFO      pInfo
    )
{

    if (cm_hal_GetIPv6DHCPInfo((PCMMGMT_CM_IPV6DHCP_INFO)pInfo) == RETURN_OK)
        return ANSC_STATUS_SUCCESS;
    else 
        return ANSC_STATUS_FAILURE;
}

ANSC_STATUS
CosaDmlCMGetLockedUpstreamChID
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pValue
    )
{
    if(!pValue){
	AnscTraceWarning(("Input parameter is NULL  %s, %d\n", __FUNCTION__, __LINE__));
	return ANSC_STATUS_FAILURE;
	}
    *pValue = docsis_GetUSChannelId();

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlCMSetLockedUpstreamChID
    (
        ANSC_HANDLE                 hContext,
        ULONG                      value
    )
{
    int chId = (int)value;
    docsis_SetUSChannelId(chId);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlCMGetStartDSFrequency
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pValue
    )
{
    if(!pValue){
	AnscTraceWarning(("Input parameter is NULL  %s, %d\n", __FUNCTION__, __LINE__));
	return ANSC_STATUS_FAILURE;
	}
    *pValue = docsis_GetDownFreq();

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlCMSetStartDSFrequency
    (
        ANSC_HANDLE                 hContext,
        ULONG                      value
    )
{
    docsis_SetStartFreq(value);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlCMGetProvType
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    if ( docsis_GetProvIpType(pValue) == RETURN_OK)
        return ANSC_STATUS_SUCCESS; 
    else
        return ANSC_STATUS_FAILURE; 
}

ANSC_STATUS
CosaDmlCMGetResetCount
    (
        ANSC_HANDLE                 hContext,
		CM_RESET_TYPE         type,
        ULONG                       *pValue
    )
{
	INT ret_val=RETURN_ERR;
	switch(type)
	{
		case CABLE_MODEM_RESET:{
				ret_val=cm_hal_Get_CableModemResetCount(pValue);
			}
			break;
		case LOCAL_RESET:{
				ret_val=cm_hal_Get_LocalResetCount(pValue);
			}
			break;
		case DOCSIS_RESET:{
				ret_val=cm_hal_Get_DocsisResetCount(pValue);
			}
			break;
		case EROUTER_RESET:{
				ret_val=cm_hal_Get_ErouterResetCount(pValue);
			}
			break;
		default:{
			 AnscTraceWarning(("Invalid type %s, %d\n", __FUNCTION__, __LINE__));
			}
	}

    	if(RETURN_ERR == ret_val)
		return ANSC_STATUS_FAILURE;
    	else
		return ANSC_STATUS_SUCCESS;
}


#define LANMODE_DM               "Device.X_CISCO_COM_DeviceControl.LanManagementEntry.1.LanMode"
#include "plugin_main_apis.h"
extern void* g_pDslhDmlAgent;

ANSC_STATUS
CosaDmlCmGetCPEList
    (
        ANSC_HANDLE                 hContext,
        PULONG                      pulInstanceNumber,
        PCOSA_DML_CPE_LIST          *ppCPEList
    )
{
    char    LanMode[64] = {0};
    ULONG   size = 64;
    if((!pulInstanceNumber) || (!ppCPEList)){
	AnscTraceWarning(("Input parameter is NULL  pulInstanceNumber = %d , ppCPEList = %d , %s, %d\n",pulInstanceNumber, ppCPEList, __FUNCTION__, __LINE__));
	return ANSC_STATUS_FAILURE;
	}
    if(0 != g_GetParamValueString(g_pDslhDmlAgent, LANMODE_DM, LanMode, &size))
    {
        return ANSC_STATUS_FAILURE;
    }    

    *pulInstanceNumber = 0;
    PCMMGMT_DML_CPE_LIST pInfo = NULL;
    if( cm_hal_GetCPEList(&pInfo, pulInstanceNumber, LanMode) != RETURN_OK)
        return ANSC_STATUS_FAILURE;

    if (*pulInstanceNumber > 0) {
        if( (*ppCPEList = (PCOSA_DML_CPE_LIST)AnscAllocateMemory(sizeof(COSA_DML_CPE_LIST)*(*pulInstanceNumber))) == NULL )
        {
            AnscTraceWarning(("AllocateMemory error %s, %d\n", __FUNCTION__, __LINE__));
            return ANSC_STATUS_FAILURE;  
        }
        AnscCopyMemory(*ppCPEList, pInfo, sizeof(COSA_DML_CPE_LIST)*(*pulInstanceNumber));
        free(pInfo);
    }

    return ANSC_STATUS_FAILURE;
}
