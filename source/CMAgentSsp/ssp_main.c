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

#ifdef __GNUC__
#if (!defined _BUILD_ANDROID) && (!defined _NO_EXECINFO_H_)
#include <execinfo.h>
#endif
#endif

#include "ssp_global.h"
#ifdef INCLUDE_BREAKPAD
#include "breakpad_wrapper.h"
#endif
#include "stdlib.h"
#include "ccsp_dm_api.h"
#ifdef USE_PCD_API_EXCEPTION_HANDLING
#include "pcdapi.h"
#endif
//#include <docsis_ext_interface.h>
#include "safec_lib_common.h"
#include "syscfg/syscfg.h"
#include <sys/stat.h>

#define DEBUG_INI_NAME "/etc/debug.ini"

#ifdef ENABLE_SD_NOTIFY
#include <systemd/sd-daemon.h>
#endif

#if (defined(INTEL_PUMA7))
#include "cap.h"
static cap_user appcaps;
#endif

#define WAN_DBUS_PATH                     "/com/cisco/spvtg/ccsp/wanmanager"
#define WAN_COMPONENT_NAME                "eRT.com.cisco.spvtg.ccsp.wanmanager"
#define WAN_COMP_NAME_WITHOUT_SUBSYSTEM "com.cisco.spvtg.ccsp.wanmanager"
#define WAN_CM_INTERFACE_INSTANCE_NUM      1
#define WAN_BOOTINFORM_CUSTOMCONFIG_PARAM_NAME "Device.X_RDK_WanManager.CPEInterface.%d.EnableCustomConfig"
#define WAN_BOOTINFORM_CONFIGWANENABLE_PARAM_NAME "Device.X_RDK_WanManager.CPEInterface.%d.ConfigureWanEnable"
#define WAN_BOOTINFORM_OPERSTATUSENABLE_PARAM_NAME "Device.X_RDK_WanManager.CPEInterface.%d.EnableOperStatusMonitor"
#define WAN_BOOTINFORM_PHYPATH_PARAM_NAME "Device.X_RDK_WanManager.CPEInterface.%d.Phy.Path"
#define WAN_BOOTINFORM_INTERFACE_PARAM_NAME "Device.X_RDK_WanManager.CPEInterface.%d.Wan.Name"
#define WAN_PHYIF_NAME_PRIMARY "erouter0"

#ifdef _COSA_BCM_ARM_
#define WAN_PHYIF_DOCSIS_NAME "cm0"
#elif defined(INTEL_PUMA7)
#define WAN_PHYIF_DOCSIS_NAME "dpdmta1"
#else
#define WAN_PHYIF_DOCSIS_NAME "cm0"
#endif

#define WAN_PHYPATH_VALUE "Device.X_CISCO_COM_CableModem."
#define WAN_NOE_PARAM_NAME                "Device.X_RDK_WanManager.CPEInterfaceNumberOfEntries"
#define WAN_IF_NAME_PARAM_NAME            "Device.X_RDK_WanManager.CPEInterface.%d.Name"

typedef struct _WAN_PARAM_INFO
{
    CHAR paramName[256];
    CHAR paramValue[256];
    enum dataType_e paramType;
}WAN_PARAM_INFO;

enum WANBOOTINFORM_MSG
{
    MSG_WAN_NAME = 0,
    MSG_PHY_PATH,
    MSG_OPER_STATUS,
    MSG_CUSTOMCONFIG,
    MSG_CONFIGURE_WAN,
    MSG_TOTAL_NUM
};
typedef struct _WAN_BOOTINFORM_MSG
{
    WAN_PARAM_INFO param[MSG_TOTAL_NUM];
    INT iNumOfParam;
    INT iWanInstanceNumber;
}WAN_BOOTINFORM_MSG;
/**
 * @defgroup CM_AGENT CM Agent
 * - Cable Modem Agent component that provides an abstraction layer software interface to third party WAN.
 * - Provides interfaces for integrating WAN interfaces with RDK-B.
 *
 * @defgroup CM_AGENT_TYPES Cable Modem Agent Types
 * @ingroup  CM_AGENT
 *
 * @defgroup CM_AGENT_APIS Cable Modem Agent APIs
 * @ingroup  CM_AGENT
 *
 **/

PDSLH_CPE_CONTROLLER_OBJECT     pDslhCpeController      = NULL;
PCOMPONENT_COMMON_DM            g_pComponent_Common_Dm  = NULL;
char                            g_Subsystem[32]         = {0};
PCCSP_COMPONENT_CFG             gpPnmStartCfg           = NULL;
PCCSP_FC_CONTEXT                pPnmFcContext           = (PCCSP_FC_CONTEXT           )NULL;
PCCSP_CCD_INTERFACE             pPnmCcdIf               = (PCCSP_CCD_INTERFACE        )NULL;
PCCC_MBI_INTERFACE              pPnmMbiIf               = (PCCC_MBI_INTERFACE         )NULL;
BOOL                            g_bActive               = FALSE;

#ifdef ENABLE_RDK_WANMANAGER
static pthread_t docsisclbk_tid;
pthread_t bootInformThreadId;
extern void RegisterDocsisCallback();
void* ThreadBootInformMsg(void *arg);
#endif
void
CcspBaseIf_deadlock_detection_log_print
(
    int sig
);

/**
 * @addtogroup CM_AGENT_APIS
 * @{
 */

/**
 * @brief This function validates user input to execute different use cases(Load CMAGENT MODULE / Get parameters / Get component memory info / Unregister component).
 *
 * @param[in] command  User Input command to handle different use cases.
 *
 * @return  Returns 0 on exit.
 */
int  cmd_dispatch(int  command)
{
    char*                           pParamNames[]      = {"Device.X_CISCO_COM_CableModem."};
    parameterValStruct_t**          ppReturnVal        = NULL;
    ULONG                           ulReturnValCount   = 0;
    ULONG                           i                  = 0;

    switch ( command )
    {
            case	'e' :

                CcspTraceInfo(("Connect to bus daemon...\n"));

            {
                char                            CName[256];

                if ( g_Subsystem[0] != 0 )
                {
                    _ansc_sprintf(CName, "%s%s", g_Subsystem, gpPnmStartCfg->ComponentId);
                }
                else
                {
                    _ansc_sprintf(CName, "%s", gpPnmStartCfg->ComponentId);
                }

                ssp_PnmMbi_MessageBusEngage
                    ( 
                        CName,
                        CCSP_MSG_BUS_CFG,
                        gpPnmStartCfg->DbusPath
                    );
            }


#ifdef ENABLE_RDK_WANMANAGER
                CcspTraceInfo(("pthread create boot inform \n"));
                pthread_create(&bootInformThreadId, NULL, &ThreadBootInformMsg, NULL);
#endif


                ssp_create_pnm(gpPnmStartCfg);
                ssp_engage_pnm(gpPnmStartCfg);

                g_bActive = TRUE;

                CcspTraceInfo(("CM Agent Module loaded successfully...\n"));

            break;

            case    'r' :

            CcspCcMbi_GetParameterValues
                (
                    DSLH_MPA_ACCESS_CONTROL_ACS,
                    pParamNames,
                    1,
                    (int *)&ulReturnValCount,
                    &ppReturnVal,
                    NULL
                );



            for ( i = 0; i < ulReturnValCount; i++ )
            {
                CcspTraceWarning(("Parameter %lu name: %s value: %s \n", i+1, ppReturnVal[i]->parameterName, ppReturnVal[i]->parameterValue));
            }


/*
            CcspCcMbi_GetParameterNames
                (
                    "Device.DeviceInfo.",
                    0,
                    &ulReturnValCount,
                    &ppReturnValNames
                );

            for ( i = 0; i < ulReturnValCount; i++ )
            {
                CcspTraceWarning(("Parameter %d name: %s bWritable: %d \n", i+1, ppReturnValNames[i]->parameterName, ppReturnValNames[i]->writable));
            }
*/
/*
            CcspCcMbi_GetParameterAttributes
                (
                    pParamNames,
                    1,
                    &ulReturnValCount,
                    &ppReturnvalAttr
                );
*/
/*
            CcspCcMbi_DeleteTblRow
                (
                    123,
                    "Device.X_CISCO_COM_SWDownload.SWDownload.1."
                );
*/

			break;

        case    'm':

                AnscPrintComponentMemoryTable(pComponentName);

                break;

        case    't':

                AnscTraceMemoryTable();

                break;

        case    'c':

                ssp_cancel_pnm(gpPnmStartCfg);

                break;

        default:
            break;
    }

    return 0;
}

static void _print_stack_backtrace(void)
{
#ifdef __GNUC__
#if (!defined _BUILD_ANDROID) && (!defined _NO_EXECINFO_H_) && (!defined _COSA_SIM_)
        void* tracePtrs[100];
        char** funcNames = NULL;
        int i, count = 0;

        int fd;
        const char* path = "/nvram/CMAgentSsp_backtrace";
        fd = open(path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
        if (fd < 0)
        {
            fprintf(stderr, "failed to open backtrace file: %s", path);
            return;
        }

        count = backtrace( tracePtrs, 100 );

        backtrace_symbols_fd( tracePtrs, count, fd);
   
        close(fd);
 
        funcNames = backtrace_symbols( tracePtrs, count );

        if ( funcNames ) {
            // Print the stack trace
            for( i = 0; i < count; i++ )
                printf("%s\n", funcNames[i] );

            // Free the string pointers
            free( funcNames );
        }
#endif
#endif
}

static void daemonize(void) {
	switch (fork()) {
	case 0:
		break;
	case -1:
		// Error
		CcspTraceInfo(("Error daemonizing (fork)! %d - %s\n", errno, strerror(
				errno)));
		exit(0);
		break;
	default:
		_exit(0);
	}

	if (setsid() < 	0) {
		CcspTraceInfo(("Error demonizing (setsid)! %d - %s\n", errno, strerror(errno)));
		exit(0);
	}

    /*
     *  What is the point to change current directory?
     *
    chdir("/");
     */

#ifndef  _DEBUG
	int fd;
	fd = open("/dev/null", O_RDONLY);
	if (fd != 0) {
		dup2(fd, 0);
		close(fd);
	}
	fd = open("/dev/null", O_WRONLY);
	if (fd != 1) {
		dup2(fd, 1);
		close(fd);
	}
	fd = open("/dev/null", O_WRONLY);
	if (fd != 2) {
		dup2(fd, 2);
		close(fd);
	}
#endif
}

/**
 * @brief This function is used to handle signal with the corresponding signal number.
 *
 * @param[in] sig  Signal number to handle.
 */
void sig_handler(int sig)
{
    CcspBaseIf_deadlock_detection_log_print(sig);

    if ( sig == SIGINT ) {
    	signal(SIGINT, sig_handler); /* reset it to this function */
    	CcspTraceInfo(("SIGINT received!\n"));
        exit(0);
    }
    else if ( sig == SIGUSR1 ) {
    	signal(SIGUSR1, sig_handler); /* reset it to this function */
    	CcspTraceInfo(("SIGUSR1 received!\n"));
    }
    else if ( sig == SIGUSR2 ) {
    	CcspTraceInfo(("SIGUSR2 received!\n"));
    }
    else if ( sig == SIGCHLD ) {
    	signal(SIGCHLD, sig_handler); /* reset it to this function */
    	CcspTraceInfo(("SIGCHLD received!\n"));
    }
    else if ( sig == SIGPIPE ) {
    	signal(SIGPIPE, sig_handler); /* reset it to this function */
    	CcspTraceInfo(("SIGPIPE received!\n"));
    }
    else if ( sig == SIGTERM )
    {
        CcspTraceInfo(("SIGTERM received!\n"));
        exit(0);
    }
    else if ( sig == SIGKILL )
    {
        CcspTraceInfo(("SIGKILL received!\n"));
        exit(0);
    }
	else if ( sig == SIGALRM ) {
		printf("~~~~ Got SIGALRM Signal !!! CMAgent~~~\n");
    	signal(SIGALRM, sig_handler); /* reset it to this function */
    	CcspTraceInfo(("SIGALRM received!\n"));		
    }
    else {
    	/* get stack trace first */
    	_print_stack_backtrace();
    	CcspTraceInfo(("Signal %d received, exiting!\n", sig));
    	exit(0);
    }
}

#ifndef INCLUDE_BREAKPAD
static int is_core_dump_opened(void)
{
    FILE *fp;
    char path[256];
    char line[1024];
    char *start, *tok, *sp;
#define TITLE   "Max core file size"
    snprintf(path, sizeof(path), "/proc/%d/limits", getpid());
    if ((fp = fopen(path, "rb")) == NULL)
        return 0;

    while (fgets(line, sizeof(line), fp) != NULL) {
        if ((start = strstr(line, TITLE)) == NULL)
            continue;

        start += strlen(TITLE);
        if ((tok = strtok_r(start, " \t\r\n", &sp)) == NULL)
            break;

        fclose(fp);
        return (tok[0] == '0' && tok[1] == '\0') ? 0 : 1;
    }

    fclose(fp);
    return 0;
}
#endif

#ifdef ENABLE_RDK_WANMANAGER
static void *GWP_docsisregistration_threadfunc(void *data)
{
if ( !data )
{
printf("\n");
 }           
pthread_detach(pthread_self());
CcspTraceError(("GWP_docsisregistration_threadfunc\n"));
RegisterDocsisCallback();
return NULL;
}

ANSC_STATUS SendMsg( char *pComponent, char *pBus, char *pParamName, char *pParamVal, enum dataType_e type, BOOLEAN bCommit )
{
    extern ANSC_HANDLE bus_handle;
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

ANSC_STATUS CosaDmlGetParamValues(char *pComponent, char *pBus, char *pParamName, char *pReturnVal)
{
    parameterValStruct_t **retVal;
    extern ANSC_HANDLE bus_handle;
    char *ParamName[1];
    int ret = 0,
        nval;

    //Assign address for get parameter name
    ParamName[0] = pParamName;

    ret = CcspBaseIf_getParameterValues(
        bus_handle,
        pComponent,
        pBus,
        ParamName,
        1,
        &nval,
        &retVal);

    //Copy the value
    if (CCSP_SUCCESS == ret)
    {
        CcspTraceWarning(("%s parameterValue[%s]\n", __FUNCTION__, retVal[0]->parameterValue));

        if (NULL != retVal[0]->parameterValue)
        {
            memcpy(pReturnVal, retVal[0]->parameterValue, strlen(retVal[0]->parameterValue) + 1);
        }

        if (retVal)
        {
            free_parameterValStruct_t(bus_handle, nval, retVal);
        }

        return ANSC_STATUS_SUCCESS;
    }

    if (retVal)
    {
        free_parameterValStruct_t(bus_handle, nval, retVal);
    }

    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS CosaDmlGetWanInstance(CHAR *pIfName, INT *pInstanceNumber)
{
    char acTmpReturnValue[256] = {0};
    INT iLoopCount = 0;
    INT iTotalNoofEntries = 0;


    if (NULL == pInstanceNumber|| NULL == pIfName)
    {
        CcspTraceError(("%s Invalid Buffer\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
    *pInstanceNumber = -1;
    if (ANSC_STATUS_FAILURE == CosaDmlGetParamValues(WAN_COMPONENT_NAME, WAN_DBUS_PATH, WAN_NOE_PARAM_NAME, acTmpReturnValue))
    {
        CcspTraceError(("%s %d Failed to get param value\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    //Total count
    iTotalNoofEntries = atoi(acTmpReturnValue);
    CcspTraceInfo(("%s %d - TotalNoofEntries:%d\n", __FUNCTION__, __LINE__, iTotalNoofEntries));

    if (0 >= iTotalNoofEntries)
    {
        return ANSC_STATUS_SUCCESS;
    }

    //Traverse from loop
    for (iLoopCount = 0; iLoopCount < iTotalNoofEntries; iLoopCount++)
    {
        char acTmpQueryParam[256] = {0};

        //Query
        snprintf(acTmpQueryParam, sizeof(acTmpQueryParam), WAN_IF_NAME_PARAM_NAME, iLoopCount + 1);

        memset(acTmpReturnValue, 0, sizeof(acTmpReturnValue));
        if (ANSC_STATUS_FAILURE == CosaDmlGetParamValues(WAN_COMPONENT_NAME, WAN_DBUS_PATH, acTmpQueryParam, acTmpReturnValue))
        {
            CcspTraceError(("%s %d Failed to get param value\n", __FUNCTION__, __LINE__));
            continue;
        }

        //Compare name
        if (0 == strcmp(acTmpReturnValue, pIfName))
        {
            *pInstanceNumber = iLoopCount + 1;
            break;
        }
    }

    return ANSC_STATUS_SUCCESS;
}

static void checkComponentHealthStatus(char * compName, char * dbusPath, char *status, int *retStatus)
{
    int ret = 0, val_size = 0;
    parameterValStruct_t **parameterval = NULL;
    extern ANSC_HANDLE bus_handle;
    char *parameterNames[1] = {};
    char tmp[256];
    char str[256];
    char l_Subsystem[128] = { 0 };

    sprintf(tmp,"%s.%s",compName, "Health");
    parameterNames[0] = tmp;

    strncpy(l_Subsystem, "eRT.",sizeof(l_Subsystem));
    snprintf(str, sizeof(str), "%s%s", l_Subsystem, compName);
    CcspTraceDebug(("str is:%s\n", str));

    ret = CcspBaseIf_getParameterValues(bus_handle, str, dbusPath,  parameterNames, 1, &val_size, &parameterval);
    CcspTraceDebug(("ret = %d val_size = %d\n",ret,val_size));
    if(ret == CCSP_SUCCESS)
    {
        CcspTraceDebug(("parameterval[0]->parameterName : %s parameterval[0]->parameterValue : %s\n",parameterval[0]->parameterName,parameterval[0]->parameterValue));
        strcpy(status, parameterval[0]->parameterValue);
        CcspTraceDebug(("status of component:%s\n", status));
    }
    free_parameterValStruct_t (bus_handle, val_size, parameterval);

    *retStatus = ret;
}

static void waitForWanMgrComponentReady()
{
    char status[32] = {'\0'};
    int count = 0;
    int ret = -1;
    while(1)
    {
        checkComponentHealthStatus(WAN_COMP_NAME_WITHOUT_SUBSYSTEM, WAN_DBUS_PATH, status,&ret);
        if(ret == CCSP_SUCCESS && (strcmp(status, "Green") == 0))
        {
            CcspTraceInfo(("%s component health is %s, continue\n", WAN_COMPONENT_NAME, status));
            break;
        }
        else
        {
            count++;
            if(count%5 == 0)
            {
                CcspTraceError(("%s component Health, ret:%d, waiting\n", WAN_COMPONENT_NAME, ret));
            }
            sleep(5);
        }
    }
}

INT InitBootInformInfo(WAN_BOOTINFORM_MSG *pMsg)
{
    BOOL bEthWanEnable = FALSE;
    CHAR wanName[64];
    CHAR out_value[64];
    CHAR acSetParamName[256];
    INT iWANInstance = 0;

    if (!pMsg)
        return -1;
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
        snprintf(wanName, sizeof(wanName), "%s", WAN_PHYIF_NAME_PRIMARY);
    }

    pMsg->iWanInstanceNumber = WAN_CM_INTERFACE_INSTANCE_NUM;
    pMsg->iNumOfParam = MSG_TOTAL_NUM;

    CosaDmlGetWanInstance(WAN_PHYIF_DOCSIS_NAME, &iWANInstance);

    if (-1 != iWANInstance)
    {
        pMsg->iWanInstanceNumber = iWANInstance;
    }

    memset(acSetParamName, 0, sizeof(acSetParamName));
    snprintf(acSetParamName, sizeof(acSetParamName), WAN_BOOTINFORM_INTERFACE_PARAM_NAME, WAN_CM_INTERFACE_INSTANCE_NUM);
    strncpy(pMsg->param[MSG_WAN_NAME].paramName,acSetParamName,sizeof(pMsg->param[MSG_WAN_NAME].paramName));
    pMsg->param[MSG_WAN_NAME].paramType = ccsp_string;

    if (bEthWanEnable == TRUE)
    {
        strncpy(pMsg->param[MSG_WAN_NAME].paramValue,WAN_PHYIF_DOCSIS_NAME,sizeof(pMsg->param[MSG_WAN_NAME].paramValue));
    }
    else
    {
        strncpy(pMsg->param[MSG_WAN_NAME].paramValue,wanName,sizeof(pMsg->param[MSG_WAN_NAME].paramValue));
    }
    memset(acSetParamName, 0, sizeof(acSetParamName));
    snprintf(acSetParamName, sizeof(acSetParamName), WAN_BOOTINFORM_PHYPATH_PARAM_NAME, WAN_CM_INTERFACE_INSTANCE_NUM);
    strncpy(pMsg->param[MSG_PHY_PATH].paramName,acSetParamName,sizeof(pMsg->param[MSG_PHY_PATH].paramName));
    strncpy(pMsg->param[MSG_PHY_PATH].paramValue,WAN_PHYPATH_VALUE,sizeof(pMsg->param[MSG_PHY_PATH].paramValue));
    pMsg->param[MSG_PHY_PATH].paramType = ccsp_string;

    memset(acSetParamName, 0, sizeof(acSetParamName));
    snprintf(acSetParamName, sizeof(acSetParamName), WAN_BOOTINFORM_OPERSTATUSENABLE_PARAM_NAME, WAN_CM_INTERFACE_INSTANCE_NUM);
    strncpy(pMsg->param[MSG_OPER_STATUS].paramName,acSetParamName,sizeof(pMsg->param[MSG_OPER_STATUS].paramName));
    strncpy(pMsg->param[MSG_OPER_STATUS].paramValue,"true",sizeof(pMsg->param[MSG_OPER_STATUS].paramValue));
    pMsg->param[MSG_OPER_STATUS].paramType = ccsp_boolean;

    memset(acSetParamName, 0, sizeof(acSetParamName));
    snprintf(acSetParamName, sizeof(acSetParamName), WAN_BOOTINFORM_CONFIGWANENABLE_PARAM_NAME, WAN_CM_INTERFACE_INSTANCE_NUM);
    strncpy(pMsg->param[MSG_CONFIGURE_WAN].paramName,acSetParamName,sizeof(pMsg->param[MSG_CONFIGURE_WAN].paramName));
    strncpy(pMsg->param[MSG_CONFIGURE_WAN].paramValue,"false",sizeof(pMsg->param[MSG_CONFIGURE_WAN].paramValue));
    pMsg->param[MSG_CONFIGURE_WAN].paramType = ccsp_boolean;

    memset(acSetParamName, 0, sizeof(acSetParamName));
    snprintf(acSetParamName, sizeof(acSetParamName), WAN_BOOTINFORM_CUSTOMCONFIG_PARAM_NAME, WAN_CM_INTERFACE_INSTANCE_NUM);
    strncpy(pMsg->param[MSG_CUSTOMCONFIG].paramName,acSetParamName,sizeof(pMsg->param[MSG_CUSTOMCONFIG].paramName));
    strncpy(pMsg->param[MSG_CUSTOMCONFIG].paramValue,"false",sizeof(pMsg->param[MSG_CUSTOMCONFIG].paramValue));
    pMsg->param[MSG_CUSTOMCONFIG].paramType = ccsp_boolean;
        
    return 0;
}

void* ThreadBootInformMsg(void *arg)
{
    WAN_BOOTINFORM_MSG msg = {0};
    INT retryMax = 60;
    INT retryCount = 0;
    BOOL retryBootInform = FALSE;
    pthread_detach(pthread_self());
    waitForWanMgrComponentReady();
    InitBootInformInfo(&msg);
    while (1)
    {
        INT index = 0;
        for (index = 0; index < msg.iNumOfParam; ++index)
        {
            ANSC_STATUS ret = SendMsg(
                    WAN_COMPONENT_NAME,
                    WAN_DBUS_PATH,
                    msg.param[index].paramName,
                    msg.param[index].paramValue,
                    msg.param[index].paramType,
                    TRUE);
            if (ret == ANSC_STATUS_FAILURE)
            {
                retryBootInform = TRUE;
                break;
            }
        }
        if (FALSE == retryBootInform)
        {
            break;
        }
        if (retryCount > retryMax)
            break;
        ++retryCount;
        sleep(1);
    }
    return arg;
}

#endif
#if (defined(INTEL_PUMA7))
static bool drop_root()
{
    appcaps.caps = NULL;
    appcaps.user_name = NULL;
    bool ret = false;
    bool blocklist_ret = false;
    blocklist_ret = isBlocklisted();
    if(blocklist_ret)
    {
      CcspTraceInfo(("NonRoot feature is disabled\n"));
    }
    else
    {
    CcspTraceInfo(("NonRoot feature is enabled, dropping root privileges for CcspCMAgentSsp Process\n"));
    if(init_capability() != NULL) {
       if(drop_root_caps(&appcaps) != -1) {
          if(update_process_caps(&appcaps) != -1) {
             read_capability(&appcaps);
             ret = true;
          }
       }
    }
    }
    return ret;
}
#endif

int main(int argc, char* argv[])
{
    int                             cmdChar            = 0;
    BOOL                            bRunAsDaemon       = TRUE;
    int                             idx                = 0;
    char                            cmd[1024]          = {0};
    FILE                           *fd                 = NULL;
    DmErr_t                         err;
    char                            *subSys            = NULL;
    extern ANSC_HANDLE bus_handle;
    errno_t        rc = -1;
    int ind = -1;
	
	#ifdef FEATURE_SUPPORT_RDKLOG
	rdk_logger_init(DEBUG_INI_NAME);
	#endif

    /*
     *  Load the start configuration
     */
    gpPnmStartCfg = (PCCSP_COMPONENT_CFG)AnscAllocateMemory(sizeof(CCSP_COMPONENT_CFG));
    
    if ( gpPnmStartCfg )
    {
        CcspComponentLoadCfg(CCSP_PNM_START_CFG_FILE, gpPnmStartCfg);
    }
    else
    {
        printf("Insufficient resources for start configuration, quit!\n");
        exit(1);
    }
    
    /* Set the global pComponentName */
    pComponentName = gpPnmStartCfg->ComponentName;

    AnscSetTraceLevel(CCSP_TRACE_LEVEL_INFO);
    
#if defined(_DEBUG) && defined(_COSA_SIM_)
    AnscSetTraceLevel(CCSP_TRACE_LEVEL_INFO);
#endif

    for (idx = 1; idx < argc; idx++)
    {
         rc = strcmp_s("-subsys",strlen("-subsys"),argv[idx],&ind);
         ERR_CHK(rc);
         if((!ind) && (rc == EOK))
        {
           if ((idx+1) < argc)
           {
              rc = strcpy_s(g_Subsystem,sizeof(g_Subsystem), argv[idx+1]);
              if(rc != EOK)
             {
               ERR_CHK(rc);
               return ANSC_STATUS_FAILURE;
             }
           }
           else
           {
               CcspTraceError(("parameter after -subsys is missing"));
           }
             
        }
        else
        {
           rc = strcmp_s("-c", strlen("-c"),argv[idx],&ind );
           ERR_CHK(rc);
           if((!ind) && (rc == EOK))
          {
            bRunAsDaemon = FALSE;
          }
       }

    }

#if (defined(INTEL_PUMA7))
    if(!drop_root()) {
       CcspTraceInfo(("Dropping root privileges for CcspCMAgentSsp is failed\n"));
    }
#endif
    if ( bRunAsDaemon )
        daemonize();

    /*This is used for ccsp recovery manager and systemd*/
    fd = fopen("/var/tmp/CcspCMAgentSsp.pid", "w+");
    if ( !fd )
    {
        CcspTraceWarning(("Create /var/tmp/CcspCMAgentSsp.pid error. \n"));
        return 1;
    }
    sprintf(cmd, "%d", getpid());
    fputs(cmd, fd);
    fclose(fd);

#ifdef INCLUDE_BREAKPAD
    breakpad_ExceptionHandler();
    signal(SIGUSR1, sig_handler);	
#else

    if (is_core_dump_opened())
    {
        signal(SIGUSR1, sig_handler);
        CcspTraceWarning(("Core dump is opened, do not catch signal\n"));
    }
    else
    {
        CcspTraceWarning(("Core dump is NOT opened, backtrace if possible\n"));
    signal(SIGTERM, sig_handler);
    signal(SIGINT, sig_handler);
    signal(SIGUSR1, sig_handler);
    signal(SIGUSR2, sig_handler);

    signal(SIGSEGV, sig_handler);
    signal(SIGBUS, sig_handler);
    signal(SIGKILL, sig_handler);
    signal(SIGFPE, sig_handler);
    signal(SIGILL, sig_handler);
    signal(SIGQUIT, sig_handler);
    signal(SIGHUP, sig_handler);
	signal(SIGALRM, sig_handler);
    }
#endif

#ifdef USE_PCD_API_EXCEPTION_HANDLING
    printf("Registering PCD exception handler for CcspCMAgent\n");
    PCD_api_register_exception_handlers( argv[0], NULL );
#endif

#ifdef ENABLE_RDK_WANMANAGER
    CcspTraceInfo(("pthread create docsis registration\n"));
    pthread_create(&docsisclbk_tid, NULL, GWP_docsisregistration_threadfunc, NULL);
#endif
	
    cmd_dispatch('e');
    // printf("Calling Docsis\n");

    // ICC_init();
    // DocsisIf_StartDocsisManager();

#ifdef _COSA_SIM_
    subSys = "";        /* PC simu use empty string as subsystem */
#else
    subSys = NULL;      /* use default sub-system */
#endif
    err = Cdm_Init(bus_handle, subSys, NULL, NULL, pComponentName);
    if (err != CCSP_SUCCESS)
    {
        fprintf(stderr, "Cdm_Init: %s\n", Cdm_StrError(err));
        exit(1);
    }

#ifdef ENABLE_SD_NOTIFY
    sd_notifyf(0, "READY=1\n"
              "STATUS=CcspCMAgent is Successfully Initialized\n"
              "MAINPID=%lu", (unsigned long) getpid());
  
    CcspTraceInfo(("RDKB_SYSTEM_BOOT_UP_LOG : CcspCMAgent sd_notify Called\n"));
#endif

    creat("/tmp/cm_initialized",S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

    printf("Entering CM loop\n");
    CcspTraceWarning(("RDKB_SYSTEM_BOOT_UP_LOG : Entering CM loop\n"));

    if ( bRunAsDaemon )
    {
        while(1)
        {
            sleep(30);
        }
    }
    else
    {
        while ( cmdChar != 'q' )
        {
            cmdChar = getchar();

            cmd_dispatch(cmdChar);
        }
    }

    err = Cdm_Term();
    if (err != CCSP_SUCCESS)
    {
        fprintf(stderr, "Cdm_Term: %s\n", Cdm_StrError(err));
        exit(1);
    }

    if ( g_bActive )
    {
        ssp_cancel_pnm(gpPnmStartCfg);

        g_bActive = FALSE;
    }

    return 0;
}
/** @} */  //END OF GROUP CM_AGENT_APIS

