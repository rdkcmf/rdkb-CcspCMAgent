##########################################################################
# If not stated otherwise in this file or this component's LICENSE
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
AM_CFLAGS =

ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = CcspCMAgentSsp

CcspCMAgentSsp_CPPFLAGS = \
    -I$(top_srcdir)/source/TR-181/board_sbapi \
    -I$(top_srcdir)/source/TR-181/middle_layer_src \
    -I$(top_srcdir)/source/TR-181/include \
    -I$(top_srcdir)/source/Custom \
    -I$(top_srcdir)

CcspCMAgentSsp_CFLAGS = $(SYSTEMD_CFLAGS)
CcspCMAgentSsp_CFLAGS = $(WANMGR_CFLAGS)
CcspCMAgentSsp_SOURCES = gw_prov_sm.c ssp_main.c ssp_messagebus_interface.c ssp_action.c dm_pack_datamodel.c
CcspCMAgentSsp_LDFLAGS = -lccsp_common -lsyscfg -lcm_mgnt -lsecure_wrapper $(SYSTEMD_LDFLAGS) $(WANMGR_LDFLAGS)

if FEATURE_WAN_FAIL_OVER
CcspCMAgentSsp_DEPENDENCIES = ${top_builddir}/source/TR-181/middle_layer_src/libCcspCMAgent_middle_layer_src.la
CcspCMAgentSsp_LDADD = $(CcspCMAgentSsp_DEPENDENCIES)
endif

if !PLATFORM_PUMA7_ENABLED
if FEATURE_RDKB_WAN_MANAGER
CcspCMAgentSsp_LDFLAGS += -lgwprovappabs
endif
endif

if PLATFORM_PUMA7_ENABLED
CcspCMAgentSsp_LDFLAGS += -lgwapiproxy -lgwgimproxy -ltlv
endif
