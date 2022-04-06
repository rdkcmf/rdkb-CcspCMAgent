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
SUBDIRS = source

lib_LTLIBRARIES = libcm_tr181.la

libcm_tr181_la_CPPFLAGS = \
    -I$(top_srcdir)/source/TR-181/board_sbapi \
    -I$(top_srcdir)/source/TR-181/middle_layer_src \
    -I$(top_srcdir)/source/TR-181/include \
    -I$(top_srcdir)/source/Custom

libcm_tr181_la_LDFLAGS = -lsyscfg
libcm_tr181_la_SOURCES =
libcm_tr181_la_DEPENDENCIES = \
	source/TR-181/middle_layer_src/libCcspCMAgent_middle_layer_src.la \
	source/TR-181/integration_src.shared/libCcspCMAgent_integration_src_shared.la

libcm_tr181_la_LIBADD = $(libcm_tr181_la_DEPENDENCIES)
