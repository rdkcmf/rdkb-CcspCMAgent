/*
 * If not stated otherwise in this file or this component's Licenses.txt file
 * the following copyright and licenses apply:
 *
 * Copyright 2022 RDK Management
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

#ifndef  RBUS_HANDLER_APIS_H
#define  RBUS_HANDLER_APIS_H

#if defined (WAN_FAILOVER_SUPPORTED)
#include <stdbool.h>
#include <rbus.h>

//#define NUM_OF_RBUS_PARAMS	3
#define RBUS_COMPONENT_NAME	"RbusCMAgent"
#define DOCSISLINKDOWNTIMEOUT "DocsisLinkDownTimeOut"
#define DOCSIS_LINK_STATUS_TR181	"Device.X_RDK_DOCSIS.LinkStatus"
#define DOCSIS_LINKDOWN_TR181   "Device.X_RDK_DOCSIS.LinkDown"
#define DOCSIS_LINKDOWNTIMEOUT_TR181 "Device.X_RDK_DOCSIS.LinkDownTimeout"

typedef void (*fpDocsisLinkdownSignal)();
typedef struct 
_CmAgent_Link_Status_
{
    bool DocsisLinkStatus;
    bool DocsisLinkDown;
    uint DocsisLinkDownTimeOut;
    fpDocsisLinkdownSignal pDocsisLinkdowSignal;
} CmAgent_Link_Status;

rbusError_t getBoolHandler(rbusHandle_t handle, rbusProperty_t property, rbusGetHandlerOptions_t* opts);
rbusError_t getuintHandler(rbusHandle_t handle, rbusProperty_t property, rbusGetHandlerOptions_t *opts);
rbusError_t SetBoolHandler(rbusHandle_t handle, rbusProperty_t property, rbusSetHandlerOptions_t* opts);
rbusError_t SetUintHandler(rbusHandle_t handle, rbusProperty_t property, rbusSetHandlerOptions_t* opts);

rbusError_t eventSubHandler(rbusHandle_t handle, rbusEventSubAction_t action, const char* eventName, rbusFilter_t filter, int32_t interval, bool* autoPublish);

rbusError_t cmAgentRbusInit();

void initLinkStatus();

rbusError_t sendBoolUpdateEvent(char* event_name , bool eventNewData, bool eventOldData);

void publishDocsisLinkStatus(bool link_status);

char const* GetParamName(char const* path);
BOOL SetDocsisLinkdowSignalfunc(fpDocsisLinkdownSignal CreateThreadandSendCondSignalToPthreadfunc);
#endif
#endif
