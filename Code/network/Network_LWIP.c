/**
 * @file Network_LWIP.c
 * @author ThinhNT (tienthinh@gmail.com)
 * @brief Module Network Implementation via Ethernet using LWIP library
 * @version 0.1
 * @date 2018-10-17
 *
 * @copyright Copyright (c) 2018
 *
 */
#include "Network_LWIP.h"

// Define variables to use for network interface with LWIP
static struct netif fsl_netif0; ///< network interface
static bool bIPAddrOK = false; ///< Check DHCP Bound to an address
static bool bDHCP_Client_Not_Created = true;

/*!
 * @brief Prints DHCP status of the interface when it has changed from last status.
 *
 * @param netif network interface structure
 */
static void print_dhcp_state(struct netif *netif)
{
    static u8_t dhcp_last_state = DHCP_OFF;
    //struct dhcp *dhcp = (struct dhcp *)netif_get_client_data(netif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);
    struct dhcp *dhcp = netif->dhcp;
    if (dhcp_last_state != dhcp->state)
    {
        dhcp_last_state = dhcp->state;

        PRINTF(" DHCP state       : ");
        switch (dhcp_last_state)
        {
            case DHCP_OFF:
                PRINTF("OFF");
                break;
            case DHCP_REQUESTING:
                PRINTF("REQUESTING");
                break;
            case DHCP_INIT:
                PRINTF("INIT");
                break;
            case DHCP_REBOOTING:
                PRINTF("REBOOTING");
                break;
            case DHCP_REBINDING:
                PRINTF("REBINDING");
                break;
            case DHCP_RENEWING:
                PRINTF("RENEWING");
                break;
            case DHCP_SELECTING:
                PRINTF("SELECTING");
                break;
            case DHCP_INFORMING:
                PRINTF("INFORMING");
                break;
            case DHCP_CHECKING:
                PRINTF("CHECKING");
                break;
            case DHCP_BOUND:
                PRINTF("BOUND");
                break;
            case DHCP_BACKING_OFF:
                PRINTF("BACKING_OFF");
                break;
            default:
                PRINTF("%u", dhcp_last_state);
                assert(0);
                break;
        }
        PRINTF("\r\n");

        if (dhcp_last_state == DHCP_BOUND)
        {
            PRINTF("\r\n IPv4 Address     : %u.%u.%u.%u\r\n", ((u8_t *)&netif->ip_addr.addr)[0],
                   ((u8_t *)&netif->ip_addr.addr)[1], ((u8_t *)&netif->ip_addr.addr)[2],
                   ((u8_t *)&netif->ip_addr.addr)[3]);
            PRINTF(" IPv4 Subnet mask : %u.%u.%u.%u\r\n", ((u8_t *)&netif->netmask.addr)[0],
                   ((u8_t *)&netif->netmask.addr)[1], ((u8_t *)&netif->netmask.addr)[2],
                   ((u8_t *)&netif->netmask.addr)[3]);
            PRINTF(" IPv4 Gateway     : %u.%u.%u.%u\r\n\r\n", ((u8_t *)&netif->gw.addr)[0],
                   ((u8_t *)&netif->gw.addr)[1], ((u8_t *)&netif->gw.addr)[2], ((u8_t *)&netif->gw.addr)[3]);
        }
    }
}

/*!
 * @brief The main function containing server thread.
 */
#define USE_DHCP 1
static void
dhcp_client_thread(void *arg) {
	PRINTF("DHCP CLIENT THREAD created\r\n");
	ip_addr_t fsl_netif0_ipaddr, fsl_netif0_netmask, fsl_netif0_gw;
#if USE_DHCP
	IP4_ADDR(&fsl_netif0_ipaddr, 0,0,0,0);
	IP4_ADDR(&fsl_netif0_netmask, 0,0,0,0);
	IP4_ADDR(&fsl_netif0_gw, 0,0,0,0);
	netif_add(&fsl_netif0, &fsl_netif0_ipaddr, &fsl_netif0_netmask, &fsl_netif0_gw, NULL, ethernetif_init,
			ethernet_input);
	netif_set_default(&fsl_netif0);
	netif_set_up(&fsl_netif0);

	dhcp_start(&fsl_netif0);
	//enet_mac_packet_buffer_t *packetBuffer;
#else
  IP4_ADDR(&fsl_netif0_ipaddr, 10,2,82,128);
  IP4_ADDR(&fsl_netif0_netmask, 255,255,255,0);
  IP4_ADDR(&fsl_netif0_gw, 10,2,82,1);
  netif_add(&fsl_netif0, &fsl_netif0_ipaddr, &fsl_netif0_netmask, &fsl_netif0_gw, NULL, ethernetif_init, tcpip_input);
  netif_set_default(&fsl_netif0);
  bIPAddrOK = true;
#endif
	while (1) {
#if USE_DHCP
		//ethernetif_input(&fsl_netif0, packetBuffer);
		//PRINTF("*");
		//sys_check_timeouts();
		print_dhcp_state(&fsl_netif0);
		if (Network_LWIP_Is_DHCP_Bound()) {
			//OSA_TimeDelay(60000);
		}
#else
		OSA_TimeDelay(1000);
		PRINTF("DHCP Thread \r\n");
#endif
	}
}

void Network_LWIP_TCP_Init() {
	PRINTF("FUCK Network_LWIP_TCP_Init\r\n");
	tcpip_init(NULL,NULL);
}

void Network_LWIP_DHCP_Init() {
	if (bDHCP_Client_Not_Created) {
		bDHCP_Client_Not_Created = false;
		PRINTF("[ThinhNT] TRY TO CREATE DHCP CLIENT THREAD\r\n");
		sys_thread_new("dhcp_client", dhcp_client_thread, NULL, 1024, 4);
	}
}


bool Network_LWIP_Is_DHCP_Bound()
{
	if (fsl_netif0.dhcp == NULL)
		return false;
	return (fsl_netif0.dhcp->state == DHCP_BOUND);
}
