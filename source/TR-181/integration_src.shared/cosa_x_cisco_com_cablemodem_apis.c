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
#include "syscfg/syscfg.h"
#include "safec_lib_common.h"
#include <sys/stat.h>

#define  PVALUE_MAX 1023 
#if defined (FEATURE_RDKB_WAN_MANAGER)
#define WAN_INTERFACE_PARAM_NAME "Device.X_RDK_WanManager.CPEInterface.%d.Wan.Name"

#ifdef _COSA_BCM_ARM_
#define DOCSIS_INF_NAME "cm0"
#elif defined(INTEL_PUMA7)
#define DOCSIS_INF_NAME "dpdmta1"
#else
#define DOCSIS_INF_NAME "cm0"
#endif

#define WAN_PHY_NAME "erouter0"
#define MONITOR_PHY_STATUS_MAX_TIMEOUT 240
#define MONITOR_OPER_STATUS_MAX_TIMEOUT 240
#define MONITOR_OPER_STATUS_QUERY_INTERVAL 10
#define MONITOR_PHY_STATUS_QUERY_INTERVAL 2

typedef enum WanMode
{
    WAN_MODE_AUTO = 0,
    WAN_MODE_ETH,
    WAN_MODE_DOCSIS,
    WAN_MODE_UNKNOWN
}WanMode_t;
#endif

static pthread_mutex_t __gw_cm_client_lock = PTHREAD_MUTEX_INITIALIZER;
extern ANSC_HANDLE bus_handle;
extern int Ccsp_cm_clnt_lock(void)
{
    return pthread_mutex_lock(&__gw_cm_client_lock);
}

extern int Ccsp_cm_clnt_unlock(void)
{
    return pthread_mutex_unlock(&__gw_cm_client_lock);
}

#if defined (FEATURE_RDKB_WAN_MANAGER)
ANSC_STATUS SetParamValues( char *pComponent, char *pBus, char *pParamName, char *pParamVal, enum dataType_e type, BOOLEAN bCommit )
{
    CCSP_MESSAGE_BUS_INFO *bus_info              = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    parameterValStruct_t   param_val[1]          = { 0 };
    char                  *faultParam            = NULL;
    char                   acParameterName[256]  = { 0 },
                           acParameterValue[256] = { 0 };
    int                    ret                   = 0;

    //Copy Name
    sprintf( acParameterName, "%s", pParamName );
    param_val[0].parameterName  = acParameterName;

    //Copy Value
    sprintf( acParameterValue, "%s", pParamVal );
    param_val[0].parameterValue = acParameterValue;

    //Copy Type
    param_val[0].type           = type;
    CcspTraceInfo(("%s-%d Param set %s value %s\n",__FUNCTION__,__LINE__,pParamName,pParamVal));

    ret = CcspBaseIf_setParameterValues(
                                        bus_handle,
                                        pComponent,
                                        pBus,
                                        0,
                                        0,
                                        param_val,
                                        1,
                                        bCommit,
                                        &faultParam
                                       );

    if( ( ret != CCSP_SUCCESS ) && ( faultParam != NULL ) )
    {
        CcspTraceError(("%s-%d Failed to set %s\n",__FUNCTION__,__LINE__,pParamName));
        bus_info->freefunc( faultParam );
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}
#endif

// Below function will poll the Docsis diagnostic information
void *PollDocsisInformations(void *args)
{
  UNREFERENCED_PARAMETER(args);
  creat("/nvram/docsispolltime.txt",S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

  FILE *fp;
  char buff[30];
  int pollinterval=4*3600;
  int retValue;
  ULONG i;
  errno_t rc = -1;
  int ind = -1;
  while(1)
  { 
          
            rc = memset_s(buff,sizeof(buff),0,sizeof(buff));
            ERR_CHK(rc);
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
    CcspTraceWarning(("Number of Active Rxchannel is %lu\n",pMyObject->DownstreamChannelNumber));
    for (i=0 ; i < pMyObject->DownstreamChannelNumber ; i ++)
    {
	rc = strcmp_s(pMyObject->pDownstreamChannel[i].Frequency,sizeof(pMyObject->pDownstreamChannel[i].Frequency),"",&ind);
        ERR_CHK(rc);
        if((ind) && (rc == EOK))
	{
		CcspTraceWarning(("RDKB_DOCSIS_DIAG_INFO: CM Downstream frequency is %s and Downstream is %s on channel %lu\n",pMyObject->pDownstreamChannel[i].Frequency,pMyObject->pDownstreamChannel[i].LockStatus,i));
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
    CcspTraceWarning(("RDKB_DOCSIS_DIAG_INFO: Number of Active Txchannel is %lu\n",pMyObject->UpstreamChannelNumber));
    for (i=0 ; i < pMyObject->UpstreamChannelNumber ; i ++)
    {
        if (pMyObject->pUpstreamChannel[i].Frequency[0] != '\0')

	{
		CcspTraceWarning(("RDKB_DOCSIS_DIAG_INFO: CM Upstream frequency is %s and Upstream is %s on channel %lu\n",pMyObject->pUpstreamChannel[i].Frequency,pMyObject->pUpstreamChannel[i].LockStatus,i));
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
    UNREFERENCED_PARAMETER(hDml);
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject    = (PCOSA_DATAMODEL_CABLEMODEM)phContext;
    PCOSA_DML_CM_WANCFG               pWanCfg      = NULL;
	if(pMyObject) 
    {
    	CosaDmlCmGetLog( NULL, &pMyObject->CmLog);
        pWanCfg      = &pMyObject->CmWanCfg;
        memset(pWanCfg,0,sizeof(COSA_DML_CM_WANCFG));
    }
    /*Coverity Fix CID:55875 CHECKED_RETURN */
    if( cm_hal_InitDB() != RETURN_OK )
    {
            AnscTraceWarning(("cm_hal_InitDB  is Not success   %s, %d\n", __FUNCTION__, __LINE__));
    }
    else
    {
          AnscTraceWarning(("cm_hal_InitDB  is called successfully   %s, %d\n", __FUNCTION__, __LINE__));
    }
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
    UNREFERENCED_PARAMETER(hContext);
    if(!pValue){
	AnscTraceWarning(("Input parameter is NULL  %s, %d\n", __FUNCTION__, __LINE__));
	return ANSC_STATUS_FAILURE;
	}
    if( docsis_getCMStatus(pValue) == RETURN_OK )
        return ANSC_STATUS_SUCCESS;
    else
        return ANSC_STATUS_FAILURE;
}

#if defined (FEATURE_RDKB_WAN_MANAGER)
ANSC_STATUS
CosaDmlCMWanUpdateCustomConfig
    (
        void *arg,
        BOOL             bValue
    )
{
    char command[256] = {0};
    if (bValue == TRUE)
    {
       memset(command,0,sizeof(command));
        snprintf(command,sizeof(command),"ip addr flush dev %s",DOCSIS_INF_NAME);
        system(command);
        memset(command,0,sizeof(command));
        snprintf(command,sizeof(command),"ip -6 addr flush dev %s",DOCSIS_INF_NAME);
        system(command);
        memset(command,0,sizeof(command));
        snprintf(command,sizeof(command),"sysctl -w net.ipv6.conf.%s.accept_ra=0",DOCSIS_INF_NAME);
        system(command);
        memset(command,0,sizeof(command));
        snprintf(command,sizeof(command),"sysctl -w net.ipv6.conf.%s.disable_ipv6=1",DOCSIS_INF_NAME);
        system(command); 
        memset(command,0,sizeof(command));
        snprintf(command,sizeof(command),"ip link set %s up",DOCSIS_INF_NAME);
        system(command);
        memset(command,0,sizeof(command));
        snprintf(command,sizeof(command),"brctl addif %s %s",WAN_PHY_NAME,DOCSIS_INF_NAME);
        system(command);
    }
    else
    {
        memset(command,0,sizeof(command));
        snprintf(command,sizeof(command),"brctl delif %s %s",WAN_PHY_NAME,DOCSIS_INF_NAME);
        system(command);
    }
    UNREFERENCED_PARAMETER(arg);
    return ANSC_STATUS_SUCCESS;
}

void* ThreadMonitorPhyStatusAndNotify(void *args)
{
    PCOSA_DATAMODEL_CABLEMODEM pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)args;
    pthread_detach(pthread_self());
    if (pMyObject)
    {
        PCOSA_DML_CM_WANCFG pWanCfg = &pMyObject->CmWanCfg;
        INT ret = -1;
        INT counter = 0;
        INT iWanInstanceNumber = WAN_CM_INTERFACE_INSTANCE_NUM;
        BOOLEAN rfStatus = FALSE;
        char acSetParamName[256];
        char acSetParamValue[128];
        char buf[64];
        ANSC_STATUS retval = ANSC_STATUS_SUCCESS;
        if (pWanCfg)
        {
            if (pWanCfg->wanInstanceNumber)
            {
                iWanInstanceNumber = atoi(pWanCfg->wanInstanceNumber);
            }
            while(1)
            {

                // cmoff file only for debugging purpose to simulate cable modem down case.
                if ( 0 == access( "/tmp/cmoff" , F_OK ) )                
                {
                    rfStatus = FALSE;
                    sleep(1);
                    continue;
                }
                ret = docsis_IsEnergyDetected(&rfStatus);

                if(ret == RETURN_OK)
                {
                    if (rfStatus == TRUE)
                    {
                        INT lastKnownWanMode = WAN_MODE_DOCSIS;
                        memset(buf,0,sizeof(buf));
                        if (syscfg_get(NULL, "last_wan_mode", buf, sizeof(buf)) == 0)
                        {
                            lastKnownWanMode = atoi(buf);
                        }
                        if (lastKnownWanMode == WAN_MODE_DOCSIS)
                        {
                            /* This file "/tmp/phylink_wan_state_up"
                             * is created in docsis link up callback.
                             * Send Phy Status up immediately if rf status and this file present
                             * otherwise wait till timeout in docsis mode.
                             */
                            if (0 == access( "/tmp/phylink_wan_state_up" , F_OK ))
                            {
                                break;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                if (counter >= MONITOR_PHY_STATUS_MAX_TIMEOUT)
                {
                    break;
                }

                sleep(MONITOR_PHY_STATUS_QUERY_INTERVAL);
                counter += MONITOR_PHY_STATUS_QUERY_INTERVAL;

            }
            memset(acSetParamName, 0, sizeof(acSetParamName));
            snprintf(acSetParamName, sizeof(acSetParamName), WAN_PHY_STATUS_PARAM_NAME, iWanInstanceNumber);
            memset(acSetParamValue, 0, sizeof(acSetParamValue));
            if (TRUE == rfStatus)
            {
                snprintf(acSetParamValue, sizeof(acSetParamValue), "Up");

            }
            else
            {
                snprintf(acSetParamValue, sizeof(acSetParamValue), "Down");
            }
            CcspTraceInfo(("%s-%d Param set %s\n",__FUNCTION__,__LINE__,acSetParamName));
            counter = 0;
            do
            {
                if (counter > 3) // retry 3 times
                {
                    break;
                }
                retval = SetParamValues(WAN_COMPONENT_NAME, WAN_DBUS_PATH, acSetParamName, acSetParamValue,ccsp_string,TRUE);
                sleep(1);
                ++counter;
            }while(retval == ANSC_STATUS_FAILURE);
            pWanCfg->MonitorPhyStatusAndNotify = FALSE;
        }
    }
    return args;
}

void* ThreadMonitorOperStatusAndNotify(void *args)
{
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject    = (PCOSA_DATAMODEL_CABLEMODEM)args;
    pthread_detach(pthread_self());
    if (pMyObject)
    {
        PCOSA_DML_CM_WANCFG pWanCfg = &pMyObject->CmWanCfg;
        char buf[128];
        char acSetParamName[256];
        BOOL isOperational = FALSE;
        INT counter = 0;
        INT iWanInstanceNumber = WAN_CM_INTERFACE_INSTANCE_NUM;
        char acSetParamValue[128];
        ANSC_STATUS retval = ANSC_STATUS_SUCCESS;
        if (pWanCfg)
        {
            if (pWanCfg->wanInstanceNumber)
            {
                iWanInstanceNumber = atoi(pWanCfg->wanInstanceNumber);
            }
            while(1)
            {
                memset(buf,0,sizeof(buf));
                if ( 0 == access( "/tmp/cmoff" , F_OK ) )
                {
                     isOperational = FALSE;
                     sleep(1);
                     continue;
                }
 
                if(docsis_getCMStatus(buf) == RETURN_OK)
                {
                    if (strncmp(buf,"OPERATIONAL",sizeof(buf)) == 0)
                    {              
                        isOperational = TRUE;
                        break;
                    }
                }
               if (counter >= MONITOR_OPER_STATUS_MAX_TIMEOUT)
                {
                    break;
                }
                sleep(MONITOR_OPER_STATUS_QUERY_INTERVAL);
                counter += MONITOR_OPER_STATUS_QUERY_INTERVAL;
            }
            memset(acSetParamName, 0, sizeof(acSetParamName));
            snprintf(acSetParamName, sizeof(acSetParamName), WAN_OPER_STATUS_PARAM_NAME, iWanInstanceNumber);

            memset(acSetParamValue, 0, sizeof(acSetParamValue));
            if (isOperational == TRUE)
            {
                snprintf(acSetParamValue, sizeof(acSetParamValue), "Operational");
            }
            else
            {
                snprintf(acSetParamValue, sizeof(acSetParamValue), "NotOperational");
            }
            CcspTraceInfo(("%s-%d Param set %s\n",__FUNCTION__,__LINE__,acSetParamName));
            counter = 0;
            do
            {
                if (counter > 3) // retry 3 times
                {
                    break;
                }
                retval = SetParamValues(WAN_COMPONENT_NAME, WAN_DBUS_PATH, acSetParamName, acSetParamValue,ccsp_string,TRUE);
                sleep(1);
                ++counter;
            }while(retval == ANSC_STATUS_FAILURE);

            pWanCfg->MonitorOperStatusAndNotify = FALSE;
        }
    }
    return args;
}

void* ThreadUpdateInformMsg(void *args)
{
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject    = (PCOSA_DATAMODEL_CABLEMODEM)args;
    pthread_detach(pthread_self());
    if (pMyObject)
    {
        PCOSA_DML_CM_WANCFG pWanCfg = &pMyObject->CmWanCfg;
        if (pWanCfg)
        {
            INT iWanInstanceNumber = WAN_CM_INTERFACE_INSTANCE_NUM;
            BOOL bEthWanEnable = FALSE;
            CHAR wanName[64];
            CHAR out_value[64];
            CHAR acSetParamName[256];
            ANSC_STATUS retval = ANSC_STATUS_SUCCESS;

            if (pWanCfg->wanInstanceNumber)
            {
                iWanInstanceNumber = atoi(pWanCfg->wanInstanceNumber);
            }

            if ( 0 == access( "/nvram/ETHWAN_ENABLE" , F_OK ) )
            {
                bEthWanEnable = TRUE;
            }

            memset(out_value,0,sizeof(out_value));
            memset(wanName,0,sizeof(wanName));
            syscfg_get(NULL, "wan_physical_ifname", out_value, sizeof(out_value));

            if(0 != strnlen(out_value,sizeof(out_value)))
            {
                snprintf(wanName, sizeof(wanName), "%s", out_value);
            }
            else
            {
                snprintf(wanName, sizeof(wanName), "%s", WAN_PHY_NAME);
            }


            memset(acSetParamName, 0, sizeof(acSetParamName));
            snprintf(acSetParamName, sizeof(acSetParamName), WAN_INTERFACE_PARAM_NAME, iWanInstanceNumber);

            if (bEthWanEnable == TRUE)
            {
                retval = SetParamValues(WAN_COMPONENT_NAME, WAN_DBUS_PATH, acSetParamName, DOCSIS_INF_NAME,ccsp_string,TRUE);
            }
            else
            {
                retval = SetParamValues(WAN_COMPONENT_NAME, WAN_DBUS_PATH, acSetParamName, wanName,ccsp_string,TRUE);
            }
            if (retval != ANSC_STATUS_SUCCESS)
            {
                 CcspTraceError(("%s-%d Failed to set %s\n",__FUNCTION__,__LINE__,acSetParamName));
            }
        }
    }
    return args;   
}

ANSC_STATUS
CosaDmlCMWanMonitorPhyStatusAndNotify(void *arg)
{
    pthread_t CmPhyMonitorThreadId;
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)arg;
    pthread_create(&CmPhyMonitorThreadId, NULL, &ThreadMonitorPhyStatusAndNotify, pMyObject);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlCMWanMonitorOperStatusAndNotify(void *arg)
{
    pthread_t CmOperMonitorThreadId;
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)arg;
    pthread_create(&CmOperMonitorThreadId, NULL, &ThreadMonitorOperStatusAndNotify, pMyObject);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS CosaDmlCMUpdateInformMsgToWanMgr(void *arg)
{
    pthread_t CmInformMsgThreadId;
    PCOSA_DATAMODEL_CABLEMODEM      pMyObject = (PCOSA_DATAMODEL_CABLEMODEM)arg;
    pthread_create(&CmInformMsgThreadId, NULL, &ThreadUpdateInformMsg, pMyObject);
    return ANSC_STATUS_SUCCESS;
}

#endif

ANSC_STATUS
CosaDmlCMGetLoopDiagnosticsStart
    (
        ANSC_HANDLE                 hContext,
        BOOL*                       pBool
    )
{
   UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pBool);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlCMGetLoopDiagnosticsDetails
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    UNREFERENCED_PARAMETER(hContext);
    errno_t        rc = -1;
    if(!pValue){
	AnscTraceWarning(("Input parameter is NULL  %s, %d\n", __FUNCTION__, __LINE__));
	return ANSC_STATUS_FAILURE;
	}
   rc =  strcpy_s(pValue,PVALUE_MAX, "Dummy");
   if(rc != EOK)
    {
          ERR_CHK(rc);
          return ANSC_STATUS_FAILURE;
     }
 
     
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlCMGetTelephonyDHCPStatus
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    UNREFERENCED_PARAMETER(hContext);
    errno_t        rc = -1;
    if(!pValue){
	AnscTraceWarning(("Input parameter is NULL  %s, %d\n", __FUNCTION__, __LINE__));
	return ANSC_STATUS_FAILURE;
	}
    rc =  strcpy_s(pValue,PVALUE_MAX, "Dummy-InProgress");
    if(rc != EOK)
    {
          ERR_CHK(rc);
          return ANSC_STATUS_FAILURE;
     }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlCMGetTelephonyTftpStatus
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    UNREFERENCED_PARAMETER(hContext);
    errno_t        rc = -1;
    if(!pValue){
	AnscTraceWarning(("Input parameter is NULL  %s, %d\n", __FUNCTION__, __LINE__));
	return ANSC_STATUS_FAILURE;
	}
    rc = strcpy_s(pValue,PVALUE_MAX, "Dummy-InProgress");
    if(rc != EOK)
    {
          ERR_CHK(rc);
          return ANSC_STATUS_FAILURE;
     }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlCMGetTelephonyRegistrationStatus
    (
        ANSC_HANDLE                 hContext,
        char*                       pValue
    )
{
    UNREFERENCED_PARAMETER(hContext);
    errno_t        rc = -1;
    if(!pValue){
	AnscTraceWarning(("Input parameter is NULL  %s, %d\n", __FUNCTION__, __LINE__));
	return ANSC_STATUS_FAILURE;
	}
    rc =  strcpy_s(pValue,PVALUE_MAX, "Dummy-InProgress");
    if(rc != EOK)
    {
          ERR_CHK(rc);
          return ANSC_STATUS_FAILURE;
     }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlCMGetDHCPInfo
    (
        ANSC_HANDLE                 hContext,
        PCOSA_CM_DHCP_INFO          pInfo
    )
{
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
    UNREFERENCED_PARAMETER(pCfg);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
CosaDmlCmSetLog
    (
        ANSC_HANDLE                 hContext,
        PCOSA_DML_CM_LOG            pCfg
    )
{
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
    CMMGMT_CM_EventLogEntry_t entries[DOCSIS_EVENT_LOG_SIZE];
    int count = 0;
    int i;
    int cnt=0;
    PCOSA_DML_DOCSISLOG_FULL p;
    errno_t rc = -1;

    if((!pulCount) || (!ppConf)){
	AnscTraceWarning(("Input parameter is NULL  pulCount = %ln , ppConf = %p , %s, %d\n",pulCount, ppConf, __FUNCTION__, __LINE__));
	return ANSC_STATUS_FAILURE;
	}

    count = docsis_GetDocsisEventLogItems(entries, DOCSIS_EVENT_LOG_SIZE);

    p = (PCOSA_DML_DOCSISLOG_FULL)AnscAllocateMemory(count * sizeof(COSA_DML_DOCSISLOG_FULL));
    if(p == NULL) {
        AnscTraceWarning(("ReAllocateMemory error %s, %d\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }
    for(i=0;i<count;i++) {
     if (entries[i].docsDevEvFirstTime.tv_sec == 0 )
      continue;
        rc =   memcpy_s(p[cnt].Time,sizeof(p[cnt].Time), ctime(&(entries[i].docsDevEvFirstTime.tv_sec)), sizeof(p[cnt].Time));
         if(rc != EOK)
         {
            ERR_CHK(rc);
            free(p);
            return ANSC_STATUS_FAILURE;
         }   
        p[cnt].EventID = entries[i].docsDevEvId;
        p[cnt].EventLevel = entries[i].docsDevEvLevel;
        rc = memcpy_s(p[cnt].Description,sizeof(p[cnt].Description), entries[i].docsDevEvText,sizeof(entries[i].docsDevEvText));
        if(rc != EOK)
         {
            ERR_CHK(rc);
            free(p);
            return ANSC_STATUS_FAILURE;
         }
         cnt++;

    }

    *pulCount = cnt;
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
    UNREFERENCED_PARAMETER(hContext);
    if((!pulCount) || (!ppConf)){
	AnscTraceWarning(("Input parameter is NULL  pulCount = %ln , ppConf = %p , %s, %d\n",pulCount, ppConf, __FUNCTION__, __LINE__));
	return ANSC_STATUS_FAILURE;
	}
    /* Coverity Fix CID:79243 CHECKED_RETURN */
    if( docsis_GetNumOfActiveRxChannels(pulCount) != RETURN_OK )
    {
       AnscTraceWarning(("docsis_GetNumOfActiveRxChannels is not success:%s %d\n",__FUNCTION__, __LINE__));
    }
    else
    {
       AnscTraceWarning(("docsis_GetNumOfActiveRxChannels  is called successfully   %s, %d\n", __FUNCTION__, __LINE__));
    }
      
    if(*pulCount) {

        *ppConf = (PCOSA_CM_DS_CHANNEL)AnscAllocateMemory( sizeof(COSA_CM_DS_CHANNEL) * (*pulCount) );
         if(*ppConf == NULL)
         {
             return ANSC_STATUS_FAILURE;
         }
    
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
    UNREFERENCED_PARAMETER(hContext);
    if((!pulCount) || (!ppConf)){
	AnscTraceWarning(("Input parameter is NULL  pulCount = %ln , ppConf = %p , %s, %d\n",pulCount, ppConf, __FUNCTION__, __LINE__));
	return ANSC_STATUS_FAILURE;
	}
   /*Coverity Fix CID: 78775 CHECKED_RETURN */
    if( docsis_GetNumOfActiveTxChannels(pulCount) != RETURN_OK)
    { 
      AnscTraceWarning(("docsis_GetNumOfActiveTxChannels  is Not success: %s, %d\n", __FUNCTION__, __LINE__));
    } 
   else
   { 
     AnscTraceWarning(("docsis_GetNumOfActiveTxChannels  is called  successfully: %s, %d\n", __FUNCTION__, __LINE__));
   } 

    if(*pulCount) {

        *ppConf = (PCOSA_CM_US_CHANNEL)AnscAllocateMemory( sizeof(COSA_CM_US_CHANNEL) * (*pulCount) );
        if(*ppConf == NULL)
         {
             return ANSC_STATUS_FAILURE;
         }

    
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
    if((!pulCount) || (!ppConf)){
	AnscTraceWarning(("Input parameter is NULL  pulCount = %ln , ppConf = %p , %s, %d\n",pulCount, ppConf, __FUNCTION__, __LINE__));
	return ANSC_STATUS_FAILURE;
	}
    /*Coverity Fix CID:55875 CHECKED_RETURN */
    if( docsis_GetNumOfActiveRxChannels(pulCount) != RETURN_OK)
    {
        AnscTraceWarning(("docsis_GetNumOfActiveRxChannels  is Not success   %s, %d\n", __FUNCTION__, __LINE__));
    }
    else
    {
       AnscTraceWarning(("docsis_GetNumOfActiveRxChannels  is called successfully : %s, %d\n", __FUNCTION__, __LINE__));
    }
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
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
	UNREFERENCED_PARAMETER(hContext);
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
    UNREFERENCED_PARAMETER(hContext);
    char    LanMode[64] = {0};
    ULONG   size = 64;
    if((!pulInstanceNumber) || (!ppCPEList)){
	AnscTraceWarning(("Input parameter is NULL  pulInstanceNumber = %ln , ppCPEList = %p , %s, %d\n",pulInstanceNumber, ppCPEList, __FUNCTION__, __LINE__));
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
            /*Coverity Fix CID:79510 RESOURCE_LEAK */
            free(pInfo);
            return ANSC_STATUS_FAILURE;  
        }
        AnscCopyMemory(*ppCPEList, pInfo, sizeof(COSA_DML_CPE_LIST)*(*pulInstanceNumber));
        free(pInfo);
    }

    return ANSC_STATUS_SUCCESS;
}
