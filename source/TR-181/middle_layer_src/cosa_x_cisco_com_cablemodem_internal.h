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

    module: cosa_deviceinfo_internal.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        01/14/2011    initial revision.

**************************************************************************/

#ifndef  _COSA_CM_INTERNAL_H
#define  _COSA_CM_INTERNAL_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "cosa_x_cisco_com_cablemodem_apis.h"

/**
 * @addtogroup CM_AGENT_TYPES
 * @{
 */
#define COSA_IFSTACK_ALIAS_LENGTH              64
#define CM_REFRESH_INTERVAL                    20
#define TIME_NO_NEGATIVE(x) ((long)(x) < 0 ? 0 : (x))

#define  COSA_DATAMODEL_CABLEMODEM_CLASS_CONTENT                            \
    /* duplication of the base object class content */                      \
    COSA_BASE_CONTENT                                                       \
    /* start of Lan config security object class content */                 \
    BOOLEAN LoopDiagnosticsStart;                                           \
    COSA_DML_CM_LOG                 CmLog;                                               \
    COSA_DML_CM_WANCFG              CmWanCfg;                                            \
    ULONG                           CMErrorCodewordsNumber;                              \
    ULONG                           CMErrorCodewordsUpdateTime;                          \
    PCOSA_DML_CMERRORCODEWORDS_FULL pCMErrorCodewords;                                   \
    ULONG                           DocsisLogNumber;                                     \
    ULONG                           DocsisLogUpdateTime;                                 \
    PCOSA_DML_DOCSISLOG_FULL        pDocsisLog;                                          \
    ULONG                           DownstreamChannelNumber;                             \
    PCOSA_CM_DS_CHANNEL             pDownstreamChannel;                                  \
    ULONG                           DownstreamChannelUpdateTime;                         \
    ULONG                           UpstreamChannelNumber;                               \
    PCOSA_CM_US_CHANNEL             pUpstreamChannel;                                    \
    ULONG                           UpstreamChannelUpdateTime;                           \
    char                            MDDIPOverride[64];                                   \
    ULONG                           CPEListNumber;                                       \
    PCOSA_DML_CPE_LIST              pCPEList;                                              \
    ULONG                           CPEListUpdateTime;                           \

typedef  struct
_COSA_DATAMODEL_CABLDMODEM_CLASS_CONTENT
{
    COSA_DATAMODEL_CABLEMODEM_CLASS_CONTENT
}
COSA_DATAMODEL_CABLEMODEM, *PCOSA_DATAMODEL_CABLEMODEM;
/** @} */  //END OF GROUP CM_AGENT_TYPES

/*
    Standard function declaration
*/
/**
 * @addtogroup CM_AGENT_APIS
 * @{
 */

/**
 * @brief This function is used to create CMAgent object and return handle.
 *
 * @return  Returns newly created object handle.
 */
ANSC_HANDLE
CosaCableModemCreate
    (
        VOID
    );

/**
 * @brief This function is used to Initialize middle layer for created CMAgent object Device.DeviceInfo.
 *
 * @param[in] hThisObject  Object handle.
 *
 * @return  Returns status of the operation.
 */
ANSC_STATUS
CosaCableModemInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief This function is used to remove CMAgent object(Device.DeviceInfo).
 *
 * @param[in] hThisObject  Object handle to delete.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on success case.
 */
ANSC_STATUS
CosaCableModemRemove
    (
        ANSC_HANDLE                 hThisObject
    );

/** @} */  //END OF GROUP CM_AGENT_APIS
#endif
