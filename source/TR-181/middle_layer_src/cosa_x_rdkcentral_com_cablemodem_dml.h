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

    module: cosa_x_rdkcentral_com_cablemodem_dml.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        08/09/2017   initial revision.

**************************************************************************/


#ifndef  _COSA_RDKCENTRAL_CM_DML_H
#define  _COSA_RDKCENTRAL_CM_DML_H

/**
 * @addtogroup CM_AGENT_APIS
 * @{
 */
/***********************************************************************

 APIs for Object:

    X_RDKCENTRAL-COM_CableModem.DsOfdmChan

    *  X_RDKCENTRAL_COM_CableModem_DsOfdmChan_IsUpdated
    *  X_RDKCENTRAL_COM_CableModem_DsOfdmChan_Synchronize    
    *  X_RDKCENTRAL_COM_CableModem_DsOfdmChan_GetEntryCount    
    *  X_RDKCENTRAL_COM_CableModem_DsOfdmChan_GetEntry        
    *  X_RDKCENTRAL_COM_CableModem_DsOfdmChan_GetParamUlongValue

***********************************************************************/
/**
 * @brief This function is called to retrieve the count of the table associated with X_RDKCENTRAL_COM_CableModem_DsOfdmChan datamodel.
 *
 * @param[in] hInsContext  Instance handle.
 *
 * @return  Returns the table count.
 */
ULONG
X_RDKCENTRAL_COM_CableModem_DsOfdmChan_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief This function is called to retrieve the entry specified by the index associated with X_RDKCENTRAL_COM_CableModem_DsOfdmChan datamodel.
 *
 * @param[in] hInsContext      Instance handle.
 * @param[in] nIndex           Index number to get DS channel information.
 * @param[in] pInsNumber       Output instance number.
 *
 * @return  Returns handle of DS channel info on success case, Otherwise returns NULL.
 */
ANSC_HANDLE
X_RDKCENTRAL_COM_CableModem_DsOfdmChan_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );


/**
 * @brief This function is used to check whether the table is updated or not associated with X_RDKCENTRAL_COM_CableModem_DsOfdmChan datamodel.
 *
 * @param[in] hInsContext    Instance handle.
 *
 * @return  Returns TRUE once updated.
 */
BOOL
X_RDKCENTRAL_COM_CableModem_DsOfdmChan_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief This function is called to synchronize the table by getting DS OFDM channel info by HAL API's. API associated with
 * X_RDKCENTRAL_COM_CableModem_DsOfdmChan datamodel.
 *
 * @param[in] hInsContext  Instance handle.
 *
 * @return  Returns 0 once synchronized.
 */
ULONG
X_RDKCENTRAL_COM_CableModem_DsOfdmChan_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief This function checks for the parameter field and retrieves the value associated with X_RDKCENTRAL_COM_CableModem_DsOfdmChan datamodel.
 *
 * @param[in] hInsContext     Instance handle.
 * @param[in] ParamName       Parameter field.
 * @param[in] pUlong          Parameter Value.
 *
 * @return  Returns TRUE once get the value, returns FALSE when receive unsupported parameter.
 */
BOOL
X_RDKCENTRAL_COM_CableModem_DsOfdmChan_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/***********************************************************************

 APIs for Object:

    X_RDKCENTRAL-COM_CableModem.UsOfdmaChan

    *  X_RDKCENTRAL_COM_CableModem_UsOfdmaChan_IsUpdated
    *  X_RDKCENTRAL_COM_CableModem_UsOfdmaChan_Synchronize        
    *  X_RDKCENTRAL_COM_CableModem_UsOfdmaChan_GetEntryCount    
    *  X_RDKCENTRAL_COM_CableModem_UsOfdmaChan_GetEntry    
    *  X_RDKCENTRAL_COM_CableModem_UsOfdmaChan_GetParamBoolValue
    *  X_RDKCENTRAL_COM_CableModem_UsOfdmaChan_GetParamUlongValue    

***********************************************************************/
/**
 * @brief This function is called to retrieve the US channel total count associated with X_RDKCENTRAL_COM_CableModem_UsOfdmaChan datamodel.
 *
 * @param[in] hInsContext  Instance handle.
 *
 * @return  Returns the table count.
 */
ULONG
X_RDKCENTRAL_COM_CableModem_UsOfdmaChan_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief This function is called to retrieve the US channel entry specified by the index associated with X_RDKCENTRAL_COM_CableModem_UsOfdmaChan datamodel.
 *
 * @param[in] hInsContext      Instance handle.
 * @param[in] nIndex           Index number to get US channel information.
 * @param[in] pInsNumber       Output instance number.
 *
 * @return  Returns handle of US channel info on success case, Otherwise returns NULL.
 */
ANSC_HANDLE
X_RDKCENTRAL_COM_CableModem_UsOfdmaChan_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief This function is used to check whether the US channel info table is updated or not. Function associated
 * with X_RDKCENTRAL_COM_CableModem_UsOfdmaChan datamodel.
 *
 * @param[in] hInsContext    Instance handle.
 *
 * @return  Returns TRUE once updated.
 */
BOOL
X_RDKCENTRAL_COM_CableModem_UsOfdmaChan_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief This function is called to synchronize the table by getting US OFDM channel info by HAL API's.
 * Function associated with X_RDKCENTRAL_COM_CableModem_UsOfdmaChan datamodel.
 *
 * @param[in] hInsContext  Instance handle.
 *
 * @return  Returns 0 once synchronized.
 */
ULONG
X_RDKCENTRAL_COM_CableModem_UsOfdmaChan_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief This function checks for the parameter field and retrieves the parameter's boolean value
 * associated with X_RDKCENTRAL_COM_CableModem_UsOfdmaChan datamodel.
 *
 * @param[in] hInsContext     Instance handle.
 * @param[in] ParamName       Parameter field.
 * @param[in] pBool           Parameter's boolean Value.
 *
 * @return  Returns TRUE once get the value, returns FALSE when receive unsupported parameter.
 */
BOOL
X_RDKCENTRAL_COM_CableModem_UsOfdmaChan_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief This function checks for the parameter field and retrieves the parameter's value
 * associated with X_RDKCENTRAL_COM_CableModem_UsOfdmaChan datamodel.
 *
 * @param[in] hInsContext     Instance handle.
 * @param[in] ParamName       Parameter field.
 * @param[in] pUlong          Parameter's Value.
 *
 * @return  Returns TRUE once get the value, returns FALSE when receive unsupported parameter.
 */
BOOL
X_RDKCENTRAL_COM_CableModem_UsOfdmaChan_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/***********************************************************************

 APIs for Object:

    X_RDKCENTRAL-COM_CableModem.StatusOfdma

    *  X_RDKCENTRAL_COM_CableModem_StatusOfdma_IsUpdated
    *  X_RDKCENTRAL_COM_CableModem_StatusOfdma_Synchronize        
    *  X_RDKCENTRAL_COM_CableModem_StatusOfdma_GetEntryCount    
    *  X_RDKCENTRAL_COM_CableModem_StatusOfdma_GetEntry    
    *  X_RDKCENTRAL_COM_CableModem_StatusOfdma_GetParamBoolValue
    *  X_RDKCENTRAL_COM_CableModem_StatusOfdma_GetParamUlongValue    

***********************************************************************/
/**
 * @brief This function is called to retrieve the CM status of US channel total count
 * associated with X_RDKCENTRAL-COM_CableModem.StatusOfdma datamodel.
 *
 * @param[in] hInsContext  Instance handle.
 *
 * @return  Returns the total channel count.
 */
ULONG
X_RDKCENTRAL_COM_CableModem_StatusOfdma_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief This function is called to retrieve the CM status of US channel entry specified by the index.
 * Function is associated with X_RDKCENTRAL-COM_CableModem.StatusOfdma datamodel.
 *
 * @param[in] hInsContext      Instance handle.
 * @param[in] nIndex           Index number to get CM status of US channel information.
 * @param[in] pInsNumber       Output instance number.
 *
 * @return  Returns handle of the CM status of US channel info on success case, Otherwise returns NULL.
 */
ANSC_HANDLE
X_RDKCENTRAL_COM_CableModem_StatusOfdma_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

/**
 * @brief This function is used to check whether the CM status of US channel info table is updated or not.
 * Function is associated with X_RDKCENTRAL-COM_CableModem.StatusOfdma datamodel.
 *
 * @param[in] hInsContext    Instance handle.
 *
 * @return  Returns TRUE once updated.
 */
BOOL
X_RDKCENTRAL_COM_CableModem_StatusOfdma_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief This function is called to synchronize the table by getting status of US OFDM channel info by HAL API's.
 * Function is associated with X_RDKCENTRAL-COM_CableModem.StatusOfdma datamodel.
 *
 * @param[in] hInsContext  Instance handle.
 *
 * @return  Returns 0 once synchronized.
 */
ULONG
X_RDKCENTRAL_COM_CableModem_StatusOfdma_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

/**
 * @brief This function checks for the parameter field of CM status and retrieves the parameter's boolean value
 * associated with X_RDKCENTRAL-COM_CableModem.StatusOfdma datamodel.
 *
 * @param[in] hInsContext     Instance handle.
 * @param[in] ParamName       Parameter field.
 * @param[in] pBool           Parameter's boolean Value.
 *
 * @return  Returns TRUE once get the value, returns FALSE when receive unsupported parameter.
 */
BOOL
X_RDKCENTRAL_COM_CableModem_StatusOfdma_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

/**
 * @brief This function checks for the parameter field of CM status and retrieves the parameter's value
 * associated with X_RDKCENTRAL-COM_CableModem.StatusOfdma datamodel.
 *
 * @param[in] hInsContext     Instance handle.
 * @param[in] ParamName       Parameter field.
 * @param[in] pUlong          Parameter's Value.
 *
 * @return  Returns TRUE once get the value, returns FALSE when receive unsupported parameter.
 */
BOOL
X_RDKCENTRAL_COM_CableModem_StatusOfdma_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

/** @} */  //END OF GROUP CM_AGENT_APIS
#endif /* _COSA_RDKCENTRAL_CM_DML_H */
