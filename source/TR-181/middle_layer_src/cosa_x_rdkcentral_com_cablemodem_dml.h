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

/***********************************************************************

 APIs for Object:

    X_RDKCENTRAL-COM_CableModem.DsOfdmChan

    *  X_RDKCENTRAL_COM_CableModem_DsOfdmChan_IsUpdated
    *  X_RDKCENTRAL_COM_CableModem_DsOfdmChan_Synchronize    
    *  X_RDKCENTRAL_COM_CableModem_DsOfdmChan_GetEntryCount    
    *  X_RDKCENTRAL_COM_CableModem_DsOfdmChan_GetEntry        
    *  X_RDKCENTRAL_COM_CableModem_DsOfdmChan_GetParamUlongValue

***********************************************************************/
ULONG
X_RDKCENTRAL_COM_CableModem_DsOfdmChan_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    );

ANSC_HANDLE
X_RDKCENTRAL_COM_CableModem_DsOfdmChan_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

BOOL
X_RDKCENTRAL_COM_CableModem_DsOfdmChan_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
X_RDKCENTRAL_COM_CableModem_DsOfdmChan_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

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
ULONG
X_RDKCENTRAL_COM_CableModem_UsOfdmaChan_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    );

ANSC_HANDLE
X_RDKCENTRAL_COM_CableModem_UsOfdmaChan_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

BOOL
X_RDKCENTRAL_COM_CableModem_UsOfdmaChan_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
X_RDKCENTRAL_COM_CableModem_UsOfdmaChan_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

BOOL
X_RDKCENTRAL_COM_CableModem_UsOfdmaChan_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

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
ULONG
X_RDKCENTRAL_COM_CableModem_StatusOfdma_GetEntryCount
    (
        ANSC_HANDLE                 hInsContext
    );

ANSC_HANDLE
X_RDKCENTRAL_COM_CableModem_StatusOfdma_GetEntry
    (
        ANSC_HANDLE                 hInsContext,
        ULONG                       nIndex,
        ULONG*                      pInsNumber
    );

BOOL
X_RDKCENTRAL_COM_CableModem_StatusOfdma_IsUpdated
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
X_RDKCENTRAL_COM_CableModem_StatusOfdma_Synchronize
    (
        ANSC_HANDLE                 hInsContext
    );

BOOL
X_RDKCENTRAL_COM_CableModem_StatusOfdma_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    );

BOOL
X_RDKCENTRAL_COM_CableModem_StatusOfdma_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      pUlong
    );

#endif /* _COSA_RDKCENTRAL_CM_DML_H */
