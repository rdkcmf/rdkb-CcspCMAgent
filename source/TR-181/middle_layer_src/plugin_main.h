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


/**********************************************************************

    module: plugin_main.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the exported apis for SNMP plugin.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        06/25/03    initial revision.

**********************************************************************/


#ifndef  _PLUGIN_MAIN_H
#define  _PLUGIN_MAIN_H



#define ANSC_EXPORT_API

#ifdef __cplusplus 
extern "C"{
#endif

/***************************************************************************
 *
 *  BMEL stands for "Broadway MIB Extension Library"
 *
 ***************************************************************************/

/**
 * @addtogroup CM_AGENT_APIS
 * @{
 */

/**
 * @brief Cosa INIT API.
 *
 * @param[in] uMaxVersionSupported     Maximum supported plugin version.
 * @param[in] hCosaPlugInfo            Plugin info.
 *
 * @return  Returns the status of the operation.
 */
int ANSC_EXPORT_API
COSA_Init
    (
        ULONG                       uMaxVersionSupported, 
        void*                       hCosaPlugInfo         /* PCOSA_PLUGIN_INFO passed in by the caller */
    );

/**
 * @brief Cosa Asynchronous INIT API.
 *
 * @param[in] uMaxVersionSupported     Maximum supported plugin version.
 * @param[in] hCosaPlugInfo            Plugin info.
 *
 * @return  Returns the status of the operation.
 */
int ANSC_EXPORT_API
COSA_Async_Init
    (
        ULONG                       uMaxVersionSupported, 
        void*                       hCosaPlugInfo         /* PCOSA_PLUGIN_INFO passed in by the caller */
    );

/**
 * @brief This function is used to find the supported object.
 *
 * @param[in] pObjName     Object name.
 *
 * @return  Returns the status of the operation.
 */
BOOL ANSC_EXPORT_API
COSA_IsObjectSupported
    (
        char*                        pObjName
    );

/**
 * @brief This function is used to unload all the BE manager object created.
 *
 * @return  Returns the status of the operation.
 */
void ANSC_EXPORT_API
COSA_Unload
    (
        void
    );

/**
 * @brief This function is used to get all the memory details.
 */
void ANSC_EXPORT_API
COSA_MemoryCheck
    (
        void
    );

/**
 * @brief This function is used to get all the memory usage details.
 */
void ANSC_EXPORT_API
COSA_MemoryUsage
    (
        void
    );

/**
 * @brief This function is used to get all the memory table informations maintained.
 */
void ANSC_EXPORT_API
COSA_MemoryTable
    (
        void
    );

/** @} */  //END OF GROUP CM_AGENT_APIS

#ifdef __cplusplus 
}
#endif
#endif
