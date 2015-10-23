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

LDFLAGS += -lpthread

HAL_LDFLAGS = -L$(CCSP_INSTALL_ROOT)/lib -lhal_cm
LDFLAGS += $(HAL_LDFLAGS)

DHCP_LDFLAGS = -L$(SDK_PATH)/ti/netdk/src/uipc
DHCP_LDFLAGS += -L$(SDK_PATH)/ti/netdk/src/ti_udhcp
DHCP_LDFLAGS += -L$(SDK_PATH)/ti/netdk/src/ti_dhcpv6
DHCP_LDFLAGS += -luipc -lapi_dhcpv4c -ldhcp4cApi
LDFLAGS += $(DHCP_LDFLAGS)

#RDKB MOD for SDK 4.3. Removed obsolete Intel Libraries and replaed with new ones
CM_LDFLAGS = -L$(SDK_PATH)/ti/lib 
CM_LDFLAGS += -lall_docsis
CM_LDFLAGS += -lti_sme -lsme
CM_LDFLAGS += -lticc -lm
#RDKB MOD END

LDFLAGS += $(CM_LDFLAGS) -lcrypto

# UTOPIA_LDFLAGS = -L$(SDK_PATH)/ti/lib -lutapi -lutctx -lsyscfg -lsysevent -lulog
# LDFLAGS += $(UTOPIA_LDFLAGS)
