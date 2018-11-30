/**
 * @file Network.c
 * @author ThinhNT (tienthinh@gmail.com)
 * @brief Module Network Implementation
 * @version 0.1
 * @date 2018-10-17
 *
 * @copyright Copyright (c) 2018
 *
 */
#include "Network.h"


static char tmpStr[128];
static char tmpPath[128];
//static char filePath[128];

#include "fsl_sdhc_card.h"
#include "fsl_sdmmc_card.h"
#include "fsl_debug_console.h"
#include "ff.h"
#include "modem.h"
#include "modem_ftp_client.h"
#include "modem_debug.h"

static uint8_t rxChar;
static uart_state_t uartState;
void uart_rx_callback(uint32_t instance, void * uartState)
{
	uart_state_t *state = (uart_state_t*)uartState;
	modem_rx_cmplt_callback(state->rxBuff, 1 );
}

SNetStt g_Network_Status;

char** str_split(char* a_str, const char a_delim)
{
	memset(tmpStr, 0x00, sizeof(tmpStr));
	strcpy(tmpStr,a_str);
	PRINTF("\r\n str_split %s\r\n", tmpStr);
	char** result    = 0;
	size_t count     = 0;
	char* tmp        = tmpStr;
	char* last_comma = 0;
	char delim[2];
	delim[0] = a_delim;
	delim[1] = 0;

	/* Count how many elements will be extracted. */
	while (*tmp)
	{
		if (a_delim == *tmp)
		{
			count++;
			last_comma = tmp;
		}
		tmp++;
	}

	/* Add space for trailing token. */
	count += last_comma < (tmpStr + strlen(tmpStr) - 1);

	/* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
	count++;

	result = malloc(sizeof(char*) * count);

	if (result)
	{
		size_t idx  = 0;
		char* token = strtok(tmpStr, delim);

		while (token)
		{
			assert(idx < count);
			*(result + idx++) = strdup(token);
			token = strtok(0, delim);
		}
		assert(idx == count - 1);
		*(result + idx) = 0;
	}

	return result;
}

/**
 * @brief Khoi tao module SIM + Ethernet, Enable DHCP
 *
 * @return int
 */
int Net_ModuleInitHw() {
	// Init for Ethernet using LWIP
	//Network_LWIP_TCP_Init();
	Network_LWIP_DHCP_Init();
	g_Network_Status.NetConStat = NET_CON_CONNECTED;
	g_Network_Status.NetIF = NET_IF_ETHERNET;
	// TODO: There should be a class to manage switch connection between
	// ETHERNET & SIMCOMMM



	// Initialize variable uartState of type uart_state_t


	// Fill in uart config data
	uart_user_config_t uartConfig = {
		.bitCountPerChar = kUart8BitsPerChar,
		.parityMode      = kUartParityDisabled,
		.stopBitCount    = kUartOneStopBit,
		.baudRate        = BOARD_MODEM_UART_BAUD
	};

	// Initialize the uart module with base address and config structure
	UART_DRV_Init(BOARD_MODEM_UART_INSTANCE, &uartState, &uartConfig);

	//Initialize the callback function
	UART_DRV_InstallRxCallback(BOARD_MODEM_UART_INSTANCE,
								  uart_rx_callback,
								  &rxChar,
								  &uartState,
								  true);

	modem_init();
//
////#define SERVER_IP 	"27.118.20.209"
////#define SERVER_PORT 	21
////#define USER_NAME "ftpuser1"
////#define PASSWORD "zxcvbnm@12"
//	modem_ftp_init("27.118.20.209", 21, "ftpuser1", "zxcvbnm@12");
//	modem_ftp_connect();
	return 1;
}
/**
 * @brief Start 2 TCP Server SIMCOM + LWIP
 * Allow maximum 5 connection
 * @param port
 * @return NetStatus
 */
NetStatus Net_TCPServerStart(int port)
{
	lwtcp_result_t ret1 = Network_LWTCPServer_Start(port);
	if (ret1 != LWTCP_RESULT_OK)
		return NET_ERR_LWIP_SERVER;
	return NET_ERR_NONE;
}
/**
 * Check if network is up via Ethernet / SIMCOMM
 * @return
 */
bool Net_Is_Up()
{
	bool ret1 = Network_LWIP_Is_Up();
	// TODO: ret2 = SIMCOMM is up
	bool ret2 = true;
	if (ret1)
		return true;
	else
	{
		if (!ret2)	return false;
	}

	return true;
}
/**
 * Set the client callback for each client connected to this server
 * @param fn
 * @return
 */
NetStatus Net_TCPServerSetCallback(ClientThread fn)
{
	lwtcp_result_t ret1 = Network_LWTCP_Set_Callback(fn);
	if (ret1 != LWTCP_RESULT_OK)
		return NET_ERR_LWIP_SERVER;
	return NET_ERR_NONE;
}
/**
 * Start A TCP Client via current Ethernet / SIMCOM
 * @param ip
 * @param port
 * @return
 */
NetStatus Net_TCPClientStart(const char* ip, int port)
{
	//TODO: Implement
	return NET_ERR_NONE;
}
/**
 * @brief Return Netstatus of IF & connection status
 *
 * @return SNetStt
 */
SNetStt Net_ModuleGetStatus() {
	return g_Network_Status;
}
/**
 * @brief Open FTP Client via Ethernet,
 * Step 1: If not success retry 10 times for each second
 * Step 2: If still failed open via Wireless 10 times for each second
 * Step 3: If still failed retry step 1 & 2 until
 * @param ip Ip address of server
 * @param port port of cmd connection
 * @param usrname username of ftp
 * @param passwd password of ftp
 * @return NetStatus
 */
NetStatus Net_FTPClientStart(const char *ip, int port, const char* usrname, const char* passwd) {

#if 0
	lwtcp_result_t ret1 = Network_LWFTP_Start(ip, port, usrname, passwd);
	if (ret1 != LWTCP_RESULT_OK)
	{

		return NET_ERR_LWIP_FTPCLIENT;
	}

	return NET_ERR_NONE;
#else
	if(Network_MDMFTP_Start(ip, port, usrname, passwd) != MDMFTP_RESULT_OK)
	{
		MODEM_DEBUG("[manhbt] MDMFTP Client start FAILED!");
		g_Network_Status.NetIF = NET_IF_UNKNOWN;
		g_Network_Status.NetConStat = NET_CON_ERR_UNKNOWN;
	}
	else
	{
		MODEM_DEBUG("[manhbt] MDMFTP Client start OK!");
		g_Network_Status.NetIF = NET_IF_UNKNOWN;
		g_Network_Status.NetConStat = NET_CON_DISCONNECTED;
	}
	return NET_ERR_LWIP_FTPCLIENT;
#endif
}
/**
 * @brief If connected then send file.
 * If file send failed retry 3 times
 *
 * @param dirPath directory in the filesystem (sdcard)
 * @param fileName file name (filename should include datetime information)
 * @return NetStatus
 */
NetStatus Net_FTPClientSendFile(const char *dirPath, const char *fileName) {
	PRINTF("\r\nNet_FTPClientSendFile (%s,%s)\r\n", dirPath, fileName);
	NetStatus ret;
	lwftp_result_t ret1 = LWFTP_RESULT_OK;
	/**
	 * TODO: Seperate filename to get the datetime information AG_SGCE_KHI001_20181107105400.txt
	 * datetime will be /2018/11/07 for folder
	 * AG_SGCE_KHI001_20181107105400.txt will be stored in that folder
	 */
	char** tokens;
	int i = 0, err, retVal;
	int len;
	char cwd[128];
	tokens= str_split(fileName, '_');//AG_SGCE_KHI001_20181107105400.txt then *tokens ->
	if (tokens) {
		while(*(tokens+i)) {
			i++;
		}
		if (i > 0) {
			i-=1;
			len = strlen(*(tokens+i));
			if (len >= 14) {
				// Do something with this filename  AG_SGCE_KHI001_20181107105400.txt
				memset(tmpStr,0,sizeof(tmpStr));
				memset(tmpPath,0,sizeof(tmpPath));
				strcpy(tmpPath, DEFAULT_FTP_FOLDER_PATH);//tmpPath = "/home/ftpuser1/test/thinh"
				strcat(tmpPath, "/"); // tmpPath = "/home/ftpuser1/test/thinh"
				memcpy(tmpStr, *(tokens+i), 4);// copy year 2018
				strcat(tmpPath, tmpStr); //tmpPath = "/home/ftpuser1/test/thinh/2018"

				memset(tmpStr,0,sizeof(tmpStr));
				memcpy(tmpStr, (*(tokens+i))+4, 2);// copy month 11
				strcat(tmpPath, ("/")); // now tmpPath = "/home/ftpuser1/test/thinh/2018/"
				strcat(tmpPath, tmpStr); // now tmpPath = "/home/ftpuser1/test/thinh/2018/11"

				memset(tmpStr,0,sizeof(tmpStr));
				memcpy(tmpStr, (*(tokens+i))+6, 2);// copy day 07
				strcat(tmpPath, ("/")); // now tmpPath = "/home/ftpuser1/test/thinh/2018/11/"
				strcat(tmpPath, tmpStr); // now tmpPath = "/home/ftpuser1/test/thinh/2018/11/07"


//				strcpy(filePath,dirPath); // filePath = "/home
//				strcat(filePath, ("/")); // now tmpPath = "/home/"
//				strcat(filePath, fileName); // now tmpPath = "/home/AG_SGCE_KHI001_20181107105400.txt"
				err = f_getcwd(cwd, sizeof(cwd));
				if(err == FR_OK) {
					LREP("current dir = %s\r\n", cwd);
					//LREP("cwd successfully\r\n");
				} else {
					LREP("cwd failed \r\n");
				}
				// Change directory to dirpath
				retVal = f_chdir(dirPath);
				if(retVal != FR_OK) {
					LREP("chdir err = %d\r\n", retVal);
				}
				// Send file
				PRINTF("SendFile: %s,%s", tmpPath, fileName);
				ret1 = Network_LWFTP_SendFile(tmpPath, fileName); // dirpath should be
				if (ret1 == LWFTP_RESULT_OK) {
					ret = NET_ERR_NONE;
				}
				/**
				 * manhbt send file via UC15
				 */
				else
				{
					ret1 = Network_MDMFTP_SendFile(tmpPath, fileName);
					LREP("Send File %s via Ethernet failed, try with 3G modem, ret_code = %d\r\n", fileName, ret1);
				}
				// change back to previous directory
				retVal = f_chdir(cwd);
				if(retVal != FR_OK) {
					LREP("chdir err = %d\r\n", retVal);
				}
			}
		}
		i = 0;
		while(*(tokens+i)) {
			free(*(tokens+i));
			i++;
		}
		free(tokens);
	} else {
		return NET_ERR_FILENAME;
	}
}

NetStatus Net_FTPClientDeleteFile(const char *path)
{
	lwftp_result_t ret1;
	if (g_Network_Status.NetIF == NET_IF_ETHERNET) {
		ret1 = Network_LWFTP_Delete(path);
		if (ret1 == LWFTP_RESULT_OK) {
			return NET_ERR_NONE;
		}
	}
	return NET_ERR_UNKNOWN;
}
/**
 * @brief Send data to server
 *
 * @param data
 * @param length
 * @return NetStatus
 */
NetStatus Net_TCPClientSendData(const uint8_t *data, uint32_t length) {


}
/**
 * @brief Send data to all client
 *
 * @param data
 * @param length
 * @return NetStatus
 */
NetStatus Net_TCPServerSendDataToAllClient(const uint8_t *data, uint32_t length) {

}
