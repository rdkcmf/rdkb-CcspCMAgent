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

    module: cosa_rdkcentral_com_internal.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        08/09/2017    initial revision.

**************************************************************************/

#ifndef  _COSA_RDKCENTRAL_CM_INTERNAL_H
#define  _COSA_RDKCENTRAL_CM_INTERNAL_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "cosa_x_rdkcentral_com_cablemodem_apis.h"

/**
 * @addtogroup CM_AGENT_TYPES
 * @{
 */
#define COSA_IFSTACK_ALIAS_LENGTH              64
#define RDKCM_REFRESH_INTERVAL                 20
#define TIME_NO_NEGATIVE(x) 				   ((long)(x) < 0 ? 0 : (x))

#define  COSA_DATAMODEL_RDKCENTRAL_CABLEMODEM_CLASS_CONTENT                            \
    /* duplication of the base object class content */                      \
    COSA_BASE_CONTENT                                                       \
    /* start of Lan config security object class content */                 \
    ULONG										DsOfdmChannelUpdateTime;						 \
    ULONG                           			DsOfdmChannelTotalNumbers;                         \
    PCOSA_X_RDKCENTRAL_COM_CM_DS_OFDM_CHAN 		pDsOfdmChannel;                                   \
    ULONG										UsOfdmChannelUpdateTime;						 \
    ULONG                           			UsOfdmChannelTotalNumbers;                         \
    PCOSA_X_RDKCENTRAL_COM_CM_US_OFDMA_CHAN 	pUsOfdmChannel;                                   \
    ULONG										CMStatusofUsChannelUpdateTime;						 \
    ULONG                           			CMStatusofUsChannelTotalNumbers;                         \
    PCOSA_X_RDKCENTRAL_COM_CMSTATUSOFDMA_US 	pCMStatusofUsChannel;                                   \

typedef  struct 
_COSA_DATAMODEL_RDKCENTRAL_CABLDMODEM_CLASS_CONTENT
{
    COSA_DATAMODEL_RDKCENTRAL_CABLEMODEM_CLASS_CONTENT
}
COSA_DATAMODEL_RDKCENTRAL_CABLEMODEM, *PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM;
/** @} */  //END OF GROUP CM_AGENT_TYPES

/*
    Standard function declaration
*/
/**
 * @addtogroup CM_AGENT_APIS
 * @{
 */

/**
 * @brief This function is used to create CMAgent object in its DML layer.
 *
 * @return  Returns newly created object handle.
 */
ANSC_HANDLE
CosaRDKCentralComCableModemCreate
    (
        VOID
    );

/**
 * @brief This function is used to Initialize middle layer for created CMAgent object in its DML layer(Device.X_RDKCENTRAL-COM_CableModem).
 *
 * @param[in] hThisObject  Object handle.
 *
 * @return  Returns status of the operation.
 */
ANSC_STATUS
CosaRDKCentralComCableModemInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

/**
 * @brief This function is used to remove CMAgent object(Device.X_RDKCENTRAL-COM_CableModem).
 *
 * @param[in] hThisObject  Object handle to delete.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on success case, Otherwise returns ANSC_STATUS_FAILURE.
 */
ANSC_STATUS
CosaRDKCentralComCableModemRemove
    (
        ANSC_HANDLE                 hThisObject
    );

/** @} */  //END OF GROUP CM_AGENT_APIS
#endif /* _COSA_RDKCENTRAL_CM_INTERNAL_H */
