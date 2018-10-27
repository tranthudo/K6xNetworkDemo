/*
 * This is template for main module created by KSDK Project Generator. Enjoy!
 */

/*
 * [File Name]     main.c
 * [Platform]      MK64FN1M0VLL12
 * [Project]       UCOS-III-K64F-LWIP001
 * [Version]       1.00
 * [Author]        ThinhNT2
 * [Date]          10/23/2018
 * [Language]      'C'
 * [History]       1.00 - Original Release
 *
 */

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include "lwip/opt.h"

#include "httpserver-netconn.h"

#include "Network.h"

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
//


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

/**
 * Example thread (just print Hello)
 * @param arg
 */
static void example_thread(void *arg) {
	static bool tested = false;
	while(1) {
		OSA_TimeDelay(1000);
		PRINTF("Example Thread is running\r\n");
		if (!tested) {
			if (Network_LWIP_Is_DHCP_Bound()) {
				Network_LWFTP_Test();
				tested = true;
			}
		} else {
			PRINTF("#");
		}
	}
}
/**
 * Create thread and run OSA_Start();
 */
void threads_init() {
	sys_thread_new("Example", example_thread, NULL, 1024, 4);
	Net_ModuleInitHw();
	OSA_Start();
}

char* sexyboyslogo = "\r\n"
		"                       ,@@.\r\n"
		"                   @@@%@@@@@/                                                                                                              \r\n"
		" %@@%#              @@*@@@@@%             #@@%/  .@@@@@@@@@,@@@     #@@/@@     (@@        .@@@@@@(       @@@@@%   @@#     @@,     &@@#     \r\n"
		"    @%@@@(           %@@@@@@@          ,@@@%%@@(  @@         %@@.  @@@  %@@.  ,@@.        ,@@   %@@   .@@@/,,#@@@  @@/   &@(   &@@@%%@&    \r\n"
		"        #@@@@@@#      @@@@@@@         ,@@         @@          .@@%@@(    /@@  @@#         .@@   /@@   @@/     .@@,  @@, #@@   %@@          \r\n"
		"           %@@@@&   %@@@@@@@@          @@@@@@@@. ,@@@@@@@@&     @@@,      /@@@@%          .@@@@@@&   &@%       @@*   @@%@@    *@@@@@@@/    \r\n"
		"           @@@@@@@   @@@@@*&@@            ,,/&@@,*@@,..        @@@@@       ,@@@           .@@,**&@@( @@.      /@@     @@@(       .,*(@@@   \r\n"
		"         *@@@. @@@@@@@@@@    @&       %,      %@(*@@         %@@. .@@/     .@@            .@@    #@@ &@&     .@@#     @@#    /#      *@@.  \r\n"
		"        @@*    ,@@@@@@@@      @/     ,@@@@@@@@@& .@@%(#%%%  @@&     @@@   .@@             .@@@@@@@(   &@@@@@@@@      @@/     @@@@@@@@@@.   \r\n"
		"      &@%       .@@@@@@%       @       *@@@@#     .@@@@@@@/ #        /*    @,              ,%(*         *&@@&                %@@@&/      \r\n"
		"     /@&          /@@@*        @@&\r\n\r\n";
/*!
 * @brief Main function.
 */

/*!
 * @brief Main function.
 */
#if defined(FSL_RTOS_MQX)
void Main_Task(uint32_t param)
#else
int main(void)
#endif

{
#if 0
  app_low_level_init();
  OSA_Init();
  PRINTF("\r\n====================================================\r\n");
  PRINTF("ThinhNT HTTP SERVER Built %s %s\r\n", __DATE__, __TIME__);
  LWIP_DEBUGF(HTTPD_DEBUG,("TCP/IP initializing...\r\n"));
  tcpip_init(NULL,NULL);
  LWIP_DEBUGF(HTTPD_DEBUG,("TCP/IP initialized.\r\n"));

  lwip_netconn_init();
  for(;;);
#else
	app_low_level_init();
	OSA_Init();
	PRINTF("%s", sexyboyslogo);
	PRINTF("SBoys - Network Module Example Built  %s %s\r\n", __DATE__, __TIME__);
	LWIP_DEBUGF(NETMODULE_DEBUG,("Threads initializing...\r\n"));
	threads_init();
	for (;;);
#endif
}


