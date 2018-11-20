/**
 * @file Network_LWIP.c
 * @author ThinhNT (tienthinh@gmail.com)
 * @brief Module Network FTP Implementation via Ethernet using LWIP library
 * the Network Module use Laurent GONZALEZ <lwip@gezedo.com> simple FTP client
 * from https://github.com/gezedo/lwftp
 * @version 0.1
 * @date 2018-10-17
 *
 * @copyright Copyright (c) 2018
 *
 */

#include <string.h>
#include "Network_LWFTP.h"
#include "lwip/tcp.h"
#include "lwip/tcpip.h"
#include "ff.h"

/** Enable debugging for LWFTP */
#ifndef LWFTP_DEBUG
#define LWFTP_DEBUG   LWIP_DBG_ON
#endif

#define LWFTP_TRACE   (LWFTP_DEBUG|LWIP_DBG_TRACE)
#define LWFTP_STATE   (LWFTP_DEBUG|LWIP_DBG_STATE)
#define LWFTP_WARNING (LWFTP_DEBUG|LWIP_DBG_LEVEL_WARNING)
#define LWFTP_SERIOUS (LWFTP_DEBUG|LWIP_DBG_LEVEL_SERIOUS)
#define LWFTP_SEVERE  (LWFTP_DEBUG|LWIP_DBG_LEVEL_SEVERE)

#define PTRNLEN(s)  s,(sizeof(s)-1)
//static char* lwftp_curdir;
static bool lwftp_connected = false;
static char* lwftp_ip;
static char* lwftp_user;
static char* lwftp_passwd;
static int lwftp_port;
struct timeval tv;
static lwftp_state_t lwftp_state = LWFTP_CLOSED;
static int 	socket_ctrl = NULL, socket_dat = NULL;
static struct 	sockaddr_in server_ctrl, server_dat;
//char* 	request_msg,reply_msg;
static char 	tmpStr[LWFTP_CONST_BUF_SIZE+8];
static char 	request_msg[LWFTP_CONST_BUF_SIZE+8], reply_msg[LWFTP_CONST_BUF_SIZE+8], data_buf[LWFTP_CONST_BUF_SIZE+8];
static int lwftp_bufsize = LWFTP_CONST_BUF_SIZE;

// Define static functions
static lwftp_result_t lwftp_reconnect();
static lwftp_result_t lwftp_connect();

/**
 * Reconnect in case not connected
 * @return LWFTP_RESULT_OK if connected
 */
static lwftp_result_t lwftp_reconnect(){
	if (!Network_LWIP_Is_Up()) {
		lwftp_connected = false;
		lwftp_state = LWFTP_CLOSED;
		PRINTF("lwftp_reconnect Failed because ethernet down \r\n");
		return LWFTP_RESULT_ERR_CONNECT;
	}
	lwftp_result_t ret = LWFTP_RESULT_OK;
	int iTries = 0;
	while (iTries++ < 10) {
		// Step 1: Connect to the server if not connected
		if (lwftp_connected != true)
		{
			if (lwftp_ip != NULL)
			{
				ret = lwftp_connect();
			}
			if (ret != LWFTP_RESULT_OK) {
				OSA_TimeDelay(1000); // Delay 1s for ... fucking boy
				PRINTF("LWFTP retry connect %d times ...\r\n", iTries);
				continue; // retries
			}
		} else {
			break;
		}
	}
	return ret;
}

/**
 * Receive socket data from socket and to buffer
 * @note use this for socket receive several times continuously
 * @param sock socket to receive
 * @param buf buffer to receive
 * @param bufsize buffer size to receive
 * @return number of received bytes
 */
static int lwftp_receive_ctrl_data(int sock, char* buf, int bufsize) {
	int resp_len = 0, total_len=0;
	int max_receive_byte = bufsize;
	do {
		max_receive_byte -= resp_len;
		resp_len = recv(sock, reply_msg+total_len, max_receive_byte, 0);
		total_len+= resp_len;
		if (resp_len > 0)
			PRINTF("reply_msg (len = %d):  %s\r\n",resp_len, reply_msg);

	} while (resp_len > 0 && resp_len < lwftp_bufsize);
	return resp_len;
}




static bool lwftp_is_busy()
{
	static bool lwftp_busy = false;
	return lwftp_busy;
}
/**
 * Connect using already setup ip address of server + user/pass
 * @return result of connection
 */
static lwftp_result_t lwftp_connect()
{
	int ret;
	if (socket_ctrl != NULL) {
		close(socket_ctrl);
	}
	socket_ctrl = socket(AF_INET, SOCK_STREAM, 0);


	if (socket_ctrl == -1)
	{
		perror("Could not create socket");
		lwftp_connected = false;
		return LWFTP_RESULT_ERR_CONNECT;
	}
	tv.tv_sec = 3000; //dkm co gi do sai sai o day khi dung lwip 3000s ma nhu la 3s
	tv.tv_usec = 10000;
	// set timeout of socket
	setsockopt(socket_ctrl, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
	server_ctrl.sin_addr.s_addr = inet_addr(lwftp_ip);
	server_ctrl.sin_family = AF_INET;
	server_ctrl.sin_port = htons(lwftp_port);

	// Connect to server
	ret = connect(socket_ctrl, (struct sockaddr *)&server_ctrl, sizeof(server_ctrl));
	if (ret < 0)
	{
		perror("Connection failed");
		lwftp_connected = false;
		return LWFTP_RESULT_ERR_CONNECT;
	} else {
		PRINTF("Connected\r\n");
		ret = LWFTP_RESULT_OK;
		lwftp_connected = true;
	}
	lwftp_receive_ctrl_data(socket_ctrl, reply_msg, lwftp_bufsize);

	// Enter logging information USERNAME
	strcpy(request_msg, "USER ");
	strcat(request_msg, lwftp_user);
	strcat(request_msg, "\r\n");
	PRINTF("Enter Loggin INformation: %s\r\n", request_msg);
	write(socket_ctrl, request_msg, strlen(request_msg));
	memset(reply_msg,0x00,lwftp_bufsize);
	lwftp_receive_ctrl_data(socket_ctrl, reply_msg, lwftp_bufsize);
	// Enter logging information PASSWORD
	strcpy(request_msg, "PASS ");
	strcat(request_msg, lwftp_passwd);
	strcat(request_msg, "\r\n");
	PRINTF("Enter Password: %s\r\n", lwftp_passwd);
	write(socket_ctrl, request_msg, strlen(request_msg));
	memset(reply_msg,0x00,lwftp_bufsize);
	lwftp_receive_ctrl_data(socket_ctrl, reply_msg, lwftp_bufsize);
	lwftp_state = LWFTP_CONNECTED;
	return LWFTP_RESULT_OK;
}


lwftp_result_t Network_LWFTP_Start(const char *ip, int port, const char* usrname, const char* passwd)
{
	int ret;
	// TODO: check condition before openning new connection
	lwftp_ip = ip;
	lwftp_port = port;
	lwftp_passwd = passwd;
	lwftp_user = usrname;
	return LWFTP_RESULT_OK;
	// TODO: check if have to connect or not
	//ret = lwftp_connect();
	if (ret != 0) {
		return LWFTP_RESULT_ERR_CONNECT;
	} else {
		return LWFTP_RESULT_OK;
	}
}

lwftp_result_t Network_LWFTP_SendFile(const char *dirPath, const char *fileName)
{
	if (!Network_LWIP_Is_Up()) {
		PRINTF("Network_LWFTP_SendFile Failed because ethernet down or DHCP is not Bound \r\n");
		return LWFTP_RESULT_ERR_CONNECT;
	}
	lwftp_result_t ret = LWFTP_RESULT_ERR_UNKNOWN;
	lwftp_result_t result;
	char path[256];
	int iTries = 0;
	unsigned int a,b,c,d,e,f;
	int resp_len = 0, response;
	int max_receive_byte = lwftp_bufsize;
	//(void)(a,b,c,d,e,f);
	char *ptr;
	int 	resp_code;
	unsigned long data_port; // port for data
	FIL fil; // file object
	UINT btr; // number of file to read
	UINT br; // number of read bytes
	FRESULT fr; // FatFS return code
	size_t len; // written length
	PRINTF("\r\n=====Network_LWFTP_SendFile: %s, %s=======\r\n", dirPath, fileName);
	// Connect and try at least 10 times
	// TODO: Step 1: Check the connection, if not connect connect and try
	while (iTries++ < 10) {
		// Step 1: Connect to the server if not connected
		result = lwftp_reconnect();
		if (result != LWFTP_RESULT_OK)
		{
			OSA_TimeDelay(1000);
			continue;
		}
		// Step 2: Change to directory, check if it exists?
		result = Network_LWFTP_CWD(dirPath);
		if (result == LWFTP_RESULT_OK) {
			PRINTF("Change to directory %s\r\n", dirPath);
		} else {
			// directory not exist create directory
			result = Network_LWFTP_MKD(dirPath);
			if (result == LWFTP_RESULT_OK) {
				result = Network_LWFTP_CWD(dirPath);
			}
			if (result != LWFTP_RESULT_OK) {
				lwftp_connected = false;
				continue;
			}

		}

		// Step 3: Connected, try to send file to server
		// Getting the PASV port
		memset(request_msg, 0x00, sizeof(request_msg));
		strcpy(request_msg, "PASV\r\n");
		write(socket_ctrl, request_msg, strlen(request_msg));
		memset(reply_msg,0x00,BUFSIZ);
		recv(socket_ctrl, reply_msg, BUFSIZ, 0);
		resp_code = strtoul(reply_msg, NULL, 10);
		PRINTF("reply_msg:  %s resp_code = %d\r\n", reply_msg, resp_code);
		// Find pasv port
		ptr = strchr(reply_msg, '(');
		do {
			a = strtoul(ptr+1,&ptr,10);
			b = strtoul(ptr+1,&ptr,10);
			c = strtoul(ptr+1,&ptr,10);
			d = strtoul(ptr+1,&ptr,10);
			e = strtoul(ptr+1,&ptr,10);
			f = strtoul(ptr+1,&ptr,10);
		} while(0);
		data_port = e*256+f;
		PRINTF("pasv port =%d\r\n", data_port);
		lwftp_state = LWFTP_PASV_SENT;
		// connect to data port connection
		socket_dat = socket(AF_INET, SOCK_STREAM, 0);
		setsockopt(socket_dat, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
		if (socket_dat == -1)
		{
			perror("Could not create socket");
			return LWFTP_RESULT_ERR_INTERNAL;
		}
		server_dat.sin_addr.s_addr = inet_addr(lwftp_ip);
		server_dat.sin_family = AF_INET;
		server_dat.sin_port = htons(data_port);
		// Connect to server for data
		if (connect(socket_dat, (struct sockaddr *)&server_dat, sizeof(server_dat)) < 0)
		{
			perror("Connection data port failed");
			return LWFTP_RESULT_ERR_CONNECT;
		} else {
			PRINTF("Connected data port\r\n");
		}
		// Store a file read from sdcard "STOR ./<fileName>\r\n"
		memset(request_msg, 0x00, sizeof(request_msg));
		strcpy(request_msg, "STOR ");
		strcat(request_msg, "./"); // concatenate with current directory path
		//strcat(request_msg, "");
		strcat(request_msg, fileName);
		strcat(request_msg, "\r\n");
	//	char cEOF = 0xFF;
		PRINTF("Send to socket_ctrl: %s\r\n", request_msg);
		write(socket_ctrl, request_msg, strlen(request_msg));
		// Write data to file in server
#ifdef ENABLE_FTP_FILE_TEST
		fr = f_open(&fil, ENABLE_FTP_FILE_TEST, FA_READ);
#else
		fr = f_open(&fil, fileName, FA_READ);
#endif
		btr = LWFTP_CONST_LINE_SIZE;
		if (fr == FR_OK) {
			do {
				// read from file file to buffer reply_msg
				fr = f_read(&fil, data_buf, btr, &br);
				if (fr != FR_OK) {
					PRINTF("LWFTP read file failed\r\n");
					return LWFTP_RESULT_ERR_LOCAL;
				}
				if (br > 0) {
					PRINTF("Write to socket data %d bytes\r\n", br);
					len = write(socket_dat, data_buf, br);
				}
			}
			while (br == btr); // while not reached the EOF
		} else {
			PRINTF("LWFTP open file failed\r\n");
			return LWFTP_RESULT_ERR_LOCAL;
		}
		// close the file
		f_close(&fil);
		lwftp_state = LWFTP_CLOSING;
		// close data socket
		close(socket_dat);
		// receive message from ctrl socket
		do {
			max_receive_byte -= resp_len;
			resp_len = recv(socket_ctrl, reply_msg, max_receive_byte, 0);
			if (resp_len > 0)
				PRINTF("reply_msg (len = %d):  %s\r\n",resp_len, reply_msg);
			if (reply_msg > 0 ) {
				response = strtoul(reply_msg, NULL, 10);
				if (response > 0) {
					if (response==150) {
						ret = LWFTP_RESULT_OK;
						PRINTF(("Send file to FTP Successfully\r\n"));
					}
				}
			}
			// process message and check for response
		} while (resp_len > 0 && resp_len < lwftp_bufsize);
		//lwftp_receive_ctrl_data(socket_ctrl, reply_msg, lwftp_bufsize);

		PRINTF("Goodbye\r\n");
		lwftp_state = LWFTP_CLOSED;
		Network_LWFTP_Disconnect();
		return ret;
	}
}
lwftp_result_t Network_LWFTP_Delete(const char *path)
{
	if (!Network_LWIP_Is_Up()) {
		PRINTF("Network_LWFTP_Delete Failed because ethernet down or dhcp is not bound \r\n");
		return LWFTP_RESULT_ERR_CONNECT;
	}
	PRINTF("==========Network_LWFTP_Delete: %s\r\n=========", path);
	lwftp_result_t result = LWFTP_RESULT_ERR_FILENAME;
	int resp_len = 0, response;
	int max_receive_byte = lwftp_bufsize;
	int iTries = 0;
	//lwftp_result_t ret;
	// Check for connection and reconnect in case
	lwftp_reconnect();

	if (lwftp_connected == true)
	{
		// Store a file read from sdcard
		memset(request_msg, 0x00, sizeof(request_msg));
		strcpy(request_msg, "DELE ");
		strcat(request_msg, path);
		strcat(request_msg, "\r\n");
		PRINTF("Delete file cmd: %s\r\n", request_msg);
		write(socket_ctrl, request_msg, strlen(request_msg));
		result = LWFTP_RESULT_ERR_FILENAME;
		do {
			max_receive_byte -= resp_len;
			resp_len = recv(socket_ctrl, reply_msg, max_receive_byte, 0);
			if (resp_len > 0)
				PRINTF("reply_msg (len = %d):  %s\r\n",resp_len, reply_msg);
			if (reply_msg > 0 ) {
				response = strtoul(reply_msg, NULL, 10);
				if (response > 0) {
					if (response==250) {
						result = LWFTP_RESULT_OK;
					}
				}
			}
			// process message and check for response
		} while (resp_len > 0 && resp_len < lwftp_bufsize);
	}
	return result;
}
lwftp_result_t Network_LWFTP_Disconnect()
{
	close(socket_ctrl);
	socket_ctrl = NULL;
	lwftp_connected = false;
	return LWFTP_RESULT_OK;
}




char *Netwrok_LWIP_Get_ServerIP()
{
	return lwftp_ip;
}
lwftp_state_t Network_LWIP_Get_State()
{
	return lwftp_state;
}

/**
 * Create directory in the server
 * @param dirpath directory path
 */
lwftp_result_t Network_LWFTP_MKD(const char* dirpath)
{
	PRINTF("===================Network_LWFTP_MKD: %s==================\r\n", dirpath);
	if (!Network_LWIP_Is_Up()) {
		PRINTF("Network_LWFTP_MKD Failed because ethernet down \r\n");
		return LWFTP_RESULT_ERR_CONNECT;
	}
	lwftp_result_t result = LWFTP_RESULT_ERR_FILENAME;
	char full_path[256];
	int resp_len = 0, response;
	int max_receive_byte = lwftp_bufsize;
	char** tokens;
	bool isMKDRecursiveOK = false;
	bool isNotOK = false;  // in case something wrong in between, to free all tokens buffer alloc dinamically
	int i;
	// Check for connection and reconnect in case
	lwftp_reconnect();
	if (lwftp_connected == true) {
		// Try to make dir with full path first
		memset(request_msg, 0x00, sizeof(request_msg));
		strcpy(request_msg, "MKD ");
		strcat(request_msg, dirpath);
		strcat(request_msg, "\r\n");
		PRINTF("Send mkdir cmd: %s\r\n", request_msg);
		write(socket_ctrl, request_msg, strlen(request_msg));
		result = LWFTP_RESULT_ERR_FILENAME;
		do {
			max_receive_byte -= resp_len;
			resp_len = recv(socket_ctrl, reply_msg, max_receive_byte, 0);
			if (resp_len > 0)
				PRINTF("reply_msg (len = %d):  %s\r\n",resp_len, reply_msg);
			if (reply_msg > 0 ) {
				response = strtoul(reply_msg, NULL, 10);
				if (response > 0) {
					if (response==257) {
						result = LWFTP_RESULT_OK;
					}
				}
			}
			// process message and check for response
		} while (resp_len > 0 && resp_len < lwftp_bufsize);
		if (result == LWFTP_RESULT_OK) {
			result == Network_LWFTP_CWD(dirpath);
			if (result == LWFTP_RESULT_OK)
				return result;
		}
	}
	if (dirpath[0] == '/') {
		Network_LWFTP_CWD("/");
		tokens= str_split(&dirpath[1], '/');
	} else {
		tokens= str_split(dirpath, '/');
	}
	if (lwftp_connected == true)
	{
		if (tokens)
		{
			for (i = 0; *(tokens + i); i++)
			{
				if (isNotOK == true) {

					free(*(tokens + i));
					continue;
				}
				PRINTF("DIR=[%s]\r\n", *(tokens + i));
				// Change to directory and check if exists or not
				result = Network_LWFTP_CWD(*(tokens + i));
				if (result == LWFTP_RESULT_OK) {
					free(*(tokens + i));
					continue;
				}
				// If not exists try to create new directory
				memset(request_msg, 0x00, sizeof(request_msg));
				strcpy(request_msg, "MKD ");
				strcat(request_msg, *(tokens + i));
				strcat(request_msg, "\r\n");
				PRINTF("Send mkdir cmd: %s\r\n", request_msg);
				write(socket_ctrl, request_msg, strlen(request_msg));
				result = LWFTP_RESULT_ERR_FILENAME;
				do {
					max_receive_byte -= resp_len;
					resp_len = recv(socket_ctrl, reply_msg, max_receive_byte, 0);
					if (resp_len > 0)
						PRINTF("reply_msg (len = %d):  %s\r\n",resp_len, reply_msg);
					if (reply_msg > 0 ) {
						response = strtoul(reply_msg, NULL, 10);
						if (response > 0) {
							if (response==257) {
								result = LWFTP_RESULT_OK;
							}
						}
					}
					// process message and check for response
				} while (resp_len > 0 && resp_len < lwftp_bufsize);
				result = Network_LWFTP_CWD(*(tokens + i));
				free(*(tokens + i));
				if (result != LWFTP_RESULT_OK) {
					free(*(tokens + i));
					isNotOK = true;
				}
			}
			printf("\n");
			free(tokens);
		}

	}
	return result;
}


/**
 * Change current directory in ftp server to dirpath
 * @param dirpath
 * @return result
 */
lwftp_result_t Network_LWFTP_CWD(const char* dirpath)
{
	if (!Network_LWIP_Is_Up()) {
		PRINTF("Network_LWFTP_Delete Failed because ethernet down or dhcp is not bound \r\n");
		return LWFTP_RESULT_ERR_CONNECT;
	}
	PRINTF("==========Network_LWFTP_CWD: %s\r\n=========", dirpath);
	lwftp_result_t result = LWFTP_RESULT_ERR_FILENAME;
	int resp_len = 0, response;
	int max_receive_byte = lwftp_bufsize;
	//int iTries = 0;
	//lwftp_result_t ret;
	// Check for connection and reconnect in case
	lwftp_reconnect();

	if (lwftp_connected == true)
	{
		// Store a file read from sdcard
		memset(request_msg, 0x00, sizeof(request_msg));
		strcpy(request_msg, "CWD ");
		strcat(request_msg, dirpath);
		strcat(request_msg, "\r\n");
		PRINTF("Change to directory %s\r\n", request_msg);
		write(socket_ctrl, request_msg, strlen(request_msg));
		result = LWFTP_RESULT_ERR_FILENAME;
		do {
			max_receive_byte -= resp_len;
			resp_len = recv(socket_ctrl, reply_msg, max_receive_byte, 0);
			if (resp_len > 0)
				PRINTF("reply_msg (len = %d):  %s\r\n",resp_len, reply_msg);
			if (reply_msg > 0 ) {
				response = strtoul(reply_msg, NULL, 10);
				if (response > 0) {
					if (response==250) {
						result = LWFTP_RESULT_OK;
					} else if (response == 550) {
						//result = LWFTP_RESULT_ERR_FILENAME;
					}
				}
			}
			// process message and check for response
		} while (resp_len > 0 && resp_len < lwftp_bufsize);
	}
	return result;
}
