/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include "lwip/opt.h"

#include "httpserver-netconn.h"

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

#ifndef HTTPD_DEBUG
#define HTTPD_DEBUG         LWIP_DBG_ON
#endif
#ifndef HTTPD_STACKSIZE
#define HTTPD_STACKSIZE        3000
#endif
#ifndef HTTPD_PRIORITY
#define HTTPD_PRIORITY          3
#endif

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

const static char http_html_hdr[] = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
const static char http_index_html[] = "<html><head><title>Congrats!</title></head><body><h1>Welcome to our lwIP HTTP server!</h1><p>This is a small test page, served by httpserver-netconn.</body></html>";
struct netif fsl_netif0;
const static bool bIPAddrOK = false;

#if defined(FSL_RTOS_MQX)
void Main_Task(uint32_t param);
TASK_TEMPLATE_STRUCT  MQX_template_list[] =
{
    { 1L,     Main_Task,      3000L,  MQX_MAIN_TASK_PRIORITY, "Main",      MQX_AUTO_START_TASK},
    { 0L,     0L,             0L,    0L, 0L,          0L }
};
#endif

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

/*!
 * @brief Serve one HTTP connection accepted in the http thread.
 */
static void http_server_netconn_serve(struct netconn *conn)
{
    struct netbuf *inbuf;
    char *buf;
    u16_t buflen;
    err_t err;

    // Read the data from the port, blocking if nothing yet there. 
    // We assume the request (the part we care about) is in one netbuf
    err = netconn_recv(conn, &inbuf);

    if (err == ERR_OK)
    {
        netbuf_data(inbuf, (void**)&buf, &buflen);

        // Is this an HTTP GET command? (only check the first 5 chars, since
        // there are other formats for GET, and we're keeping it very simple )
        if (buflen>=5 &&
                buf[0]=='G' &&
                buf[1]=='E' &&
                buf[2]=='T' &&
                buf[3]==' ' &&
                buf[4]=='/' )
        {

            // Send the HTML header 
            // subtract 1 from the size, since we dont send the \0 in the string
            // NETCONN_NOCOPY: our data is const static, so no need to copy it
            netconn_write(conn, http_html_hdr, sizeof(http_html_hdr)-1, NETCONN_NOCOPY);

            // Send our HTML page
            netconn_write(conn, http_index_html, sizeof(http_index_html)-1, NETCONN_NOCOPY);
        }
    }
    // Close the connection (server closes in HTTP)
    netconn_close(conn);

    // Delete the buffer (netconn_recv gives us ownership,
    // so we have to make sure to deallocate the buffer)
    netbuf_delete(inbuf);
}

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

bool IsDHCPBound(struct netif *netif) {
	if (netif == NULL)
		return false;
	struct dhcp *dhcp = netif->dhcp;
	return (dhcp->state == DHCP_BOUND);
}


/*!
 * @brief The main function containing server thread.
 */
#define USE_DHCP 1
static void
dhcp_client_thread(void *arg) {
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
	enet_mac_packet_buffer_t *packetBuffer;
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
		sys_check_timeouts();
		print_dhcp_state(&fsl_netif0);
#else
		OSA_TimeDelay(1000);
		PRINTF("DHCP Thread \r\n");
#endif
	}
}

/*!
 * @brief The main function containing server thread.
 */
static void
http_server_netconn_thread(void *arg)
{
#if 1
    struct netconn *conn, *newconn;
    err_t err;
    LWIP_UNUSED_ARG(arg);
    PRINTF("HTTP Thread Started\r\n");
    while (!IsDHCPBound(&fsl_netif0)) {
    	OSA_TimeDelay(1000);
    	PRINTF(".");
    }
    netif_set_up(&fsl_netif0);
    // Create a new TCP connection handle
    conn = netconn_new(NETCONN_TCP);
    LWIP_ERROR("http_server: invalid conn", (conn != NULL), return;);

    // Bind to port 80 (HTTP) with default IP address
    netconn_bind(conn, NULL, 80);

    // Put the connection into LISTEN state
    netconn_listen(conn);

    do {
        err = netconn_accept(conn, &newconn);
        if (err == ERR_OK) {
            http_server_netconn_serve(newconn);
            netconn_delete(newconn);
        }
    } while(err == ERR_OK);
    LWIP_DEBUGF(HTTPD_DEBUG,
    ("http_server_netconn_thread: netconn_accept received error %d, shutting down", err));
    netconn_close(conn);
    netconn_delete(conn);
#else
    while (1) {
		OSA_TimeDelay(1000);
		PRINTF("HTTP Thread \r\n");
    }
#endif
}

/*!
 * @brief Initialize the HTTP server.
 */
void lwip_netconn_init()
{
	PRINTF("lwip_netconn_init");
	sys_thread_new("dhcp_client", dhcp_client_thread, NULL, 1024, 4);
    sys_thread_new("http_server_netconn", http_server_netconn_thread, NULL, HTTPD_STACKSIZE, HTTPD_PRIORITY);
    OSA_Start();
}

/*!
 * @brief Initialize hardware.
 */
static void app_low_level_init(void)
{
    // Open UART module for debug
    hardware_init();

    // Open ENET clock gate
    CLOCK_SYS_EnableEnetClock(0);
    // Select PTP timer OUTCLK
    CLOCK_SYS_SetEnetTimeStampSrc(0, kClockTimeSrcOsc0erClk);

    // Disable the mpu
    MPU_BWR_CESR_VLD(MPU, 0);
}

/*!
 * @brief Main function.
 */
#if defined(FSL_RTOS_MQX)
void Main_Task(uint32_t param)
#else
int main(void)
#endif

{


  app_low_level_init();
  OSA_Init();
  PRINTF("\r\n====================================================\r\n");
  PRINTF("ThinhNT HTTP SERVER Built %s %s\r\n", __DATE__, __TIME__);
  LWIP_DEBUGF(HTTPD_DEBUG,("TCP/IP initializing...\r\n"));
  tcpip_init(NULL,NULL);
  LWIP_DEBUGF(HTTPD_DEBUG,("TCP/IP initialized.\r\n"));

  lwip_netconn_init();
  for(;;);
}

#endif // LWIP_NETCONN
