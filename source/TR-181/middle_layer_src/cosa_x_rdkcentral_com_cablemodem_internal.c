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

    module: cosa_x_rdkcentral_com_cablemodem_internal.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaRDKCentralComCableModemCreate
        *  CosaRDKCentralComCableModemInitialize
        *  CosaRDKCentralComCableModemRemove
    -------------------------------------------------------------------

    environment:

        platform independent

    -------------------------------------------------------------------

    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.

**************************************************************************/

#include "plugin_main_apis.h"
#include "cosa_x_rdkcentral_com_cablemodem_apis.h"
#include "cosa_x_rdkcentral_com_cablemodem_dml.h"
#include "cosa_x_rdkcentral_com_cablemodem_internal.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        CosaRDKCentralComCableModemCreate
            (
            );

    description:

        This function constructs cosa device info object and return handle.

    argument:  

    return:     newly created device info object.

**********************************************************************/

ANSC_HANDLE
CosaRDKCentralComCableModemCreate
    (
        VOID
    )
{
    PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM   pMyObject    = (PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM)NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM)AnscAllocateMemory(sizeof(COSA_DATAMODEL_RDKCENTRAL_CABLEMODEM));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->Oid               = COSA_DATAMODEL_RDKCENTRAL_CM_OID;
    pMyObject->Create            = CosaRDKCentralComCableModemCreate;
    pMyObject->Remove            = CosaRDKCentralComCableModemRemove;
    pMyObject->Initialize        = CosaRDKCentralComCableModemInitialize;

    pMyObject->Initialize   ((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaRDKCentralComCableModemInitialize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate  cosa device info object and return handle.

    argument:   ANSC_HANDLE                 hThisObject
            This handle is actually the pointer of this object
            itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaRDKCentralComCableModemInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                  			returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM   pMyObject    = (PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM)hThisObject;

    /* Initiation all functions */

    /* Initialize middle layer for Device.X_RDKCENTRAL-COM_CableModem.  */
    CosaDmlRDKCentralCMInit(NULL, (PANSC_HANDLE)pMyObject);

#if defined (_CM_HIGHSPLIT_SUPPORTED_)
    /* Initialize middle layer for Device.X_RDK_CableModem.  */
    CosaDmlRDKCMInit(NULL, (PANSC_HANDLE)pMyObject);
#endif /* _CM_HIGHSPLIT_SUPPORTED_ */

    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        CosaRDKCentralComCableModemRemove
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate  cosa device info object and return handle.

    argument:   ANSC_HANDLE                 hThisObject
            This handle is actually the pointer of this object
            itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
CosaRDKCentralComCableModemRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     		   returnStatus = ANSC_STATUS_SUCCESS;
    PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM      pMyObject    = (PCOSA_DATAMODEL_RDKCENTRAL_CABLEMODEM)hThisObject;

    /* Remove necessary resounce */

    /* Remove self */
    AnscFreeMemory((ANSC_HANDLE)pMyObject);

    return returnStatus;
}


