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

/** @file gw_prov_abstraction.h
 *  @brief This file defines the abstracted apis which are called
 *    by the gateway provisioning application during initialization.
 *    It provides interfaces to be used for handshaking during DOCSIS
 *    initialization.
 *
 *  @author Comcast Cable Inc
 *  @bug No known bugs.
 */
#ifndef _GW_DOCSISABS_H_
#define _GW_DOCSISABS_H_
	
/* -- Includes -- */	
#include <stdio.h>
#include "stdbool.h"

#define GW_TR069_TLV_MAX_URL_LEN            120  
#define GW_TR069_TLV_MAX_USERNAME_LEN       96
#define GW_TR069_TLV_MAX_PASSWORD_LEN       96

/* TR-069 Management Server (TLV 2) sub-TLVs Tree */
#define GW_SUBTLV_TR069_ENABLE_CWMP_EXTIF                 1
#define GW_SUBTLV_TR069_URL_EXTIF                         2
#define GW_SUBTLV_TR069_USERNAME_EXTIF                    3
#define GW_SUBTLV_TR069_PASSWORD_EXTIF                    4
#define GW_SUBTLV_TR069_CONNREQ_USERNAME_EXTIF            5
#define GW_SUBTLV_TR069_CONNREQ_PASSWORD_EXTIF            6
#define GW_SUBTLV_TR069_ACS_OVERRIDE_EXTIF                7
#if defined(_PLATFORM_RASPBERRYPI_) || defined(_COSA_BCM_ARM_)
typedef enum
{
    False = 0,
    True = 1
} Bool;

typedef int                 Int;
typedef unsigned int        Uns;
typedef unsigned int        Uint;
typedef unsigned long       Ulong;
typedef char                Char;
typedef char*               String;
typedef void*               Ptr;
typedef long long           Int64;
typedef int                 Int32;
typedef short               Int16;
typedef signed char         Int8;
typedef unsigned long long  Uint64;
typedef unsigned int        Uint32;
typedef unsigned short      Uint16;
typedef unsigned char       Uint8;

typedef enum Status
{
    OK = 0,
    NOK = -1,
    STATUS_OK = OK,
    STATUS_NOK = NOK,
} STATUS;
#define MAC_ADDR_LEN    6

typedef struct mac_addr
{
    Uint8 hw[ MAC_ADDR_LEN ];
} macaddr_t;
#endif

/*! \var typedef enum DOCSIS_Esafe_Db_extIf_e
    \brief Type of enable.
*\n           Needs to be in sync with the enum defined for DOCSIS ESAFE DB.
*/
typedef enum
{
    DOCESAFE_ENABLE_DISABLE_extIf,
    DOCESAFE_ENABLE_IPv4_extIf,
    DOCESAFE_ENABLE_IPv6_extIf,
    DOCESAFE_ENABLE_IPv4_IPv6_extIf,

    DOCESAFE_ENABLE_NUM_ENABLE_TYPES_extIf
} DOCSIS_Esafe_Db_extIf_e;

/*! \var typedef enum TlvParsingStatus TlvParsingStatusExtIf_e
    \brief A parse TLV return status.
*\n          This enum is defined as per the TLV parser definition.
*/
typedef enum TlvParsingStatus
{
    TLV_OK_extIf = 0,                          
    TLV_ILLEGAL_LEN_extIf = -1,                
    TLV_UNDERFLOW_LEN_IS_MISSING_extIf = -2,   
    TLV_UNDERFLOW_VALUE_IS_MISSING_extIf = -3, 
    TLV_LEN_OVERFLOW_extIf = -4,               
    TLV_MANDATORY_TYPE_IS_MISSING_extIf = -5, 
    TLV_REOCCURRING_MISMATCH_extIf = -6,       
    TLV_END_BUFF_LEN_OVERFLOW_extIf = -7,      
    TLV_CALLBACK_ABORT_extIf = -8              

} TlvParsingStatusExtIf_e; 

/*! \var typedef enum esafeErouterOperModeExtIf_e
    \brief eRouter oper mode
*\n            The internal definitions match the MIB requirements.
*\n            Needs to be in sync with the enum defined for DOCSIS ESAFE DB.
*/
typedef enum
{
    DOCESAFE_EROUTER_OPER_DISABLED_extIf,
    DOCESAFE_EROUTER_OPER_IPV4_extIf,
    DOCESAFE_EROUTER_OPER_IPV6_extIf,
    DOCESAFE_EROUTER_OPER_IPV4_IPV6_extIf,
    DOCESAFE_EROUTER_OPER_NOIPV4_NOIPV6_extIf,

    DOCESAFE_EROUTER_NUM_OPER_MODES_extIf,
} esafeErouterOperModeExtIf_e;

/*! \enum esafeProvisioningStatusProgressExtIf_e
 *  \brief The current state of the eSAFE provisioning process
 *      as defined in ESAFE-MIB.
 *\n    Needs to be in sync with the enum defined for DOCSIS ESAFE DB.
 */
typedef enum {
        ESAFE_PROV_STATE_NOT_INITIATED_extIf = 1,     	
	ESAFE_PROV_STATE_IN_PROGRESS_extIf = 2,		
	ESAFE_PROV_STATE_FINISHED_extIf = 3 		
} esafeProvisioningStatusProgressExtIf_e;

/*! \enum GwTr069PaEnableCwmpTypeExtIf_e
 *  \brief CWMP status which is part of tr069pa configuration 
 *  TLV 202.2 structure
 *      
 */
typedef enum
{
    GW_TR069_ENABLE_CWMP_FALSE_EXTIF = 0,
    GW_TR069_ENABLE_CWMP_TRUE_EXTIF = 1
} GwTr069PaEnableCwmpTypeExtIf_e;

/*! \enum GwTr069PaAcsOverrideTypeExtIf_e
 *  \brief ACS override status which is part of tr069pa configuration 
 *  TLV 202.2 structure
 *      
 */
typedef enum
{
    GW_TR069_ACS_OVERRIDE_DISABLED_EXTIF = 0,
    GW_TR069_ACS_OVERRIDE_ENABLED_EXTIF = 1
} GwTr069PaAcsOverrideTypeExtIf_e;

/*! \enum GwTlv2StructExtIf_t
 *  \brief tr069pa configuration TLV 202.2 structure
 *      
 */
typedef struct GwTlv2StructExtIf
{
    Int tlvIndex;
    GwTr069PaEnableCwmpTypeExtIf_e EnableCWMP;
    Char URL[GW_TR069_TLV_MAX_URL_LEN+1];
    Char Username[GW_TR069_TLV_MAX_USERNAME_LEN+1];
    Char Password[GW_TR069_TLV_MAX_PASSWORD_LEN+1];
    Char ConnectionRequestUsername[GW_TR069_TLV_MAX_USERNAME_LEN+1];
    Char ConnectionRequestPassword[GW_TR069_TLV_MAX_PASSWORD_LEN+1];
    GwTr069PaAcsOverrideTypeExtIf_e ACSOverride;
}GwTlv2StructExtIf_t;

/*! \enum TlvParseCallbackStatusExtIf_e
 *  \brief Call back status of TLV parser
 *      
 */
typedef enum TlvParseCallbackStatusExtIf
{
    TLV_PARSE_CALLBACK_OK_EXTIF = 0,          /* Success - continue with the TLV Parsing */
    TLV_PARSE_CALLBACK_ABORT_EXTIF = -1       /* Abort - quit the TLV Parsing and return abort status */

} TlvParseCallbackStatusExtIf_e;


/* 
* Define callback function pointers which needs to be called
* from provisioning abstraction layer when any provisioning
* event occurs.
*/
typedef void (*fpDocsisLinkDown_1)();
typedef void (*fpDocsisLinkDown_2)();
typedef int (*fpDocsisLinkUp)();
typedef void (*fpDocsisCfgfile)(Char*);
typedef void (*fpDocsisTftpOk)();
typedef void (*fpBefCfgfileEntry)();
typedef void (*fpDocsisInited)();
typedef void (*fpProvEntry)();
typedef void (*fpDocsisEnabled)(Uint8);
#if defined(INTEL_PUMA7)
typedef void (*fpDocsisRATransInterval)(Uint16);
#endif
typedef void (*fpGW_Tr069PaSubTLVParse)(Uint8 type, Uint16 length, const Uint8* value);
#ifdef CISCO_CONFIG_DHCPV6_PREFIX_DELEGATION
typedef void (*fpGW_SetTopologyMode)(Uint8 type, Uint16 length, const Uint8* value);
#endif

/*! \var typedef struct __appCallBack 
*   \brief struct of pointers to the function pointers of callback functions.
*/
#ifdef CISCO_CONFIG_DHCPV6_PREFIX_DELEGATION
typedef struct __appCallBack
{
	fpDocsisLinkDown_1 pGWP_act_DocsisLinkDown_1;
	fpDocsisLinkDown_2 pGWP_act_DocsisLinkDown_2;
	fpDocsisLinkUp pGWP_act_DocsisLinkUp;
	fpDocsisCfgfile pGWP_act_DocsisCfgfile;
	fpDocsisTftpOk pGWP_act_DocsisTftpOk;
	fpBefCfgfileEntry pGWP_act_BefCfgfileEntry;
	fpDocsisInited pGWP_act_DocsisInited;
	fpProvEntry pGWP_act_ProvEntry;
	fpDocsisEnabled pDocsis_gotEnable;
	fpGW_Tr069PaSubTLVParse pGW_Tr069PaSubTLVParse;
	fpGW_SetTopologyMode pGW_SetTopologyMode;
}appCallBack;
#else
typedef struct __appCallBack
{
	fpDocsisLinkDown_1 pGWP_act_DocsisLinkDown_1;
        fpDocsisLinkDown_2 pGWP_act_DocsisLinkDown_2;
	fpDocsisLinkUp pGWP_act_DocsisLinkUp;
	fpDocsisCfgfile pGWP_act_DocsisCfgfile;
	fpDocsisTftpOk pGWP_act_DocsisTftpOk;
	fpBefCfgfileEntry pGWP_act_BefCfgfileEntry;
	fpDocsisInited pGWP_act_DocsisInited;
	fpProvEntry pGWP_act_ProvEntry;
        fpDocsisEnabled pDocsis_gotEnable;
    #if defined(INTEL_PUMA7)
        fpDocsisRATransInterval pDocsis_GetRATransInterval;
    #endif

        fpGW_Tr069PaSubTLVParse pGW_Tr069PaSubTLVParse;
}appCallBack;
#endif
 /*
 * Function Definitions
 */
int sendPseudoBridgeModeMessage(unsigned char enable);

/**   
* @brief Enable/Disable an eSafe device. 
         API should pass the interface name and enable/disable type.
*\n  Prototype : void eSafeDevice_Enable
        (
           DOCSIS_Esafe_Db_extIf_e enableMode
        )
*\n Caller: static STATUS GWP_UpdateEsafeAdminMode(DOCSIS_Esafe_Db_extIf_e enableMode)
*
*
* @param[in] DOCSIS_Esafe_Db_extIf_e enableMode - Enable/Disable + enabled type
* @param[out] none.
* @retval void.
*/
void 
eSafeDevice_Enable(DOCSIS_Esafe_Db_extIf_e enableMode);

/** 
* @brief Initialize an eSafe device database. This API should be able to pass the
*        device name, mac address, enable type of the eSafe device to be initialized.
*\n  Prototype :
        void eSafeDevice_Initialize()        
*\n Caller : static void GWP_DocsisInited(void)
*
*
* @param[in] macAddr - MacAddress of the device.
* @param[out] None.
* @retval void.
*/
void eSafeDevice_Initialize(macaddr_t* macAddr);

/** 
* @brief  Adds / Deletes a physical network interface used by the erouter.
*\n Prototype :
        void eSafeDevice_AddeRouterPhysicalNetworkInterface
	(
		char *devName, 
		bool addDel
	)
*\n Caller : static void GWP_DocsisInited(void)
*
*
* @param[in] char *devName - name of the network device.
* @param[in] bool addDel - True -> Add, False -> Delete
* @retval void.
*/
void eSafeDevice_AddeRouterPhysicalNetworkInterface(char *devName, bool addDel);

/** 
* @brief  Set the erouter operation mode
*\n Prototype :
        void eSafeDevice_SetErouterOperationMode
	(
		esafeErouterOperModeExtIf_e operMode
	)
*\n Caller : static void GWP_EnableERouter(void)
*\n	   static void GWP_DisableERouter(void)
*\n	   static int GWP_ProcessIpv4Down(void)
*\n	   static int GWP_ProcessIpv4Up(void)
*\n	   static int GWP_ProcessIpv6Down(void)
*\n	   static int GWP_ProcessIpv6Up(void)
*\n	   static int GWP_act_DocsisInited_callback()
*	   
*
*
* @param[in] esafeErouterOperModeExtIf_e operMode - Erouter operation mode
* @param[out] None.
* @retval void.
*/
void eSafeDevice_SetErouterOperationMode(esafeErouterOperModeExtIf_e operMode); 

/** 
* @brief  Set the provisioning status progress.
*\n Prototype :
        void eSafeDevice_SetProvisioningStatusProgress
	(
		esafeProvisioningStatusProgressExtIf_e StatusProgress
	)
*\n Caller : static void GWP_DocsisInited(void)
*\n	   static void GWP_EnableERouter(void)
*\n        static void GWP_DisableERouter(void)
*\n 	   static int GWP_ProcessIpv4Up(void)
*\n        static int GWP_ProcessIpv6Up(void)
*\n  	   static int GWP_act_StartActiveUnprovisioned()
*\n 	   static int GWP_act_InactiveBefCfgfile()
*	   
*
*
* @param[in] esafeProvisioningStatusProgressExtIf_e StatusProgress
*            -notInitiated(1) inProgress(2) finished(3)
* @param[out] None.
* @retval void.
*/
void eSafeDevice_SetProvisioningStatusProgress(esafeProvisioningStatusProgressExtIf_e StatusProgress); 

/** 
* @brief Get the operation mode of erouter interface
*\n Prototype :
        void eSafeDevice_GetErouterOperationMode
	(
		esafeErouterOperModeExtIf_e *operMode
	)
*\n Caller : static int GWP_ProcessIpv6Down(void)
*\n	   static int GWP_ProcessIpv6Up(void)
*\n	   static int GWP_ProcessIpv4Down(void)
*\n	   static int GWP_ProcessIpv4Up(void)
*
*
* @param[in] esafeErouterOperModeExtIf_e *operMode - Erouter operation mode
* @param[out] None.
* @retval void.
*/
void eSafeDevice_GetErouterOperationMode(esafeErouterOperModeExtIf_e *operMode);

/** 
* @brief  Set service impact support status for DOCSIS interface
* This indicates the service interruption impact assessment of the corresponding device.
*\n Prototype :
        void eSafeDevice_SetServiceIntImpact()
*\n Caller : static int GWP_act_DocsisInited_callback()
*
*
* @param[in] None.
* @param[out] None.
* @retval void.
*/
void eSafeDevice_SetServiceIntImpact();

/** 
* @brief  Get MAC address of net device name.
*\n Prototype :
        void getNetworkDeviceMacAddress
	(
		macaddr_t* macAddr
	)
*\n Caller : static void GWP_DocsisInited(void)
*
*
* @param[in] None.
* @param[out] macaddr_t* macAddr - Maccaddress of the device
* @retval void.
*/
void getNetworkDeviceMacAddress(macaddr_t* macAddr);

/** 
* @brief  get the multicast group address.
*\n Prototype :
        void getMultiCastGroupAddress
	(
		Uint8 *inetAddr, Uint8 *inetMcgrpAddr
	)
*\n Caller : static void *GWP_sysevent_threadfunc(void *data)
*	   static int GWP_act_DocsisInited_callback()
*
*
* @param[in] Uint8 *inetAddr
* @param[out] Uint8 *inetMcgrpAddr
* @retval void.
*/
void getMultiCastGroupAddress(Uint8 *inetAddr, Uint8 *inetMcgrpAddr);

/** 
* @brief  Set MAC address for the network device.
*\n Prototype :
        void setNetworkDeviceMacAddress
	(
		const Char *devName,macaddr_t* macAddr
	)
*\n Caller : static int GWP_act_DocsisInited_callback()
*
*
* @param[in] const Char *devName - Name of the Device.
* @param[in] macaddr_t* macAddr - Macaddress of the.
* @param[out] None.
* @retval void.
*/
void setNetworkDeviceMacAddress(const Char *devName,macaddr_t* macAddr);

/** 
* @brief  Calculate interface's base solicited node address
*			- Link local address for the interface
*\n Prototype :
        void getInterfaceLinkLocalAddress
	(
		const Char* ifname, Uint8 *linkLocalAddr
	)
*\n Caller : static int GWP_act_DocsisInited_callback()
*
*
* @param[in] const Char* ifname - Network interface name
* @param[out] Uint8 *linkLocalAddr - result link local address
* @retval void.
*/
void getInterfaceLinkLocalAddress(const Char* ifname, Uint8 *linkLocalAddr);

/** 
* @brief  Connect/Disconnect DOCSIS local bridge
* 	  
*\n Prototype :
        void connectLocalBridge
	(
		bool
	)
*\n Caller : static int GWP_act_DocsisInited_callback()
*
*
* @param[in] bool bValue - True -> Connect, False -> Disconnect
* @param[out]
* @retval void.
*/
void connectLocalBridge(bool);

/** 
* @brief  API will send event notification on DOCSIS initialization.
*\n Prototype :
        void notifyDocsisInitializedResponse()
*\n Caller :  static int GWP_act_DocsisInited_callback()
*
*
* @param[in] None.
* @param[out] None.
* @retval void.
*/
void notifyDocsisInitializedResponse();

/** 
* @brief  Register for DOCSIS init event which will be handled by
* 	  the state machine which handles gateway provisioning.
*\n Prototype :
        void registerDocsisInitEvents()
*\n Caller :  static int GWP_act_ProvEntry_callback()
*
*
* @param[in] None.
* @param[out] None.
* @retval void.
*/
void registerDocsisInitEvents();

/** 
* @brief  Start DOCSIS config file parsing. 
* 	  Once DOCSIS config file is TFTPied, this API should be 
*	  called to parse it.
*\n Prototype :
        int startDocsisCfgParsing()
*\n Caller :  static int GWP_act_DocsisTftpOk_callback()
*
*
* @param[in] None.
* @param[out] none.
* @retval int.
*/
int startDocsisCfgParsing();

/** 
* @brief  Init docsis resources required for external docsis interface.
*	  Here all database resources needed for DOCSIS maintenance 
* 	  are initialized.
*\n Prototype :
        void initializeDocsisInterface()
*\n Caller :  static void GWP_DocsisInited(void)
*
*
* @param[in]  None.
* @param[out] None.
* @retval void.
*/
void initializeDocsisInterface();

/** 
* @brief  Register for all DOCSIS events. 
* 	  Here all events which needs to be handled through provisioning
*	  state machine will be registered.
*\n Prototype :
        void registerDocsisEvents()
*\n Caller :  static void GWP_DocsisInited(void)
*
*
* @param[in] None.
* @param[out] None.
* @retval void.
*/
void registerDocsisEvents();

/** 
* @brief  Create state machine for event handling from DOCSIS stack
*\n Prototype :
        void SME_CreateEventHandler
	(
		appCallBack *pAppCallBack
	)
*\n Caller : int main(int argc, char *argv[])
*
*
* @param[in] appCallBack *pAppCallBack.
* @param[out] None.
* @retval void.
*/
void SME_CreateEventHandler(appCallBack *pAppCallBack);

/** 
* @brief  Send a notification to GIM on esafe configuration file.
*\n Prototype :
        void  notificationReply_CfgFileForEsafe()
*\n Caller : static int GWP_act_DocsisCfgfile_callback(Char* cfgFile).
*
*
* @param[in] None.
* @param[out] None.
* @retval void.
*/
void  notificationReply_CfgFileForEsafe();

/** 
* @brief  Get the CM factory mode(disabled(0), enabled(1)).
*\n Prototype :
        void getDocsisDbFactoryMode
	(
		Uint32* factoryMode
	)
*\n Caller : static int GWP_act_DocsisInited_callback()
*
*
* @param[in] None.
* @param[out] Uint32* factoryMode.
* @retval void.
*/
void getDocsisDbFactoryMode(Uint32* factoryMode);

/** 
* @brief  Get WAN interface MAC address.
*\n Prototype :
        void getWanMacAddress
	(
		macaddr_t* macAddr
	)
*\n Caller : static int GWP_act_DocsisInited_callback()
*
*
* @param[in] None.
* @param[out] macaddr_t* macAddr
* @retval void.
*/
void getWanMacAddress(macaddr_t* macAddr);

/** 
* @brief  Start WEB GUI process.
*\n Prototype :
        void startWebUIProcess()
*\n Caller : static void *GWP_sysevent_threadfunc(void *data)
*
*
* @param[in] None.
* @param[out] None.
* @retval void.
*/
void startWebUIProcess();

/** 
* @brief  Send process ready signal to PCD process so that next
*	  can be executed by PCD.
*\n Prototype :
        void sendProcessReadySignal()
*\n Caller : static int GWP_act_ProvEntry_callback()
*
*
* @param[in] None.
* @param[out] None.
* @retval void.
*/
void sendProcessReadySignal();

/** 
* @brief Find process Id of specified process.
*\n Prototype :
        pid_t findProcessId
	(
		Char* processName
	)
*\n Caller : int main(int argc, char *argv[]).
*
*
* @param[in] char *argv[0] - Process name whose process id need to 
*			     be find out.
* @param[out] None.
* @retval pid_t : Process Id.
*/
int findProcessId(Char* processName);

/** 
* @brief Register exception handlers for the process as part of PCD.
*\n Prototype :
       	void registerProcessExceptionHandlers
	(
		Char *name
	)
*\n Caller : int main(int argc, char *argv[]).
*
*
* @param[in] char* argv[0] - Process name which has to register 
*			     handler for exceptions.
* @param[out] None.
* @retval void.
*/
void registerProcessExceptionHandlers(Char *name);

/** 
* @brief Function is called to parse the TLV parameters 
*          available in esafe configuration file.
*\n Prototype :
        int parseTlv
	(
		Uint8 *confFileBuff, 
		Uint32 confFileBuffLen
	)
*\n Caller : GWP_act_DocsisCfgfile_callback(Char* cfgFile).
*
*
* @param[in] Uint8 *confFileBuff : Pointer to the config file.
* @param[in] Uint32 confFileBuffLen : Config file buffer length.
* @param[out] None.
* @retval int
*/
int parseTlv(Uint8 *confFileBuff, Uint32 confFileBuffLen);


/** 
* @brief This API is called to send all TR69 SNMP TLV11 configuration to 
*	 SNMP agent
*\n Prototype :
        static STATUS sendTLV11toSnmpAgent
	(
		void * Snmp_Tlv11Buf, 
		int Snmp_Tlv11BufLen
	)
*\n Caller : static STATUS GW_UpdateTr069Cfg(void).
*
*
* @param[in] void * Snmp_Tlv11Buf : SNMP TLV's data buffer.
* @param[in] int Snmp_Tlv11BufLen : SNMP TLV's data buffer size.
* @param[out] None.
* @retval STATUS
*/
#if 0
static STATUS sendTLV11toSnmpAgent(void * Snmp_Tlv11Buf, int Snmp_Tlv11BufLen);
#endif

#endif

