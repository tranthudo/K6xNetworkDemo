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
#include "lwip/opt.h"
#if LWIP_NETCONN
#include <stdio.h>

#include "lwip/netif.h"
#include "lwip/sys.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/tcpip.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "fsl_clock_manager.h"
#include "fsl_os_abstraction.h"
#include "ethernetif.h"
#include "board.h"
/**
 * Init TCP IP
 */
void Network_LWIP_TCP_Init();
/**
 * Initialize LWIP DHCP
 */
void Network_LWIP_DHCP_Init();
bool Network_LWIP_Is_DHCP_Bound();
#endif //LWIP_NETCONN
