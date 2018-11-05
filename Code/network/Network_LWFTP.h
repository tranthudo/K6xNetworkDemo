/**
 * @file Network_LWIP.h
 * @author ThinhNT (tienthinh@gmail.com)
 * @brief Module Network implementation via LWIP library
 * @version 0.1
 * @date 2018-10-17
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#ifndef NETWORK_LWFTP_H
#define NETWORK_LWFTP_H

#include "NetCommon.h"
#include "lwip/opt.h"
#if LWIP_NETCONN
#include <stdio.h>

#include "lwip/netif.h"
#include "lwip/sys.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/sockets.h"
#include "lwip/tcpip.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "fsl_clock_manager.h"
#include "fsl_os_abstraction.h"
#include "ethernetif.h"
#include "board.h"
#ifdef __cplusplus
extern "C" {
#endif
enum lwftp_results {
  LWFTP_RESULT_OK=0,
  LWFTP_RESULT_INPROGRESS,
  LWFTP_RESULT_LOGGED,
  LWFTP_RESULT_ERR_UNKNOWN,   /** Unknown error */
  LWFTP_RESULT_ERR_ARGUMENT,  /** Wrong argument */
  LWFTP_RESULT_ERR_MEMORY,    /** Out of memory */
  LWFTP_RESULT_ERR_CONNECT,   /** Connection to server failed */
  LWFTP_RESULT_ERR_HOSTNAME,  /** Failed to resolve server hostname */
  LWFTP_RESULT_ERR_CLOSED,    /** Connection unexpectedly closed by remote server */
  LWFTP_RESULT_ERR_TIMEOUT,   /** Connection timed out (server didn't respond in time) */
  LWFTP_RESULT_ERR_SRVR_RESP, /** Server responded with an unknown response code */
  LWFTP_RESULT_ERR_INTERNAL,  /** Internal network stack error */
  LWFTP_RESULT_ERR_LOCAL,     /** Local storage error */
  LWFTP_RESULT_ERR_FILENAME   /** Remote host could not find file */
};


/** LWFTP control connection state */
typedef enum  {
  LWFTP_CLOSED=0,
  LWFTP_CONNECTED,
  LWFTP_USER_SENT,
  LWFTP_PASS_SENT,
  LWFTP_LOGGED,
  LWFTP_TYPE_SENT,
  LWFTP_PASV_SENT,
  LWFTP_RETR_SENT,
  LWFTP_STOR_SENT,
  LWFTP_XFERING,
  LWFTP_DATAEND,
  LWFTP_QUIT,
  LWFTP_QUIT_SENT,
  LWFTP_CLOSING,
} lwftp_state_t;

// LWFTP API
/**
 * Setup LWIP thread for FTP
 * @param ipaddr
 * @param port number (usually 21)
 * @param
 */
err_t Network_LWFTP_Setup(const char *ipaddr, int port, const char* usrname, const char* passwd);
err_t Network_LWFTP_SendFile(const char *dirPath, const char *fileName);
err_t Network_LWFTP_Retrieve();
err_t Network_LWFTP_Disconnect();
char *Netwrok_LWIP_Get_ServerIP();
lwftp_state_t Network_LWIP_Get_State();

#endif //LWIP_NETCONN
#ifdef __cplusplus
}
#endif

#endif// NETWORK_LWIP_H
