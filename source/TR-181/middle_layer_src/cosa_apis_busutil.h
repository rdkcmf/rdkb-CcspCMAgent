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


/**********************************************************************

    module: dslh_dmagnt_exported.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the framework's exported functions
        by the Dslh DataModelAgent object;

        *   CosaGetParamValueUlong2
        *   CosaGetParamValueString
        *   CosaGetParamValueBool
        *   CosaGetInstanceNumberByIndex

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        01/06/2011    initial revision.
        01/11/2011    added SLAP related apis.
        03/21/2011    added api to retrieve instance number by index

**********************************************************************/
#ifndef  _COSA_APIS_BUSUTIL_H
#define  _COSA_APIS_BUSUTIL_H

/**
 * @addtogroup CM_AGENT_APIS
 * @{
 */

/**
 * @brief This function is called to retrieve a UONG value of a parameter.
 *
 * @param[in] pParamName     Parameter name.
 *
 * @return  Returns the Ulong parameter value.
 */
ULONG
CosaGetParamValueUlong
    (
        char*                       pParamName
    );

/**
 * @brief This function is called to retrieve String parameter value.
 *
 * @param[in] pParamName     Parameter field to get.
 * @param[out] pBuffer       Buffer space for the value.
 * @param[out] pulSize       Buffer size.
 *
 * @return  Returns the status of the operation.
 */
int
CosaGetParamValueString
    (
        char*                       pParamName,
        char*                       pBuffer,
        PULONG                      pulSize
    );

/**
 * @brief This function is called to get Boolean parameter value.
 *
 * @param[in] pParamName     Parameter field to get.
 *
 * @return  Returns the boolean value of the requested parameter field.
 */
BOOL
CosaGetParamValueBool
    (
        char*                       pParamName
    );

/**
 * @brief This function is used to retrieve the instance number by index value.
 *
 * @param[in] pObjName     Object name.
 * @param[in] ulIndex      Index value to get instance number.
 *
 * @return  Returns the instance number of the corresponding index value.
 */
ULONG
CosaGetInstanceNumberByIndex
    (
        char*                      pObjName,
        ULONG                      ulIndex
    );

/** @} */  //END OF GROUP CM_AGENT_APIS
#endif
