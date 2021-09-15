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

    module: cosa_X_CISCO_COM_CableModem_dml.h

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


#ifndef  _COSA_CM_DML_H
#define  _COSA_CM_DML_H

/**
 * @addtogroup CM_AGENT_APIS
 * @{
 */
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

/**
 * @brief This function checks for the parameter field and retrieves the parameter's boolean value associated with the datamodel
 * under X_CISCO_COM_CableModem.
 *
 * @param[in] hInsContext     Instance handle.
 * @param[in] ParamName       Parameter field.
 * @param[out] pBool          Parameter's boolean Value.
 *
 * @return  Returns TRUE once get the value, returns FALSE when receive unsupported parameter.
 */
BOOL
X_CISCO_COM_CableModem_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief This function checks for the parameter field and retrieves the parameter's integer value associated with the datamodel
 * under X_CISCO_COM_CableModem.
 *
 * @param[in] hInsContext     Instance handle.
 * @param[in] ParamName       Parameter field.
 * @param[out] pInt           Parameter's integer Value.
 *
 * @return  Returns TRUE once get the value, returns FALSE when receive unsupported parameter.
 */
BOOL
X_CISCO_COM_CableModem_GetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int*                        pInt
    );

/**
 * @brief This function checks for the parameter field and retrieves the value associated with the datamodel
 * under X_CISCO_COM_CableModem.
 *
 * @param[in] hInsContext     Instance handle.
 * @param[in] ParamName       Parameter field.
 * @param[in] pUlong          Parameter Value.
 *
 * @return  Returns TRUE once get the value, returns FALSE when receive unsupported parameter.
 */
BOOL
X_CISCO_COM_CableModem_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/**
 * @brief This function is called to retrieve String parameter value associated with the datamodel
 * under X_CISCO_COM_CableModem.
 *
 * @param[in] hInsContext   Object handle.
 * @param[in] ParamName     Parameter field to get.
 * @param[out] pValue       Parameter value.
 * @param[out] pUlSize      String length.
 *
 * @return  Returns 0 once get the value, Otherwise returns -1 on failure case.
 */
ULONG
X_CISCO_COM_CableModem_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief This function is called to set Boolean parameter value associated with the datamodel
 * under X_CISCO_COM_CableModem.
 *
 * @param[in] hInsContext   Object handle.
 * @param[in] ParamName     Parameter field to set.
 * @param[in] bValue        Parameter value to set.
 *
 * @return  Returns TRUE once set the value, returns FALSE when receive unsupported parameter to set.
 */
BOOL
X_CISCO_COM_CableModem_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

/**
 * @brief This function is called to set integer parameter value associated with the datamodel
 * under X_CISCO_COM_CableModem.
 *
 * @param[in] hInsContext   Object handle.
 * @param[in] ParamName     Parameter field to set.
 * @param[in] value         Parameter value to set.
 *
 * @return  Returns TRUE once set the value, returns FALSE when receive unsupported parameter to set.
 */
BOOL
X_CISCO_COM_CableModem_SetParamIntValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        int                         value
    );

/**
 * @brief This function is called to set Ulong value associated with the datamodel
 * under X_CISCO_COM_CableModem.
 *
 * @param[in] hInsContext   Object handle.
 * @param[in] ParamName     Parameter field to set.
 * @param[in] uValuepUlong  Parameter value to set.
 *
 * @return  Returns TRUE once set the value, returns FALSE when receive unsupported parameter to set.
 */
BOOL
X_CISCO_COM_CableModem_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValuepUlong
    );

/**
 * @brief This function is called to set String parameter value associated with the datamodel
 * under X_CISCO_COM_CableModem.
 *
 * @param[in] hInsContext   Object handle.
 * @param[in] ParamName     Parameter field to set.
 * @param[in] strValue      String Parameter value to set.
 *
 * @return  Returns TRUE once set the value, returns FALSE when receive unsupported parameter to set.
 */
BOOL
X_CISCO_COM_CableModem_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       strValue
    );

/**
 * @brief This function is called for validation. validation is for the input data of data model X_CISCO_COM_CableModem.
 *
 * @param[in] hInsContext          Object handle.
 * @param[in] pReturnParamName     The buffer (128 bytes) of parameter name if there's a validation.
 * @param[in] puLength             The output length of the param name.
 *
 * @return  Returns TRUE if there's no validation.
 */
BOOL
X_CISCO_COM_CableModem_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

/**
 * @brief This function is called to finally commit all the update associated with the datamodel
 * under X_CISCO_COM_CableModem.
 *
 * @param[in] hInsContext          Object handle.
 *
 * @return  Returns the status of the operation.
 */
ULONG
X_CISCO_COM_CableModem_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief This function is called to roll back the update whenever there's a validation found.
 * Rollback is done when validation fails.
 *
 * @param[in] hInsContext          Object handle.
 *
 * @return  Returns the status of the operation.
 */
ULONG
X_CISCO_COM_CableModem_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief This function is called to retrieve the log count associated with the DocsisLog datamodel.
 *
 * @param[in] hInsContext  Instance handle.
 *
 * @return  Returns the total count.
 */
ULONG
DocsisLog_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
DocsisLog_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief This function is called to retrieve the docsis log entry specified by the index associated with the DocsisLog datamodel.
 *
 * @param[in] hInsContext      Instance handle.
 * @param[in] nIndex           Index number to get docsis log entry information.
 * @param[in] pInsNumber       Output instance number.
 *
 * @return  Returns handle to identify the entry on success case, Otherwise returns NULL.
 */
ANSC_HANDLE
DocsisLog_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief This function is used to check whether the docsis log info table is updated or not
 * associated with the DocsisLog datamodel.
 *
 * @param[in] hInsContext    Instance handle.
 *
 * @return  Returns TRUE once updated.
 */
BOOL
DocsisLog_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief This function is called to synchronize the table info associated with the DocsisLog datamodel.
 *
 * @param[in] hInsContext  Instance handle.
 *
 * @return  Returns 0 once synchronized.
 */
ULONG
DocsisLog_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief This function checks for the parameter field and retrieves the value of docsis log info.
 * Details associated with the DocsisLog datamodel.
 *
 * @param[in] hInsContext     Instance handle.
 * @param[in] ParamName       Parameter field.
 * @param[in] pUlong          Parameter Value.
 *
 * @return  Returns TRUE once get the value, returns FALSE when receive unsupported parameter.
 */
BOOL
DocsisLog_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    );

/**
 * @brief This function is called to retrieve String parameter value of docsis log info
 * associated with the DocsisLog datamodel.
 *
 * @param[in] hInsContext   Object handle.
 * @param[in] ParamName     Parameter field to get.
 * @param[out] pValue       Parameter value.
 * @param[out] pUlSize      String length.
 *
 * @return  Returns 0 once get the value, Otherwise returns -1 on failure case.
 */
ULONG
DocsisLog_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief This function is called to retrieve the DS channel total count associated with DownstreamChannel datamodel.
 *
 * @param[in] hInsContext  Instance handle.
 *
 * @return  Returns the table count.
 */
ULONG
DownstreamChannel_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief This function is called to retrieve the DS channel entry specified by the index associated with DownstreamChannel datamodel.
 *
 * @param[in] hInsContext      Instance handle.
 * @param[in] nIndex           Index number to get US channel information.
 * @param[in] pInsNumber       Output instance number.
 *
 * @return  Returns handle of DS channel info on success case, Otherwise returns NULL.
 */
ANSC_HANDLE
DownstreamChannel_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief This function is used to check whether the DS table info is updated or not associated with DownstreamChannel datamodel.
 *
 * @param[in] hInsContext    Instance handle.
 *
 * @return  Returns TRUE once updated.
 */
BOOL
DownstreamChannel_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief This function is called to synchronize the table by getting DS channel info associated with DownstreamChannel datamodel.
 *
 * @param[in] hInsContext  Instance handle.
 *
 * @return  Returns 0 once synchronized.
 */
ULONG
DownstreamChannel_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief This function checks for the DS parameter field and retrieves the value associated with DownstreamChannel datamodel.
 *
 * @param[in] hInsContext     Instance handle.
 * @param[in] ParamName       Parameter field.
 * @param[in] pUlong          Parameter Value.
 *
 * @return  Returns TRUE once get the value, returns FALSE when receive unsupported parameter.
 */
BOOL
DownstreamChannel_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    );

/**
 * @brief This function is called to retrieve String parameter value associated with DownstreamChannel datamodel.
 *
 * @param[in] hInsContext   Object handle.
 * @param[in] ParamName     Parameter field to get.
 * @param[out] pValue       Parameter value.
 * @param[out] pUlSize      String length.
 *
 * @return  Returns 0 once get the value, Returns 1 if short of buffer size, Otherwise returns -1 when receive unsupported parameter.
 */
ULONG
DownstreamChannel_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief This function is called to retrieve the US channel total count associated with UpstreamChannel datamodel.
 *
 * @param[in] hInsContext  Instance handle.
 *
 * @return  Returns the table count.
 */
ULONG
UpstreamChannel_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief This function is called to retrieve the US channel entry specified by the index associated with UpstreamChannel datamodel.
 *
 * @param[in] hInsContext      Instance handle.
 * @param[in] nIndex           Index number to get US channel information.
 * @param[in] pInsNumber       Output instance number.
 *
 * @return  Returns handle of US channel info on success case, Otherwise returns NULL.
 */
ANSC_HANDLE
UpstreamChannel_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief This function is used to check whether the US table info is updated or not associated with UpstreamChannel datamodel.
 *
 * @param[in] hInsContext    Instance handle.
 *
 * @return  Returns TRUE once updated.
 */
BOOL
UpstreamChannel_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief This function is called to synchronize the table by getting US channel info associated with UpstreamChannel datamodel.
 *
 * @param[in] hInsContext  Instance handle.
 *
 * @return  Returns 0 once synchronized.
 */
ULONG
UpstreamChannel_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief This function checks for the US parameter field and retrieves the Ulong value associated with UpstreamChannel datamodel.
 *
 * @param[in] hInsContext     Instance handle.
 * @param[in] ParamName       Parameter field.
 * @param[in] pUlong          Parameter Value.
 *
 * @return  Returns TRUE once get the value, returns FALSE when receive unsupported parameter.
 */
BOOL
UpstreamChannel_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    );

/**
 * @brief This function is called to retrieve String parameter value associated with UpstreamChannel datamodel.
 *
 * @param[in] hInsContext   Object handle.
 * @param[in] ParamName     Parameter field to get.
 * @param[out] pValue       Parameter value.
 * @param[out] pUlSize      String length.
 *
 * @return  Returns 0 once get the value, Returns 1 if short of buffer size, Otherwise returns -1 when receive unsupported parameter.
 */
ULONG
UpstreamChannel_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/**
 * @brief This function is called to retrieve the CPE list total count associated with CPEList datamodel.
 *
 * @param[in] hInsContext  Instance handle.
 *
 * @return  Returns the table count.
 */
ULONG
CPEList_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief This function is called to retrieve the CPE list entry specified by the index associated with CPEList datamodel.
 *
 * @param[in] hInsContext      Instance handle.
 * @param[in] nIndex           Index number to get information.
 * @param[in] pInsNumber       Output instance number.
 *
 * @return  Returns handle of CPE list info on success case, Otherwise returns NULL.
 */
ANSC_HANDLE
CPEList_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief This function is used to check whether the CPE list table info is updated or not associated with CPEList datamodel.
 *
 * @param[in] hInsContext    Instance handle.
 *
 * @return  Returns TRUE once updated.
 */
BOOL
CPEList_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief This function is called to synchronize the table by getting CPE list info associated with CPEList datamodel.
 *
 * @param[in] hInsContext  Instance handle.
 *
 * @return  Returns 0 once synchronized.
 */
ULONG
CPEList_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief This function is called to retrieve String parameter value from DML CPE list associated with CPEList datamodel.
 *
 * @param[in] hInsContext   Object handle.
 * @param[in] ParamName     Parameter field to get.
 * @param[out] pValue       Parameter value.
 * @param[out] pUlSize      String length.
 *
 * @return  Returns 0 once get the value, Returns 1 if short of buffer size, Otherwise returns -1 when receive unsupported parameter.
 */
ULONG
CPEList_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

/** @} */  //END OF GROUP CM_AGENT_APIS
#endif
