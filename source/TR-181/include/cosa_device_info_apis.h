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

    module: cosa_device_info_apis.h

        For COSA Data Model Library Development

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
/**
 * @addtogroup CM_AGENT_APIS
 * @{
 */

/**
 * @brief This function validates firmware download control details and provide the status.
 *
 * @param[in] hDml       Data Model Layer handle.
 * @param[in] phContext  Data Model Device info.
 *
 * @return  Returns ANSC_STATUS_SUCCESS when firmware download control details enabled.
 */
ANSC_STATUS CosaDmlDIInit(ANSC_HANDLE hDml, PANSC_HANDLE phContext);

/**
 * @brief This function checks for the device's build info in-order to update the download control flag details.
 *
 * @param[in] hContext  Data Model Device info.
 *
 * @return  Returns ANSC_STATUS_SUCCESS once download control flag info successfully updated.
 * 			Returns ANSC_STATUS_FAILURE on failure case.
 */
ANSC_STATUS CosaDmlDIGetDLFlag(ANSC_HANDLE hContext);

/**
 * @brief This function is used to get Firmware version from device info details.
 *
 * @param[in] hContext  Data Model Device info.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on successfully getting firmware version.
 * 			Returns ANSC_STATUS_FAILURE on failure case.
 */
ANSC_STATUS CosaDmlDIGetFWVersion(ANSC_HANDLE hContext);

/**
 * @brief This function is used to get download status. This retrives status from hal layer.
 *
 * @param[in] hContext     Data Model Device info.
 * @param[out] DL_Status   Download status buffer to fill.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on successfully getting download status.
 */
ANSC_STATUS CosaDmlDIGetDLStatus(ANSC_HANDLE hContext, char *DL_Status);

/**
 * @brief This function is used to get the protocol(http/https) from download url.
 *
 * @param[in] hContext     Data Model Device info.
 * @param[out] Protocol    Download protocol buffer to fill.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on successfully getting download protocol.
 */
ANSC_STATUS CosaDmlDIGetProtocol(ANSC_HANDLE hContext, char *Protocol);

#if defined (_COSA_INTEL_XB3_ARM_) || defined (_XB6_PROD_REQ_) || defined (_XB6_PRODUCT_REQ_)
/**
 * @brief This function checks for energy detection and retrieves the signal status.
 *
 * @param[out] pRfSignalStatus     Buffer to retrieve signal status.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on successfully getting signal status, Otherwise return ANSC_STATUS_FAILURE.
 */
ANSC_STATUS CosaDmlDIGetRfSignalStatus(BOOLEAN *pRfSignalStatus);
#endif

/**
 * @brief This function validates all the possible required cases and process for downloading latest firmware.
 *
 * @param[in] hContext     Data Model Device info.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on successfully created firmware download thread to start the process.
 *          Returns ANSC_STATUS_FAILURE on failure care.
 */
ANSC_STATUS CosaDmlDIDownloadNow(ANSC_HANDLE hContext);

/**
 * @brief This function used to set download URL to trigger firmware download.
 *
 * @param[in] hContext     Data Model Device info.
 * @param[in] URL          Download URL to set in DM device info structure.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on successfully assign the DL URL.
 */
ANSC_STATUS CosaDmlDISetURL(ANSC_HANDLE hContext, char *URL);

/**
 * @brief This function used to set download Image.
 *
 * @param[in] hContext     Data Model Device info.
 * @param[in] Image        Download Image to set in DM device info structure.
 *
 * @return  Returns ANSC_STATUS_SUCCESS on successfully assign the DL image.
 */
ANSC_STATUS CosaDmlDISetImage(ANSC_HANDLE hContext, char *Image);

/**
 * @brief Firmware Download Thread function.
 */
void *FWDL_ThreadFunc(void *args);

/**
 * @brief This function used to convert download firmware info into defined firmware format.
 * Valid FW names (TG1682_DEV_stable2_20170717081507sdy, TG1682_DEV_stable2_20170717081507sdy.bin, TG1682_DEV_stable2_20170717081507sdy_signed.bin, tg1682_dev_stable2_20170717081507sdy_signed.bin.ccs)
 *
 * @param[in]  fw           Firmware to download.
 * @param[out] valid_fw     Valid firmware.
 */
void convert_to_validFW(char *fw,char *valid_fw);

BOOL IsFileUpdateNeeded(ANSC_HANDLE hContext );
void WriteFactoryResetParams(ANSC_HANDLE hContext );


/** @} */  //END OF GROUP CM_AGENT_APIS
#endif
