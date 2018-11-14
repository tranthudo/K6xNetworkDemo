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

static void
dhcp_client_thread(void *arg) {
	PRINTF("DHCP CLIENT THREAD created\r\n");
	bDHCP_Client_Not_Created == false;
	OSA_TimeDelay(1000);
	dhcp_start(&fsl_netif0);
	static bool last_link_status = true;//link status default = down
	static bool link_status = true;
	while (1) {
#if USE_DHCP
		//ethernetif_input(&fsl_netif0, packetBuffer);
		//PRINTF("*");
		//sys_check_timeouts();
		print_dhcp_state(&fsl_netif0);
		if (Network_LWIP_Is_DHCP_Bound()) {
			OSA_TimeDelay(6000);
		}
		OSA_TimeDelay(1000);
		if (netif_is_up(&fsl_netif0)) {
			PRINTF("fsl_netif0 is up\r\n");
		} else {
			PRINTF("fsl_netif0 is NOT up\r\n");
		}
		link_status = PHY_Get_Initialized_LinkStatus();
		if (link_status == true && last_link_status == false) {
			// start dhcp again
			PRINTF("Link_status changed from DOWN --> UP, restart DHCP\r\n");
			dhcp_start(&fsl_netif0);
		}
		else if (link_status == false && last_link_status ==true) {
			// stop dhcp and restart when needed
			PRINTF("Link_status changed from UP --> DOWN, stop DHCP\r\n");
			dhcp_stop(&fsl_netif0);
		}
		else {
			if (link_status) {
				PRINTF("Link status is remaining UP\r\n");
			}
			else {
				PRINTF("Link status's remaining DOWN\r\n");
			}
		}
		last_link_status = link_status;
#else
		OSA_TimeDelay(1000);
		PRINTF("DHCP Thread \r\n");
#endif
	}
}

void Network_LWIP_TCP_Init() {
	PRINTF("FUCK Network_LWIP_TCP_Init\r\n");

}

void static lwip_netif_changed_callback(struct netif* netif) {
	PRINTF("\r\n=========>>>>lwip_netif_changed_callback\r\n");
}

void static lwip_netif_link_changed_callback(struct netif* netif) {
	PRINTF("\r\n=========>>>>lwip_netif_link_changed_callback\r\n");
}

void Network_LWIP_DHCP_Init() {
	if (bDHCP_Client_Not_Created) {
		tcpip_init(NULL,NULL);
		//bDHCP_Client_Not_Created = false;
		PRINTF("[ThinhNT] Try to start DHCP\r\n");
		ip_addr_t fsl_netif0_ipaddr, fsl_netif0_netmask, fsl_netif0_gw;
#if USE_DHCP
		IP4_ADDR(&fsl_netif0_ipaddr, 0,0,0,0);
		IP4_ADDR(&fsl_netif0_netmask, 0,0,0,0);
		IP4_ADDR(&fsl_netif0_gw, 0,0,0,0);
		netif_add(&fsl_netif0, &fsl_netif0_ipaddr, &fsl_netif0_netmask, &fsl_netif0_gw, NULL, ethernetif_init, tcpip_input);
		netif_set_default(&fsl_netif0);
		//netif_set_up(&fsl_netif0);
		/// Setup callback for DHCP
		netif_set_status_callback(&fsl_netif0,lwip_netif_changed_callback);

		netif_set_link_callback(&fsl_netif0,lwip_netif_link_changed_callback);

		//enet_mac_packet_buffer_t *packetBuffer;
#else
		IP4_ADDR(&fsl_netif0_ipaddr, 10,2,82,128);
		IP4_ADDR(&fsl_netif0_netmask, 255,255,255,0);
		IP4_ADDR(&fsl_netif0_gw, 10,2,82,1);
		netif_add(&fsl_netif0, &fsl_netif0_ipaddr, &fsl_netif0_netmask, &fsl_netif0_gw, NULL, ethernetif_init, tcpip_input);
		netif_set_default(&fsl_netif0);
		bIPAddrOK = true;
#endif
		sys_thread_new("dhcp_client", dhcp_client_thread, NULL, 1024, 4);
	}
}


bool Network_LWIP_Is_DHCP_Bound()
{
	if (fsl_netif0.dhcp == NULL)
		return false;
	return (fsl_netif0.dhcp->state == DHCP_BOUND);
}

bool Network_LWIP_Is_OK()  {
	if (!bDHCP_Client_Not_Created) {
		return (Network_LWIP_Is_DHCP_Bound() && PHY_Get_Initialized_LinkStatus());
	} else {
		// TODO: added when add static ip address
		return false;
	}
}
