/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2018 RDK Management
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
#if defined (WAN_FAILOVER_SUPPORTED)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "ccsp_trace.h"
#include "cosa_rbus_handler_apis.h"

CmAgent_Link_Status cmAgent_Link_Status;

rbusHandle_t handle;

unsigned int gSubscribersCount = 0;

/***********************************************************************

  Data Elements declaration:

 ***********************************************************************/
rbusDataElement_t cmAgentRbusDataElements[NUM_OF_RBUS_PARAMS] = {

	{DOCSIS_LINK_STATUS_TR181, RBUS_ELEMENT_TYPE_EVENT, {getBoolHandler, NULL, NULL, NULL, eventSubHandler, NULL}},

};

/***********************************************************************

  Get Handler APIs for objects of type RBUS_BOOL:

 ***********************************************************************/
rbusError_t getBoolHandler(rbusHandle_t handle, rbusProperty_t property, rbusGetHandlerOptions_t *opts)
{
	char const *name = rbusProperty_GetName(property);
	(void)handle;
	(void)opts;
	rbusValue_t value;
	rbusValue_Init(&value);

	if (strcmp(name, DOCSIS_LINK_STATUS_TR181) == 0)
	{
		CcspTraceWarning(("Getting DOCSIS link status value, new value = '%d'\n", cmAgent_Link_Status.DocsisLinkStatus));
		rbusValue_SetBoolean(value, cmAgent_Link_Status.DocsisLinkStatus);
	}
	else
	{
		CcspTraceWarning(("CMAgent rbus get handler invalid input\n"));
		return RBUS_ERROR_INVALID_INPUT;
	}
	rbusProperty_SetValue(property, value);
	rbusValue_Release(value);

	return RBUS_ERROR_SUCCESS;
}
/***********************************************************************

  Event subscribe handler API for objects:

 ***********************************************************************/
rbusError_t eventSubHandler(rbusHandle_t handle, rbusEventSubAction_t action, const char *eventName, rbusFilter_t filter, int32_t interval, bool *autoPublish)
{
	(void)handle;
	(void)filter;
	(void)interval;

	*autoPublish = false;

	if (!strcmp(DOCSIS_LINK_STATUS_TR181, eventName))
	{
		if (action == RBUS_EVENT_ACTION_SUBSCRIBE)
		{
			gSubscribersCount += 1;
		}
		else
		{
			if (gSubscribersCount > 0)
			{
				gSubscribersCount -= 1;
			}
		}
		CcspTraceWarning(("Subscribers count changed, new value=%d\n", gSubscribersCount));
	}
	else
	{
		CcspTraceWarning(("provider: eventSubHandler unexpected eventName %s\n", eventName));
	}
	return RBUS_ERROR_SUCCESS;
}

/***********************************************************************

  cmAgentRbusInit(): Initialize Rbus and data elements

 ***********************************************************************/
rbusError_t cmAgentRbusInit()
{
	int rc = RBUS_ERROR_SUCCESS;
	rc = rbus_open(&handle, RBUS_COMPONENT_NAME);
	if (rc != RBUS_ERROR_SUCCESS)
	{
		CcspTraceWarning(("CMAgent rbus initialization failed\n"));
		rc = RBUS_ERROR_NOT_INITIALIZED;
		return rc;
	}

	// Register data elements
	rc = rbus_regDataElements(handle, NUM_OF_RBUS_PARAMS, cmAgentRbusDataElements);

	if (rc != RBUS_ERROR_SUCCESS)
	{
		CcspTraceWarning(("rbus register data elements failed\n"));
		rc = rbus_close(handle);
		return rc;
	}
	
	initLinkStatus();

	return rc;
}

/*******************************************************************************

  initLinkStatus(): Initialize CmAgent_Link_Status struct with default values

 ********************************************************************************/
void initLinkStatus()
{
	cmAgent_Link_Status.DocsisLinkStatus = false;
	CcspTraceWarning(("Initialized DOCSIS link status with default values.\n"));
}


/*******************************************************************************

  sendUpdateEvent(): publish event after event value gets updated

 ********************************************************************************/
rbusError_t sendBoolUpdateEvent(char* event_name , bool eventNewData, bool eventOldData)
{
	rbusEvent_t event;
	rbusObject_t data;
	rbusValue_t value;
	rbusValue_t oldVal;
	rbusValue_t byVal;
	rbusError_t ret = RBUS_ERROR_SUCCESS;
	
	//initialize and set previous value for the event
	rbusValue_Init(&oldVal);
	rbusValue_SetBoolean(oldVal, eventOldData);
	//initialize and set new value for the event
	rbusValue_Init(&value);
	rbusValue_SetBoolean(value, eventNewData);
	//initialize and set responsible component name for value change
	rbusValue_Init(&byVal);
	rbusValue_SetString(byVal, RBUS_COMPONENT_NAME);
	//initialize and set rbusObject with desired values
	rbusObject_Init(&data, NULL);
	rbusObject_SetValue(data, "value", value);
	rbusObject_SetValue(data, "oldValue", oldVal);
	rbusObject_SetValue(data, "by", byVal);
	//set data to be transferred
	event.name = event_name;
	event.data = data;
	event.type = RBUS_EVENT_VALUE_CHANGED;
	//publish the event
	ret = rbusEvent_Publish(handle, &event);
	if(ret != RBUS_ERROR_SUCCESS) {
			CcspTraceWarning(("rbusEvent_Publish for %s failed: %d\n", event_name, ret));
	}
	//release all initialized rbusValue objects
	rbusValue_Release(value);
	rbusValue_Release(oldVal);
	rbusValue_Release(byVal);
	rbusObject_Release(data);
	return ret;
}

/*******************************************************************************

  publishDocsisLinkStatus(): publish DocsisLinkStatus event after event value gets updated

 ********************************************************************************/
 
void publishDocsisLinkStatus(bool link_status)
{	
	rbusError_t ret = RBUS_ERROR_SUCCESS;
	bool oldDocsisLinkStatus = cmAgent_Link_Status.DocsisLinkStatus;
	//update DocsisLinkStatus with new value
	cmAgent_Link_Status.DocsisLinkStatus = link_status;
	CcspTraceInfo(("Publishing DOCSIS link status with updated value=%s\n", cmAgent_Link_Status.DocsisLinkStatus ? "true" : "false"));
	if (gSubscribersCount > 0)
	{
		ret = sendBoolUpdateEvent(DOCSIS_LINK_STATUS_TR181, cmAgent_Link_Status.DocsisLinkStatus, oldDocsisLinkStatus);
		if(ret == RBUS_ERROR_SUCCESS) {
			CcspTraceInfo(("Published DOCSIS link status with updated value.\n"));
		}
	}
}
#endif