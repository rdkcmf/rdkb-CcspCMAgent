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

    module: cosa_device_info_apis.h

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    copyright:

        Cisco Systems, Inc.
        All Rights Reserved.

    -------------------------------------------------------------------

    description:

        This file defines the apis for objects to support Data Model Library.

    -------------------------------------------------------------------


    author:

        COSA XML TOOL CODE GENERATOR 1.0

    -------------------------------------------------------------------

    revision:

        01/11/2011    initial revision.

**************************************************************************/


#ifndef  _COSA_DI_APIS_H
#define  _COSA_DI_APIS_H

#include "cosa_apis.h"
#include "plugin_main_apis.h"

/**********************************************************************
                STRUCTURE AND CONSTANT DEFINITIONS
**********************************************************************/


/**********************************************************************
                FUNCTION PROTOTYPES
**********************************************************************/

ANSC_STATUS CosaDmlDIInit(ANSC_HANDLE hDml, PANSC_HANDLE phContext);
ANSC_STATUS CosaDmlDIGetDLFlag(ANSC_HANDLE hContext);
ANSC_STATUS CosaDmlDIGetFWVersion(ANSC_HANDLE hContext);
ANSC_STATUS CosaDmlDIGetDLStatus(ANSC_HANDLE hContext, char *DL_Status);
ANSC_STATUS CosaDmlDIGetProtocol(ANSC_HANDLE hContext, char *Protocol);
ANSC_STATUS CosaDmlDIDownloadNow(ANSC_HANDLE hContext);
ANSC_STATUS CosaDmlDISetURL(ANSC_HANDLE hContext, char *URL);
ANSC_STATUS CosaDmlDISetImage(ANSC_HANDLE hContext, char *Image);
void FWDL_ThreadFunc();
convert_to_validFW(char *fw,char *valid_fw);
#endif
