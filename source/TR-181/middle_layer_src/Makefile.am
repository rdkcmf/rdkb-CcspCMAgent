AM_CFLAGS = -D_ANSC_LINUX
AM_CFLAGS += -D_ANSC_USER
AM_CFLAGS += -D_ANSC_LITTLE_ENDIAN_
AM_LDFLAGS = -L$(top_builddir)/../CcspCommonLibrary/.libs
AM_LDFLAGS += -lccsp_common
AM_LDFLAGS += -L$(top_builddir)/../hal/lib
AM_LDFLAGS += -lhal_cm

AM_CPPFLAGS = -Wall -Werror
ACLOCAL_AMFLAGS = -I m4
hardware_platform = i686-linux-gnu

noinst_LTLIBRARIES = libCcspCMAgent_middle_layer_src.la
libCcspCMAgent_middle_layer_src_la_CPPFLAGS = -I/var/tmp/pc-rdkb/include/dbus-1.0 -I$(top_builddir)/../CcspCommonLibrary/source/ccsp/custom -I$(top_builddir)/../CcspCommonLibrary/source/ccsp/include -I$(top_builddir)/../CcspCommonLibrary/source/debug_api/include -I$(top_builddir)/../CcspCommonLibrary/source/cosa/include -I$(top_builddir)/../CcspCommonLibrary/source/cosa/include/linux -I$(top_builddir)/../CcspCommonLibrary/source/ccsp/components/include -I$(top_builddir)/../CcspCommonLibrary/source/cosa/package/slap/include -I$(top_builddir)/../hal/include -I$(top_builddir)/source/TR-181/board_sbapi -I$(top_builddir)/../CcspCommonLibrary/source/util_api/http/include -I$(top_builddir)/../CcspCommonLibrary/source/util_api/ansc/include -I$(top_builddir)/source/TR-181/middle_layer_src -I$(top_builddir)/../CcspCommonLibrary/source/ccsp/components/common/MessageBusHelper/include -I$(top_builddir)/source/TR-181/include -I$(top_builddir)/source/Custom $(CPPFLAGS)
libCcspCMAgent_middle_layer_src_la_SOURCES = plugin_main.c plugin_main_apis.c cosa_x_cisco_com_cablemodem_dml.c cosa_x_cisco_com_cablemodem_internal.c
libCcspCMAgent_middle_layer_src_la_LDFLAGS =  -L$(top_builddir)/../CcspCommonLibrary/.libs -lccsp_common -L$(top_builddir)/../hal/lib -lhal_cm