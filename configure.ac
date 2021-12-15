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
#                                              -*- Autoconf -*-
# Process this file with autoconf to produce configure script.
#

AC_PREREQ([2.65])
AC_INIT([cm_tr181], [1.0], [BUG-REPORT-ADDRESS])
AM_INIT_AUTOMAKE
LT_INIT
GTEST_ENABLE_FLAG = ""

AC_ARG_ENABLE([gtestapp],
             AS_HELP_STRING([--enable-gtestapp],[enable Gtest support (default is no)]),
             [
               case "${enableval}" in
                yes) GTEST_SUPPORT_ENABLED=true
                     GTEST_ENABLE_FLAG="-DGTEST_ENABLE"
                     m4_if(m4_sysval,[0],[AC_CONFIG_FILES([source/test/Makefile])]);;
                no) GTEST_SUPPORT_ENABLED=false AC_MSG_ERROR([Gtest support is disabled]);;
                 *) AC_MSG_ERROR([bad value ${enableval} for --enable-gtestapp ]);;
               esac
             ],
             [echo "Gtestapp is disabled"])
AM_CONDITIONAL([WITH_GTEST_SUPPORT], [test x$GTEST_SUPPORT_ENABLED = xtrue])

FEATURE_WAN_FAIL_OVER=no
AC_ARG_ENABLE([wanfailover],
        AS_HELP_STRING([--enable-wanfailover],[enable wanfailover]),
        [
          case "${enableval}" in
           yes) FEATURE_WAN_FAIL_OVER=yes ;;
           no) AC_MSG_ERROR([wanfailover is disabled]) ;;
          *) AC_MSG_ERROR([bad value ${enableval} for --enable-wanfailover ]) ;;
           esac
           ],
         [echo "wanfailover is disabled."])

AM_CONDITIONAL([FEATURE_WAN_FAIL_OVER], [test $FEATURE_WAN_FAIL_OVER = yes])

AC_PREFIX_DEFAULT(`pwd`)
AC_ENABLE_SHARED
AC_DISABLE_STATIC

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AM_PROG_CC_C_O
AM_PROG_LIBTOOL(libtool)

SYSTEMD_CFLAGS=" "
SYSTEMD_LDFLAGS=" "

WANMGR_CFLAGS=" "
WANMGR_LDFLAGS=" "

AC_ARG_ENABLE([notify],
        AS_HELP_STRING([--enable-notify],[enable systemd notify]),
        [
          case "${enableval}" in
           yes) SYSTEMD_CFLAGS="-DENABLE_SD_NOTIFY"
                SYSTEMD_LDFLAGS="-lsystemd" ;;
           no) AC_MSG_ERROR([systemd notify is disabled]) ;;
          *) AC_MSG_ERROR([bad value ${enableval} for --enable-notify ]) ;;
           esac
           ],
         [echo "systemd notify is disabled."])

FEATURE_RDKB_WAN_MANAGER=no
AC_ARG_ENABLE([wanmgr],
        AS_HELP_STRING([--enable-wanmgr],[enable wanmgr]),
        [
          case "${enableval}" in
           yes) WANMGR_CFLAGS="-DENABLE_RDK_WANMANAGER"
                WANMGR_SOURCE_T="gw_prov_sm.c"
                WANMGR_LDFLAGS="-lsysevent -lulog -lsecure_wrapper" 
		FEATURE_RDKB_WAN_MANAGER=yes ;;
           no) AC_MSG_ERROR([wanmgr is disabled]) ;;
          *) AC_MSG_ERROR([bad value ${enableval} for --enable-notify ]) ;;
           esac
           ],
         [echo "wanmgr is disabled."])


AC_SUBST(SYSTEMD_CFLAGS)
AC_SUBST(SYSTEMD_LDFLAGS)

AC_SUBST(WANMGR_CFLAGS)
AC_SUBST(WANMGR_LDFLAGS)


# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE

# Checks for library functions.
AC_FUNC_MALLOC

AC_CONFIG_FILES(
	source/TR-181/middle_layer_src/Makefile
	source/TR-181/integration_src.shared/Makefile
	source/TR-181/Makefile
	source/CMAgentSsp/Makefile
	source/Makefile
	Makefile
)

AC_SUBST(GTEST_ENABLE_FLAG)

AM_CONDITIONAL([PLATFORM_PUMA7_ENABLED], [test $PLATFORM_PUMA7_ENABLED = yes])
AM_CONDITIONAL([FEATURE_RDKB_WAN_MANAGER], [test $FEATURE_RDKB_WAN_MANAGER = yes])
AC_OUTPUT

