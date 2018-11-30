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
// Standard C Included Files
#include <stdlib.h>
#include <string.h>

// SDK Included Files
#include "fsl_clock_manager.h"
#include "fsl_os_abstraction.h"
#include "board.h"
#include "fsl_mpu_driver.h"
#include "fsl_sdhc_card.h"
#include "fsl_sdmmc_card.h"
#include "fsl_debug_console.h"
#include "ff.h"

//#include "lwip/opt.h"
#include "Network.h"
#include "Network_Test.h"

#ifndef HTTPD_DEBUG
#define HTTPD_DEBUG         LWIP_DBG_ON
#endif
#ifndef HTTPD_STACKSIZE
#define HTTPD_STACKSIZE        3000
#endif
#ifndef HTTPD_PRIORITY
#define HTTPD_PRIORITY          3
#endif

#include "modem.h"
#include "modem_ftp_client.h"


///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////
#define TASK_SHELL_PRIO         		6U
#define TASK_MODBUS_PRIO          		4U
#define TASK_FILESYSTEM_PRIO         	5U
#define TASK_SERIALCOMM_PRIO			7U
#define TASK_PERIODIC_PRIO				8U

// task size
#define TASK_SHELL_STACK_SIZE   		1024U
#define TASK_MODBUS_STACK_SIZE    		1048U
#define TASK_FILESYSTEM_STACK_SIZE   	1024U
#define TASK_SERIAL_COMM_STACK_SIZE		1024U
#define TASK_PERIODIC_STACK_SIZE		3072U

OSA_TASK_DEFINE(task_periodic,		TASK_PERIODIC_STACK_SIZE);

void App_TaskPeriodic(void *param);

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//struct netif fsl_netif0;
const static bool bIPAddrOK = false;

FATFS FatFs_SDCARD;

FIL writer;

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

    // enable SDHC module instance  ==> added for SDHC
	CLOCK_SYS_EnableSdhcClock(0);
    // Open ENET clock gate ==> added for SDHC
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
		OSA_TimeDelay(5000);
		PRINTF("Example Thread is running\r\n");
		if (!tested) {
			if (Net_Is_Up()) {
				Net_TestFTP();
			}
//			if (Network_LWIP_Is_Up()) {
//				//Test TCP server using LWIP
//				Network_LWTCPServer_Start(2011);
//				Network_LWTCP_Set_Callback(Network_LWTCP_Echo_ClientCallback);
//				//Test FTP Client using LWIP
//				Network_LWFTP_Test2();
//				tested = true;
//			}
		} else {
			PRINTF("#");
		}
	}
}
/**
 * Create thread and run OSA_Start();
 */
void threads_init() {
	// this one is for hardware init of lwip
	Net_ModuleInitHw();
	// testcode for fatfs
	osa_status_t retVal;
    retVal = OSA_TaskCreate(App_TaskPeriodic,
					(uint8_t *)"periodic",
					TASK_PERIODIC_STACK_SIZE,
					task_periodic_stack,
					TASK_PERIODIC_PRIO,
					(task_param_t)0,
					false,
					&task_periodic_task_handler);
	if (retVal != kStatus_OSA_Success)
	{
		LREP("Failed to create periodic task\r\n\r\n");
		return -1;
	} else {
		LREP("Task create successfully \r\n");
	}
	// end of hai dong code

	sys_thread_new("Example", example_thread, NULL, 4096, 8);
	OSA_Start();
}

char* sexyboyslogo = "\r\n Coypright(C) 2018 S-Boys Group \r\n";

//char* sexyboyslogo = "\r\n"
//		"                       ,@@.\r\n"
//		"                   @@@%@@@@@/                                                                                                              \r\n"
//		" %@@%#              @@*@@@@@%             #@@%/  .@@@@@@@@@,@@@     #@@/@@     (@@        .@@@@@@(       @@@@@%   @@#     @@,     &@@#     \r\n"
//		"    @%@@@(           %@@@@@@@          ,@@@%%@@(  @@         %@@.  @@@  %@@.  ,@@.        ,@@   %@@   .@@@/,,#@@@  @@/   &@(   &@@@%%@&    \r\n"
//		"        #@@@@@@#      @@@@@@@         ,@@         @@          .@@%@@(    /@@  @@#         .@@   /@@   @@/     .@@,  @@, #@@   %@@          \r\n"
//		"           %@@@@&   %@@@@@@@@          @@@@@@@@. ,@@@@@@@@&     @@@,      /@@@@%          .@@@@@@&   &@%       @@*   @@%@@    *@@@@@@@/    \r\n"
//		"           @@@@@@@   @@@@@*&@@            ,,/&@@,*@@,..        @@@@@       ,@@@           .@@,**&@@( @@.      /@@     @@@(       .,*(@@@   \r\n"
//		"         *@@@. @@@@@@@@@@    @&       %,      %@(*@@         %@@. .@@/     .@@            .@@    #@@ &@&     .@@#     @@#    /#      *@@.  \r\n"
//		"        @@*    ,@@@@@@@@      @/     ,@@@@@@@@@& .@@%(#%%%  @@&     @@@   .@@             .@@@@@@@(   &@@@@@@@@      @@/     @@@@@@@@@@.   \r\n"
//		"      &@%       .@@@@@@%       @       *@@@@#     .@@@@@@@/ #        /*    @,              ,%(*         *&@@&                %@@@&/      \r\n"
//		"     /@&          /@@@*        @@&\r\n\r\n";

const char *msg = "FS Waking up ok " __TIME__ " " __DATE__;
bool is_existed(const char *path) {
	FRESULT fr;
	FILINFO fno;
	LREP("Test for %s\r\n", path);
	bool exist = false;

	fr = f_stat(path, &fno);
	switch (fr) {
	case FR_OK:
		LREP("Size: %lu\r\n", fno.fsize);
		LREP("Timestamp: %u/%02u/%02u, %02u:%02u\r\n",
			   (fno.fdate >> 9) + 1980, fno.fdate >> 5 & 15, fno.fdate & 31,
			   fno.ftime >> 11, fno.ftime >> 5 & 63);
		LREP("Attributes: %c%c%c%c%c\r\n",
			   (fno.fattrib & AM_DIR) ? 'D' : '-',
			   (fno.fattrib & AM_RDO) ? 'R' : '-',
			   (fno.fattrib & AM_HID) ? 'H' : '-',
			   (fno.fattrib & AM_SYS) ? 'S' : '-',
			   (fno.fattrib & AM_ARC) ? 'A' : '-');
		exist = true;
		break;

	case FR_NO_FILE:
		LREP("It is not exist.\r\n");
		break;

	default:
		LREP("An error occured. (%d)\r\n", fr);
	}

	return exist;
}


char buffer[300];

void App_FS() {
	int retVal;
	memset(&FatFs_SDCARD, 0, sizeof(FATFS));

	FIL fil; // file object
	UINT btr; // number of file to read
	UINT br; // number of read bytes
	FRESULT fr; // FatFS return code
	size_t len; // written length

	retVal = f_mount(&FatFs_SDCARD, "", 0);

	if(retVal != FR_OK) {
		PRINTF("fat fs init error: %d\r\n", retVal);
	} else {
		PRINTF("fat fs init successful !\r\n");

		is_existed("dir2");

		retVal = f_mkdir("dir2");
		if(retVal != FR_OK) {
			PRINTF("mkdir err = %d\r\n", retVal);
		} else {
			PRINTF("mkdir successful !\r\n");

		}

		retVal = f_chdir("/dir2");
		if(retVal != FR_OK) {
			LREP("chdir err = %d\r\n", retVal);
		}

		retVal = f_open(&writer, "FILE1.txt", FA_CREATE_ALWAYS | FA_WRITE | FA_READ);

		if(retVal != FR_OK) {
			PRINTF("open file error: %d\r\n", retVal);
		} else {
			UINT byte_written, size;
			size = f_size(&writer);
			PRINTF("file size = %d\r\n", size);
			if(size > 0) {
				PRINTF("file size = %d bytes\r\n", size);
			} else {
				char* msg = "new fs test write to file\r\n";
				retVal = f_write(&writer, (void*)msg, strlen(msg), &byte_written);
				if(retVal != FR_OK) {
					PRINTF("write to byte failed err: %d\r\n", retVal);
				} else {
					if(byte_written != strlen(msg)) {
						PRINTF("write to file missing data, writereq = %d - writeact: %d\r\n", strlen(msg), byte_written);
					} else {
						PRINTF("write to file totally successful !\r\n");
					}
				}
			}
		}

		f_close(&writer);
		OSA_TimeDelay(1000);
		memset(buffer, 0, 300);
		LREP("start get cwd \r\n");
		int len = 300;
		int err = f_getcwd(buffer, len);

		if(err == FR_OK) {
			LREP("current dir = %s\r\n", buffer);
			LREP("cwd successfully\r\n");
		} else {
			LREP("cwd failed \r\n");
		}
		retVal = f_chdir("/");
		if(retVal != FR_OK) {
			LREP("chdir err = %d\r\n", retVal);
		}
		err = f_getcwd(buffer, len);

		if(err == FR_OK) {
			LREP("current dir = %s\r\n", buffer);
			LREP("cwd successfully\r\n");
		} else {
			LREP("cwd failed \r\n");
		}
		PRINTF("============ThinhNT test read file============\r\n");
		fr = f_open(&fil, "./thinhnt/AG_SGCE_KHI001_20181107105400.txt", FA_READ);
		btr = LWFTP_CONST_LINE_SIZE;
		if (fr == FR_OK) {
			do {
				memset(buffer, 0, 300);
				// read from file file to buffer reply_msg
				fr = f_read(&fil, buffer, btr, &br);
				if (fr != FR_OK) {
					PRINTF("LWFTP read file failed\r\n");
					return LWFTP_RESULT_ERR_LOCAL;
				}
				if (br > 0) {
					PRINTF("Read %d bytes: %s\r\n", br, buffer);
				}
			}
			while (br == btr); // while not reached the EOF
		} else {
			PRINTF("LWFTP open file failed\r\n");
			return LWFTP_RESULT_ERR_LOCAL;
		}
		// close the file
		f_close(&fil);

		OSA_TimeDelay(1000);


		memset(buffer, 0, 300);

		err = f_getcwd(buffer, len);

		if(err == FR_OK) {
			LREP("current dir = %s\r\n", buffer);
			LREP("cwd successfully\r\n");
		} else {
			LREP("cwd failed \r\n");
		}


	}
}

void App_TaskPeriodic(void *param) {

	bool run = false;
	bool testTCP = false;

	while(1) {
		if(run == false) {
			run = true;
			App_FS();

		}
//		if (testTCP == false) {
//			// ThinhNT Added to test TCP
//			if (Net_Is_Up()) {
//				testTCP = true;
//				Net_TestTCPServer();
//			}
//		}
//		modem_send_at_command("AT\r\n", "OK", 1000, 1);
		OSA_TimeDelay(1000);
		LREP(".");
	}
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
	//haidong: disable mpu dont know what it is
//#ifdef MPU_INSTANCE_COUNT
//    // disable MPU
//	int i;
//    for(i = 0; i < MPU_INSTANCE_COUNT; i++)
//    {
//        MPU_HAL_Disable(g_mpuBase[i]);
//    }
//#endif

	PRINTF("%s", sexyboyslogo);
	PRINTF("SBoys - Network Module Example Built  %s %s\r\n", __DATE__, __TIME__);
	LWIP_DEBUGF(NETMODULE_DEBUG,("Threads initializing...\r\n"));
	threads_init();
	for (;;);
#endif
}
