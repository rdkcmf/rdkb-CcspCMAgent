AM_CFLAGS = -D_ANSC_LINUX
AM_CFLAGS += -D_ANSC_USER
AM_CFLAGS += -D_ANSC_LITTLE_ENDIAN_
AM_LDFLAGS = -lccsp_common
AM_LDFLAGS += -lhal_cm

AM_CPPFLAGS = -Wall -Werror
ACLOCAL_AMFLAGS = -I m4
hardware_platform = i686-linux-gnu
bin_PROGRAMS = CcspCMAgentSsp
CcspCMAgentSsp_CPPFLAGS = -I$(top_srcdir)/source/TR-181/board_sbapi -I$(top_srcdir)/source/TR-181/middle_layer_src -I$(top_srcdir)/source/TR-181/include -I$(top_srcdir)/source/Custom $(CPPFLAGS)
CcspCMAgentSsp_SOURCES = ssp_messagebus_interface.c ssp_main.c ssp_action.c
CcspCMAgentSsp_LDFLAGS = -lccsp_common -lhal_cm
