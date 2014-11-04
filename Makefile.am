SUBDIRS = source

lib_LTLIBRARIES=libcm_tr181.la
libcm_tr181_la_CPPFLAGS= -D_ANSC_LINUX -D_ANSC_USER -D_ANSC_LITTLE_ENDIAN_ -I/var/tmp/pc-rdkb/include/dbus-1.0 -I$(top_srcdir)/../CcspCommonLibrary/source/ccsp/custom -I$(top_srcdir)/../CcspCommonLibrary/source/ccsp/include -I$(top_srcdir)/../CcspCommonLibrary/source/debug_api/include -I$(top_srcdir)/../CcspCommonLibrary/source/cosa/include -I$(top_srcdir)/../CcspCommonLibrary/source/cosa/include/linux -I$(top_srcdir)/../CcspCommonLibrary/source/ccsp/components/include -I$(top_srcdir)/../CcspCommonLibrary/source/cosa/package/slap/include -I$(top_srcdir)/../hal/include -I$(top_srcdir)/source/TR-181/board_sbapi -I$(top_srcdir)/../CcspCommonLibrary/source/util_api/http/include -I$(top_srcdir)/../CcspCommonLibrary/source/util_api/ansc/include -I$(top_srcdir)/source/TR-181/middle_layer_src -I$(top_srcdir)/../CcspCommonLibrary/source/ccsp/components/common/MessageBusHelper/include -I$(top_srcdir)/source/TR-181/include -I$(top_srcdir)/source/Custom
libcm_tr181_la_LDFLAGS=
libcm_tr181_la_SOURCES=
libcm_tr181_la_DEPENDENCIES= \
	source/TR-181/middle_layer_src/libCcspCMAgent_middle_layer_src.la \
	source/TR-181/integration_src.shared/libCcspCMAgent_integration_src_shared.la

libcm_tr181_la_LIBADD=$(libcm_tr181_la_DEPENDENCIES)
