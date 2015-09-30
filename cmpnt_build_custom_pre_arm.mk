##########################################################################
# If not stated otherwise in this file or this component's Licenses.txt
# file the following copyright and licenses apply:
#
# Copyright 2015 RDK Management
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
##########################################################################
#######################################################################
#   Copyright [2014] [Cisco Systems, Inc.]
# 
#   Licensed under the Apache License, Version 2.0 (the \"License\");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
# 
#       http://www.apache.org/licenses/LICENSE-2.0
# 
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an \"AS IS\" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
#######################################################################

#
#   for Intel USG, Pumba 6, all ARM boards
#   CM custom makefile
#

#
#   platform specific customization
#
HAL_LDFLAGS = -L$(CCSP_INSTALL_ROOT)/lib -lcm_mgnt
LDFLAGS += $(HAL_LDFLAGS)


# SDK_VERSION comes from intel_usg
ifeq ($(SDK_VERSION),)
SDK_VERSION = 4.2
$(info *** SDK_VERSION not defined, set to 4.2 ***)
endif

sdk_version := v$(SDK_VERSION)
ifneq (,$(findstring v4.2,$(sdk_version)))
# v4.2
$(info *** SDK_VERSION is 4.2 ***)
	DHCP_LDFLAGS = -L$(SDK_PATH)/ti/netdk/src/uipc
	DHCP_LDFLAGS += -luipc -lapi_dhcpv4c

	CM_LDFLAGS = -L$(SDK_PATH)/ti/lib 
	CM_LDFLAGS += -lhal_db -lhal_global -lcmd_mbox
	CM_LDFLAGS += -lhal_phy -lm -lmask_lib -lhal_reg_access
	CM_LDFLAGS += -lus_db 
	CM_LDFLAGS += -lhal_ds_calibration -lhal_tuner_api -lhal_ffs_calibration
	CM_LDFLAGS += -lhal_us_calibration
	CM_LDFLAGS += -lreinit_mac 
	CM_LDFLAGS += -ldschannellistfreqdb -lprimary_ds_freq_override_db -lgetnextfreq
	CM_LDFLAGS += -ldocsis_ext_interface -ldbridge_ctl -lcmstatus -ldmg_utils
	CM_LDFLAGS += -lnvramstorage -lutils_docsis -lqos_internal_db -lfccfg -lti_sme -lsme
	CM_LDFLAGS += -lhal_tuner_pwrSpec -lchdb_rx
else
# v4.3 and above
$(info *** SDK_VERSION is 4.3 ***)
	#RDKB MOD for SDK 4.3. Removed obsolete Intel Libraries and replaed with new ones
	DHCP_LDFLAGS = -L$(SDK_PATH)/ti/netdk/src/uipc
	DHCP_LDFLAGS += -L$(SDK_PATH)/ti/netdk/src/ti_udhcp
	DHCP_LDFLAGS += -L$(SDK_PATH)/ti/netdk/src/ti_dhcpv6
	DHCP_LDFLAGS += -luipc -lapi_dhcpv4c -ldhcp4cApi

	CM_LDFLAGS = -L$(SDK_PATH)/ti/lib 
	CM_LDFLAGS += -lall_docsis
	CM_LDFLAGS += -lti_sme -lsme
	CM_LDFLAGS += -lticc -lm
	#RDKB MOD END
endif

LDFLAGS += $(DHCP_LDFLAGS)
LDFLAGS += $(CM_LDFLAGS)
LDFLAGS += -lpthread -lcrypto


# UTOPIA_LDFLAGS = -L$(SDK_PATH)/ti/lib -lutapi -lutctx -lsyscfg -lsysevent -lulog
# LDFLAGS += $(UTOPIA_LDFLAGS)
