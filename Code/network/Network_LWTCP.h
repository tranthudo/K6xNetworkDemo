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
#ifndef NETWORK_LWTCP_H
#define NETWORK_LWTCP_H

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

#define LWTCP_SERVER_MAX_CLIENT 2

typedef struct {
	//int* fd;
	int idx;
} Tcp_Client_Arg;

typedef enum lwtcp_results {
  LWTCP_RESULT_OK=0,
  LWTCP_RESULT_ERR_BUSY,
  LWTCP_RESULT_ERR_OPENING,
  LWTCP_RESULT_ERR_BINDING,
  LWTCP_RESULT_ERR_LISTENING,
  LWTCP_RESULT_ERR_SERVER_CLOSED,
  LWTCP_RESULT_ERR_CLIENT_DISCONNECT,
  LWTCP_RESULT_ERR_SEND_ERR,
  LWTCP_RESULT_ERR_RECV_ERR,
  LWTCP_RESULT_ERR_TIMEOUT,
  LWTCP_RESULT_ERR_UNKNOWN,   /** Unknown error */
} lwtcp_result_t;


lwtcp_result_t Network_LWTCPServer_Start(int port);
lwtcp_result_t Network_LWTCPServer_SetClientStackSize(int _sz);
lwtcp_result_t Network_LWTCP_Set_Callback(ClientThread fn);
lwtcp_result_t Network_LWTCP_Close(int clientSocketId);
int Network_LWTCP_Send(int clientSocketfd, const uint8_t *data, uint32_t length);
int Network_LWTCP_Receive(int clientSocketfd, uint8_t *buf, uint32_t max_received_bytes);
#endif //LWIP_NETCONN
#ifdef __cplusplus
}
#endif

#endif// NETWORK_LWTCP_H
