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

    module: cosa_device_info_dml.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        01/05/2012    initial revision.

**************************************************************************/


#ifndef  _COSA_DI_DML_H
#define  _COSA_DI_DML_H

/***********************************************************************

 APIs for Object:

    DeviceInfo.

    *  DeviceInfo_GetParamBoolValue
    *  DeviceInfo_GetParamStringValue
    *  DeviceInfo_SetParamBoolValue
    *  DeviceInfo_SetParamStringValue

***********************************************************************/
/**
 * @addtogroup CM_AGENT_APIS
 * @{
 */

/**
 * @brief This function is called to retrieve Boolean parameter value associated with the datamodel
 * under Device.DeviceInfo. which was invoked.
 *
 * @param[in] hInsContext   Object handle.
 * @param[in] ParamName     Parameter field to get.
 * @param[out] pBool        Parameter value.
 *
 * @return  Returns TRUE once get the value, returns FALSE when receive unsupported parameter to retrieve.
 */
BOOL
DeviceInfo_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief This function is called to retrieve String parameter value associated with the datamodel
 * under Device.DeviceInfo. which was invoked.
 *
 * @param[in] hInsContext   Object handle.
 * @param[in] ParamName     Parameter field to get.
 * @param[out] pValue       Parameter value.
 * @param[out] pUlSize      String length.
 *
 * @return  Returns 1 once get the value.
 */
ULONG
DeviceInfo_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief This function is called to set Boolean parameter value associated with the datamodel
 * under Device.DeviceInfo. which was invoked.
 *
 * @param[in] hInsContext   Object handle.
 * @param[in] ParamName     Parameter field to set.
 * @param[in] bValue        Parameter value to set.
 *
 * @return  Returns TRUE once set the value, returns FALSE when receive unsupported parameter to set.
 */
BOOL
DeviceInfo_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief This function is called to retrieve String parameter value associated with the datamodel
 * under Device.DeviceInfo. which was invoked.
 *
 * @param[in] hInsContext   Object handle.
 * @param[in] ParamName     Parameter field to set.
 * @param[in] pString       Parameter value to set.
 *
 * @return  Returns TRUE once set the value, returns FALSE when receive unsupported parameter to set.
 */
BOOL
DeviceInfo_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    );	

/** @} */  //END OF GROUP CM_AGENT_APIS
#endif
