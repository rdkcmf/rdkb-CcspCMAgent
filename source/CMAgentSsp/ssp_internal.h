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

#ifndef  _SSP_INTERNAL_
#define  _SSP_INTERNAL_

/**
 * @addtogroup CM_AGENT_TYPES
 * @{
 */

#define  CCSP_COMMON_COMPONENT_HEALTH_Red                   1
#define  CCSP_COMMON_COMPONENT_HEALTH_Yellow                2
#define  CCSP_COMMON_COMPONENT_HEALTH_Green                 3

#define  CCSP_COMMON_COMPONENT_STATE_Initializing           1
#define  CCSP_COMMON_COMPONENT_STATE_Running                2
#define  CCSP_COMMON_COMPONENT_STATE_Blocked                3
#define  CCSP_COMMON_COMPONENT_STATE_Paused                 3

#define  CCSP_COMMON_COMPONENT_FREERESOURCES_PRIORITY_High  1
#define  CCSP_COMMON_COMPONENT_FREERESOURCES_PRIORITY_Low   2


extern   PCCSP_COMPONENT_CFG                        gpPnmStartCfg;

typedef  struct
_COMPONENT_COMMON_DM
{
    char*                           Name;
    ULONG                           Version;
    char*                           Author;
    ULONG                           Health;
    ULONG                           State;

    BOOL                            LogEnable;
    ULONG                           LogLevel;

    ULONG                           MemMaxUsage;
    ULONG                           MemMinUsage;
    ULONG                           MemConsumed;
}
COMPONENT_COMMON_DM,  *PCOMPONENT_COMMON_DM;

#define ComponentCommonDmInit(component_common_dm)                                          \
        {                                                                                   \
            AnscZeroMemory(component_common_dm, sizeof(COMPONENT_COMMON_DM));               \
            component_common_dm->Name        = NULL;                                        \
            component_common_dm->Version     = 1;                                           \
            component_common_dm->Author      = NULL;                                        \
            component_common_dm->Health      = CCSP_COMMON_COMPONENT_HEALTH_Red;            \
            component_common_dm->State       = CCSP_COMMON_COMPONENT_STATE_Running;         \
            if(g_iTraceLevel >= CCSP_TRACE_LEVEL_EMERGENCY)                                 \
                component_common_dm->LogLevel = (ULONG) g_iTraceLevel;                     \
            component_common_dm->LogEnable   = TRUE;                                        \
            component_common_dm->MemMaxUsage = 0;                                           \
            component_common_dm->MemMinUsage = 0;                                           \
            component_common_dm->MemConsumed = 0;                                           \
        }


#define  ComponentCommonDmClean(component_common_dm)                                        \
         {                                                                                  \
            if ( component_common_dm->Name )                                                \
            {                                                                               \
                AnscFreeMemory(component_common_dm->Name);                                  \
            }                                                                               \
                                                                                            \
            if ( component_common_dm->Author )                                              \
            {                                                                               \
                AnscFreeMemory(component_common_dm->Author);                                \
            }                                                                               \
         }


#define  ComponentCommonDmFree(component_common_dm)                                         \
         {                                                                                  \
            ComponentCommonDmClean(component_common_dm);                                    \
            AnscFreeMemory(component_common_dm);                                            \
         }

/** @} */  //END OF GROUP CM_AGENT_TYPES


/**
 * @addtogroup CM_AGENT_APIS
 * @{
 */


/**
 * @brief This function is used to load/unload component, to get component memory info.
 *
 * @param[in] command  Argument to handle different use cases.
 *
 * @return  Returns 0 once executed the case.
 */
int  cmd_dispatch(int  command);

void load_data_model();

/**
 * @brief This function is used to create component common data model object and interface, also allocates memory for the context used by data model.
 *
 * @param[in] pStartCfg Configuration with component name to create.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on success case, else ANSC_STATUS_RESOURCES on failure.
 */
ANSC_STATUS
ssp_create_pnm
    (
        PCCSP_COMPONENT_CFG         pStartCfg
    );

/**
 * @brief This function is used to initiaze the whole system by registering ccsp data model.
 *
 * @param[in] pStartCfg Component configuration for registering data model.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on success, else failure error code from corresponding function call.
 */
ANSC_STATUS
ssp_engage_pnm
    (
        PCCSP_COMPONENT_CFG         pStartCfg
    );

/**
 * @brief This function is used to Unregister the component.
 *
 * @param[in] pStartCfg Component configurations to get component details.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on success.
 */
ANSC_STATUS
ssp_cancel_pnm
    (
        PCCSP_COMPONENT_CFG         pStartCfg
    );


ANSC_STATUS 
ssp_LoadCosaPluginLibrary
(
);


ANSC_STATUS
MessageBusTaskSim
    (
        ANSC_HANDLE hThisObject
    );

/**
 * @brief This function is used to retrive component name.
 *
 * @return  Returns Component name.
 */
char*
ssp_PnmCCDmGetComponentName
    (
        ANSC_HANDLE                     hThisObject
    );

/**
 * @brief This function is used to retrive component version.
 *
 * @return  Returns Component version.
 */
ULONG
ssp_PnmCCDmGetComponentVersion
    (
        ANSC_HANDLE                     hThisObject
    );

/**
 * @brief This function is used to retrive component author.
 *
 * @return  Returns Component author.
 */
char*
ssp_PnmCCDmGetComponentAuthor
    (
        ANSC_HANDLE                     hThisObject
    );

/**
 * @brief This function is used to retrive component Health.
 *
 * @return  Returns Component health such as CCSP_COMMON_COMPONENT_HEALTH_Red/CCSP_COMMON_COMPONENT_HEALTH_Yellow/CCSP_COMMON_COMPONENT_HEALTH_Green.
 */
ULONG
ssp_PnmCCDmGetComponentHealth
    (
        ANSC_HANDLE                     hThisObject
    );

/**
 * @brief This function is used to retrive component state.
 *
 * @return  Returns Component state such as CCSP_COMMON_COMPONENT_STATE_Initializing/CCSP_COMMON_COMPONENT_STATE_Running/CCSP_COMMON_COMPONENT_STATE_Blocked/CCSP_COMMON_COMPONENT_STATE_Paused.
 */
ULONG
ssp_PnmCCDmGetComponentState
    (
        ANSC_HANDLE                     hThisObject
    );

/**
 * @brief This function is used to get the logging enabled status.
 *
 * @return  Returns TRUE on enabled case.
 */
BOOL
ssp_PnmCCDmGetLoggingEnabled
    (
        ANSC_HANDLE                     hThisObject
    );

/**
 * @brief This function is used to enable logging.
 *
 * @param[in] bEnabled  Argument to enable tracing.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on success case.
 */
ANSC_STATUS
ssp_PnmCCDmSetLoggingEnabled
    (
        ANSC_HANDLE                     hThisObject,
        BOOL                            bEnabled
    );

/**
 * @brief This function is used to get trace level.
 *
 * @return  Returns trace level.
 */
ULONG
ssp_PnmCCDmGetLoggingLevel
    (
        ANSC_HANDLE                     hThisObject
    );

/**
 * @brief This function is used to configure trace level.
 *
 * @param[in] LogLevel  Argument to set log level.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on success.
 */
ANSC_STATUS
ssp_PnmCCDmSetLoggingLevel
    (
        ANSC_HANDLE                     hThisObject,
        ULONG                           LogLevel
    );

/**
 * @brief This function is used to get used peak memory info.
 *
 * @return  Returns Maximun memory used.
 */
ULONG
ssp_PnmCCDmGetMemMaxUsage
    (
        ANSC_HANDLE                     hThisObject
    );

/**
 * @brief This function is used to get used minumum memory info.
 *
 * @return  Returns Minimum memory used.
 */
ULONG
ssp_PnmCCDmGetMemMinUsage
    (
        ANSC_HANDLE                     hThisObject
    );

/**
 * @brief This function is used to get total memory consumed by the component.
 *
 * @return  Returns total memory consumed.
 */
ULONG
ssp_PnmCCDmGetMemConsumed
    (
        ANSC_HANDLE                     hThisObject
    );

/**
 * @brief This function is used to commit the parameter settings immediately.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on success.
 */
ANSC_STATUS
ssp_PnmCCDmApplyChanges
    (
        ANSC_HANDLE                     hThisObject
    );

/** @} */  //END OF GROUP CM_AGENT_APIS
#endif
