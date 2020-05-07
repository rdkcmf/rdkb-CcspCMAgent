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

    module: cosa_device_info_api.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implementes back-end apis for the COSA Data Model Library

        *  CosaDmlDIInit
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

//!!!  This code assumes that all data structures are the SAME in middle-layer APIs and HAL layer APIs
//!!!  So it uses casting from one to the other
#include "cosa_device_info_apis.h"
#include "cm_hal.h"
#include "cosa_device_info_internal.h"
#include "safec_lib_common.h"


#define CM_HTTPURL_LEN 512
#define VALID_fW_LEN 128
#define MAX_DL_STATUS 128 
#define MAX_PROTOCOL  16
ANSC_STATUS
CosaDmlDIInit
    (
        ANSC_HANDLE                 hDml,
        PANSC_HANDLE                phContext
    )
{
    PCOSA_DATAMODEL_DEVICEINFO      pMyObject    = (PCOSA_DATAMODEL_DEVICEINFO)phContext;

    CosaDmlDIGetDLFlag((ANSC_HANDLE)pMyObject);
    
    if(pMyObject->Download_Control_Flag)
    	CosaDmlDIGetFWVersion((ANSC_HANDLE)pMyObject);
    
    return ANSC_STATUS_SUCCESS;
}
ANSC_STATUS CosaDmlDIGetDLFlag(ANSC_HANDLE hContext)
{
	PCOSA_DATAMODEL_DEVICEINFO      pMyObject    = (PCOSA_DATAMODEL_DEVICEINFO)hContext;
	FILE *fp;
	char buff[64]={0};

	pMyObject->Download_Control_Flag = FALSE;
	
	if((fp = fopen("/etc/device.properties", "r")) == NULL)
	{
		CcspTraceError(("Error while opening the file device.properties \n"));
		CcspTraceInfo((" Download_Control_Flag is %d \n", pMyObject->Download_Control_Flag));
		return ANSC_STATUS_FAILURE;
	}

	while(fgets(buff, 64, fp) != NULL) 
	{
		if(strstr(buff, "BUILD_TYPE") != NULL) 
		{
		//RDKB-22703: Enabled support to allow TR-181 firmware download on PROD builds.
			if((strcasestr(buff, "dev") != NULL) || (strcasestr(buff, "vbn") != NULL) || (strcasestr(buff, "prod") != NULL))
			{
				pMyObject->Download_Control_Flag = TRUE;
				break;
			}
		}
	}

	if(fp)
		fclose(fp);

	CcspTraceInfo((" Download_Control_Flag is %d \n", pMyObject->Download_Control_Flag));
	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS CosaDmlDIGetFWVersion(ANSC_HANDLE hContext)
{

	PCOSA_DATAMODEL_DEVICEINFO      pMyObject    = (PCOSA_DATAMODEL_DEVICEINFO)hContext;
	FILE *fp;
	char buff[128]={0};
	
	if((fp = fopen("/version.txt", "r")) == NULL)
	{
		CcspTraceError(("Error while opening the file version.txt \n"));
		return ANSC_STATUS_FAILURE;
	}

	while(fgets(buff, 128, fp) != NULL) 
	{
		if(strstr(buff, "imagename") != NULL) 
		{
			int i = 0;
			while((buff[i+10] != '\n') && (buff[i+10] != '\r') && (buff[i+10] != '\0'))
			{
				pMyObject->Current_Firmware[i] = buff[i+10];
				i++;
			}
			pMyObject->Current_Firmware[i] = '\0';
			break;
		}
	}
	
	if(fp)
		fclose(fp);
	
	CcspTraceInfo((" Current FW Version is %s \n", pMyObject->Current_Firmware));
	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS CosaDmlDIGetDLStatus(ANSC_HANDLE hContext, char *DL_Status)
{
	int dl_status = 0;
        errno_t        rc = -1;
	
	dl_status = cm_hal_Get_HTTP_Download_Status();
	CcspTraceInfo((" Download status is %d \n", dl_status));
	
	if(dl_status == 0)
        {
             rc = strcpy_s(DL_Status,MAX_DL_STATUS, "Not Started");
             if(rc != EOK)
             {
                 ERR_CHK(rc);
                 return ANSC_STATUS_FAILURE;
             }
        }
	else if(dl_status > 0 && dl_status <= 100)
        {
              rc = strcpy_s(DL_Status,MAX_DL_STATUS, "In Progress");
              if(rc != EOK)
               {
                   ERR_CHK(rc);
                   return ANSC_STATUS_FAILURE;
                }
        }
         else if(dl_status == 200)
         {
               rc = strcpy_s(DL_Status,MAX_DL_STATUS, "Completed");
               if(rc != EOK)
               {
                      ERR_CHK(rc);
                      return ANSC_STATUS_FAILURE;
                }
         }
	  else if(dl_status >= 400)
          {
                rc = strcpy_s(DL_Status,MAX_DL_STATUS, "Failed");
                if(rc != EOK)
                 {
                       ERR_CHK(rc);
                       return ANSC_STATUS_FAILURE;
                  }
           }
	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS CosaDmlDIGetProtocol(ANSC_HANDLE hContext, char *Protocol)
{
	PCOSA_DATAMODEL_DEVICEINFO     pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)hContext;
        errno_t rc = -1;
        int ind = -1;

	if(strlen(pMyObject->DownloadURL) == 0)
        {
          rc = strcpy_s(Protocol,MAX_PROTOCOL, "");
          if(rc != EOK)
          {
                ERR_CHK(rc);
                return ANSC_STATUS_FAILURE;
          }
 
        }
        else if(AnscEqualString2(pMyObject->DownloadURL,"https", 5, FALSE))
        {
		rc = strcpy_s(Protocol,MAX_PROTOCOL, "HTTPS");
                if(rc != EOK)
               {
                  ERR_CHK(rc);
                  return ANSC_STATUS_FAILURE;
               }
        }
        else if(AnscEqualString2(pMyObject->DownloadURL,"http", 4, FALSE))
        {
		   rc =  strcpy_s(Protocol,MAX_PROTOCOL, "HTTP");
                   if(rc != EOK)
                   {
                     ERR_CHK(rc);
                     return ANSC_STATUS_FAILURE;
                    }
         }

         else
         {
  
                 rc = strcpy_s(Protocol,MAX_PROTOCOL, "INVALID");
                 if(rc != EOK)
                 {
                         ERR_CHK(rc);
                         return ANSC_STATUS_FAILURE;
                 }
         }
         CcspTraceInfo((" Download Protocol is %s \n", Protocol));
	
	return ANSC_STATUS_SUCCESS;	
}

#if defined (_COSA_INTEL_XB3_ARM_) || defined (_XB6_PRODUCT_REQ_)
ANSC_STATUS CosaDmlDIGetRfSignalStatus(BOOLEAN *pRfSignalStatus) {
	int ret = RETURN_ERR;
	
	ret = docsis_IsEnergyDetected(pRfSignalStatus);

	if( ret == RETURN_ERR )
	{
		CcspTraceError((" Failed to get RfSignalStatus \n"));
		return ANSC_STATUS_FAILURE;
	}
	
	return ANSC_STATUS_SUCCESS;	
}
#endif


ANSC_STATUS CosaDmlDIDownloadNow(ANSC_HANDLE hContext)
{
	PCOSA_DATAMODEL_DEVICEINFO     pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)hContext;
	int dl_status = 0;
	int ret = RETURN_ERR, res = 0;
	char valid_fw[VALID_fW_LEN]={0};
        char pHttpUrl[CM_HTTPURL_LEN] = {'0'};
        errno_t rc = -1;
        int ind = -1;
	
	if(strlen(pMyObject->Firmware_To_Download) && strlen(pMyObject->DownloadURL))
	{
		convert_to_validFW(pMyObject->Firmware_To_Download,valid_fw);
		rc = strcasecmp_s(valid_fw,sizeof(valid_fw), pMyObject->Current_Firmware,&ind);
                ERR_CHK(rc);
                if((!ind) && (rc == EOK))
		{
			CcspTraceError((" Current FW is same, Ignoring request \n"));
			return ANSC_STATUS_FAILURE;		
		}

                rc =  strcpy_s(pHttpUrl,sizeof(pHttpUrl), "'");
                if(rc != EOK)
                {
                  ERR_CHK(rc);
                  return ANSC_STATUS_FAILURE;
                 }
                rc =  strncat_s(pHttpUrl,sizeof(pHttpUrl), pMyObject->DownloadURL, CM_HTTPURL_LEN - 1);
                if(rc != EOK)
                {
                  ERR_CHK(rc);
                  return ANSC_STATUS_FAILURE;
                 }

                rc  =  strcat_s(pHttpUrl,sizeof(pHttpUrl), "/");
                if(rc != EOK)
                {
                  ERR_CHK(rc);
                  return ANSC_STATUS_FAILURE;
                 }

                rc =  strncat_s(pHttpUrl,sizeof(pHttpUrl), pMyObject->Firmware_To_Download, CM_HTTPURL_LEN - 1);
                 if(rc != EOK)
                {
                  ERR_CHK(rc);
                  return ANSC_STATUS_FAILURE;
                 }

               rc =   strcat_s(pHttpUrl,sizeof(pHttpUrl), "'");
               if(rc != EOK)
               {
                  ERR_CHK(rc);
                  return ANSC_STATUS_FAILURE;
               }


		ret = RETURN_ERR;
		ret = cm_hal_Set_HTTP_Download_Url(pHttpUrl , pMyObject->Firmware_To_Download);

		if( ret == RETURN_ERR)
		{
			CcspTraceError((" Failed to set URL, Ignoring request \n"));
			return ANSC_STATUS_FAILURE;
		}
		
	}
	else
	{
		CcspTraceError((" URL or FW Name is missing, Ignoring request \n"));
		return ANSC_STATUS_FAILURE;		
	}

	dl_status = cm_hal_Get_HTTP_Download_Status();
	if(dl_status > 0 && dl_status <= 100)
	{
		CcspTraceError((" Already Downloading In Progress, Ignoring request \n"));
		return ANSC_STATUS_FAILURE;
	}
	else if(dl_status == 200)
	{
		CcspTraceError((" Image is already downloaded, Ignoring request \n"));
		return ANSC_STATUS_FAILURE;	
	}
	
	ret = RETURN_ERR;
	ret = cm_hal_Set_HTTP_Download_Interface(1); // interface=0 for wan0, interface=1 for erouter0

	if( ret == RETURN_ERR)
	{
		CcspTraceError((" Failed to set Interface, Ignoring request \n"));
		return ANSC_STATUS_FAILURE;
	}
/*	ret = RETURN_ERR;
	ret = cm_hal_HTTP_Download ();

	if( ret == RETURN_ERR)
	{
		CcspTraceError((" Failed to start download \n"));
		return ANSC_STATUS_FAILURE;
	}
	else
	{
		CcspTraceInfo(("Image downloading triggered successfully \n")); 
		pthread_t FWDL_Thread;
		res = pthread_create(&FWDL_Thread, NULL, FWDL_ThreadFunc, "FWDL_ThreadFunc"); 
		if(res != 0)
		{
			CcspTraceError(("Create FWDL_Thread error %d\n", res));
		}	

	}   */
	pthread_t FWDL_Thread;
	res = pthread_create(&FWDL_Thread, NULL, FWDL_ThreadFunc, "FWDL_ThreadFunc"); 
	if(res != 0)
	{
		CcspTraceError(("Create FWDL_Thread error %d\n", res));
	}	
	
	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS CosaDmlDISetURL(ANSC_HANDLE hContext, char *URL)
{
	PCOSA_DATAMODEL_DEVICEINFO     pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)hContext;
        errno_t rc = -1;

 	rc =  strcpy_s(pMyObject->DownloadURL,sizeof(pMyObject->DownloadURL), URL);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return ANSC_STATUS_FAILURE;
         }
	CcspTraceInfo((" URL is %s \n", pMyObject->DownloadURL));
	return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS CosaDmlDISetImage(ANSC_HANDLE hContext, char *Image)
{
	PCOSA_DATAMODEL_DEVICEINFO     pMyObject = (PCOSA_DATAMODEL_DEVICEINFO)hContext;
        errno_t rc = -1;

        rc = strcpy_s(pMyObject->Firmware_To_Download,sizeof(pMyObject->Firmware_To_Download), Image);
        if(rc != EOK)
        {
            ERR_CHK(rc);
            return ANSC_STATUS_FAILURE;
         }

	CcspTraceInfo((" FW DL is %s \n", pMyObject->Firmware_To_Download));
	return ANSC_STATUS_SUCCESS;	
}

void FWDL_ThreadFunc()
{
	int dl_status = 0;
	int ret = RETURN_ERR;
	ULONG reboot_ready_status = 0;
	FILE *log_wget = NULL;
	    
	pthread_detach(pthread_self());

	ret = cm_hal_HTTP_Download ();
        if( ret == RETURN_ERR)
        {
                CcspTraceError((" Failed to start download \n"));
          	return;
        }
        else
        {
		CcspTraceInfo(("Image downloading triggered successfully \n"));
		ret = RETURN_ERR;
		/* Sleeping since the status returned is 500 on immediate status query */
		CcspTraceInfo((" Sleeping to prevent 500 error \n")); 
		sleep(10);

		/* Check if the /tmp/wget.log file was created, if not wait an adidtional time */
		log_wget= fopen("/tmp/wget.log", "r");
	
		if (log_wget == NULL) 
		{
			CcspTraceInfo(("/tmp/wget.log doesn't exist. Sleeping an additional 10 seconds \n"));
			sleep(10);
		}
		else 
		{
			fclose(log_wget);
			CcspTraceInfo(("/tmp/wget.log created . Continue ...\n"));
		}

		CcspTraceInfo((" Waiting for FW DL ... \n"));
		while(1)
		{
			dl_status = cm_hal_Get_HTTP_Download_Status();

			if(dl_status >= 0 && dl_status <= 100)
				sleep(2);
			else if(dl_status == 200)
				break;
			else if(dl_status >= 400)
			{
				CcspTraceError((" FW DL is failed with status %d \n", dl_status));
				return;
			}
		}

		CcspTraceInfo((" FW DL is over \n"));

		CcspTraceInfo((" Waiting for reboot ready ... \n"));
		while(1)
		{
			ret = cm_hal_Reboot_Ready(&reboot_ready_status);

			if(ret == RETURN_OK && reboot_ready_status == 1)
				break;
			else
				sleep(5);
		}
		CcspTraceInfo((" Waiting for reboot ready over, setting last reboot reason \n"));

		system("dmcli eRT setv Device.DeviceInfo.X_RDKCENTRAL-COM_LastRebootReason string Forced_Software_upgrade");

		ret = RETURN_ERR;
		ret = cm_hal_HTTP_Download_Reboot_Now();

		if(ret == RETURN_OK)
		{
			CcspTraceInfo((" Rebooting the device now!\n"));
		}
		else
		{
			CcspTraceError((" Reboot Already in progress!\n"));
		}	
	}
}

convert_to_validFW(char *fw,char *valid_fw)
{
	/* Valid FW names
		TG1682_DEV_stable2_20170717081507sdy
		TG1682_DEV_stable2_20170717081507sdy.bin
		TG1682_DEV_stable2_20170717081507sdy_signed.bin
		tg1682_dev_stable2_20170717081507sdy_signed.bin.ccs
	*/
	
	char *buff = NULL;
	int buff_len = 0;
        errno_t rc =-1;

	if(buff = strstr(fw,"_signed"));
	else if(buff = strstr(fw,"-signed"));
	else if(buff = strstr(fw,"."));

	if(buff)
		buff_len = strlen(buff);

       rc = strncpy_s(valid_fw,VALID_fW_LEN,fw,strlen(fw)-buff_len);
       ERR_CHK(rc);

	CcspTraceInfo((" Converted image name = %s \n", valid_fw));
}
