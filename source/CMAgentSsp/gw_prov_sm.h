#ifndef _GW_GWPROV_SM_H_
#define _GW_GWPROV_SM_H_
#ifdef AUTOWAN_ENABLE
#define DEBUG_INI_NAME  "/etc/debug.ini"
#define COMP_NAME "LOG.RDK.GWPROV"
#define LOG_INFO_GWP 4

char log_buff[1024];
#ifdef FEATURE_SUPPORT_RDKLOG
#define GWPROV_PRINT(fmt ...)    {\
                                                                snprintf(log_buff, 1023, fmt);\
                                    RDK_LOG(LOG_INFO_GWP, COMP_NAME, "%s", log_buff);\
                                 }
#else
#define GWPROV_PRINT printf
#endif
#endif
#endif
