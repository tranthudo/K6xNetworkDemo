/*
 * modem_ftp_client.c
 *
 *  Created on: Oct 26, 2018
 *      Author: buiti
 */

#include "modem_ftp_client.h"

#include <debug.h>
#include <string.h>
#include "modem_ftp_command.h"
#include "modem_debug.h"

static modem_ftp_client_handle_t g_modem_ftp_client;

//modem_ftp_err_code_t modem_ftp_init(modem_ftp_client_handle_t *pHandle)
modem_ftp_err_code_t modem_ftp_init(char* server, int port,  char* username, char* password)
{
	modem_ftp_err_code_t ret = FTP_ERR_UNKNOWN;

	modem_ftp_client_handle_t *pHandle = &g_modem_ftp_client;

	if(!pHandle)
	{
		PRINTF("FTP client handle is NULL");
		return ret;
	}


	memset(pHandle->serveraddress, 0, sizeof(pHandle->serveraddress));
	memset(pHandle->username, 0, sizeof(pHandle->username));
	memset(pHandle->password, 0, sizeof(pHandle->password));

	memcpy(pHandle->serveraddress, server, strlen(server));
	memcpy(pHandle->username, username, strlen(username));
	memcpy(pHandle->password, password, strlen(password));
	pHandle->port = port;
	pHandle->state = FTP_STATE_IDLE;
	pHandle->busy = 0;
	pHandle->ssl_enable = 0;

	/* Configure modem  */
	modem_send_at_command("ATE1\r\n", "OK", 1000, 1);
	modem_delay_ms(1000);
	modem_tx_data("+++", 3);
	modem_delay_ms(1000);

	modem_send_at_command("AT+QIACT=0\r\n", "OK", 1000, 1);

	modem_send_at_command("AT+QICSGP=1,1,\"\",\"\",\"\",1\r\n","OK", 1000, 3);
//		AT+QIACT=1
	modem_send_at_command("AT+QIACT=1\r\n", "OK", 1000, 3);
	modem_send_at_command("AT+QIACT?\r\n", "OK",  1000, 1);
	modem_send_at_command("AT+QFTPCFG=\"contextid\",1\r\n", "OK", 500, 1);
	modem_send_at_command("AT+QFTPCFG=\"filetype\",1\r\n", "OK", 500, 1);
	modem_send_at_command("AT+QFTPCFG=\"transmode\",1\r\n", "OK",500, 1);
	modem_send_at_command("AT+QFTPCFG=\"rsptimeout\",60\r\n", "OK", 500, 1);
	return ret;
}

modem_ftp_err_code_t modem_ftp_connect()
{

	char cmd[128] = {0};

	memset (cmd, 0, sizeof(cmd));
	sprintf(cmd, "AT+QFTPCFG=\"account\",\"%s\",\"%s\"\r\n", g_modem_ftp_client.username, g_modem_ftp_client.password);
	modem_send_at_command(cmd, "OK", 500, 1);

	memset (cmd, 0, sizeof(cmd));
	sprintf(cmd, "AT+QFTPOPEN=\"%s\",%d\r\n", g_modem_ftp_client.serveraddress, g_modem_ftp_client.port);
	if (modem_send_at_command(cmd, "+QFTPOPEN: 0,0", 5000, 2) == TRUE)
		return FTP_ERR_OK;
	else
		return FTP_ERR_FTP_TIMEOUT;
}

modem_ftp_err_code_t modem_ftp_change_working_dir(char* working_dir)
{
	char cmd[128] = {0};

	memset (cmd, 0, sizeof(cmd));

	sprintf(cmd, "AT+QFTPCWD=\"%s\"\r\n", working_dir);
	if (modem_send_at_command(cmd, "+QFTPCWD: 0,0", 5000, 3) == TRUE)
		return FTP_ERR_OK;
	else
		return FTP_ERR_FTP_TIMEOUT;
}

modem_ftp_err_code_t modem_ftp_disconnect()
{
//	AT+QFTPCLOSE //Logout from FTP server.
//	OK
//	+QFTPCLOSE: 0,0
//	AT+QIDEACT=1

	char cmd[128] = {0};

	memset (cmd, 0, sizeof(cmd));
	sprintf(cmd, "AT+QFTPCLOSE\r\n");
//	if(modem_send_at_command(cmd, "+QFTPCLOSE: 0,0", 1000, 3) == FALSE)
	if(modem_send_at_command(cmd, "+QFTPCLOSE:", 5000, 3) == FALSE)
		return FTP_ERR_FTP_TIMEOUT;

	return FTP_ERR_OK;

	memset (cmd, 0, sizeof(cmd));
	sprintf(cmd, "AT+QIDEACT=1\r\n");
	if (modem_send_at_command(cmd, "OK", 5000, 3) == TRUE)
		return FTP_ERR_OK;
	else
		return FTP_ERR_FTP_TIMEOUT;

}


modem_ftp_err_code_t modem_ftp_get_list_files(char* path, char* filelist, uint32_t timeout)
{
	modem_ftp_err_code_t ret = FTP_ERR_UNKNOWN;
	modem_ftp_client_handle_t *pHandle = &g_modem_ftp_client;
//	char cmd[256] = {0};
//	char response[4096] = {0};
//	int recv_len = 0;
//	//get list files
//	recv_len = 0;
//	memset(cmd, 0, sizeof(cmd));
//	memset(response, 0, sizeof(response));
//
//	sprintf(cmd, AT_CMD_SET_FTP_GETLIST, (const char*)path);
//	LREP_WARNING("sending command: %s", cmd);
//	ret = exlib_platform_serial_write_read_response(&pHandle->pSerialPort, cmd, strlen(cmd), response, &recv_len, timeout, "+CFTPLIST: 0");
//
//	if((recv_len > 0) && (strstr(response, "OK") != NULL))
//	{
//		LREP("SIMCOM FTP client AT_CMD_SET_FTP_GETLIST OK!");
//		LREP_DUMP(response, recv_len, "Received data");
//		ret = FTP_ERR_OK;
//	}
//	else
//	{
//		LREP_WARNING("SIMCOM FTP CLient init [AT_CMD_SET_FTP_GETLIST] failed, recv %d bytes: %s", recv_len, response);
//		ret = FTP_ERR_FTP_TIMEOUT;
//	}
//
//

	return ret;
}
modem_ftp_err_code_t modem_ftp_get_list(char* path, SList* psFileList, uint32_t timeout)
{
	modem_ftp_err_code_t ret = FTP_ERR_UNKNOWN;
	modem_ftp_client_handle_t *pHandle = &g_modem_ftp_client;

//	char cmd[256] = {0};
//	char response[4096] = {0};
//	int recv_len = 0;
//	//get list files
//	recv_len = 0;
//	memset(cmd, 0, sizeof(cmd));
//	memset(response, 0, sizeof(response));
//
//	sprintf(cmd, AT_CMD_SET_FTP_GETLIST, (const char*)path);
//	LREP_WARNING("sending command: %s", cmd);
//	ret = exlib_platform_serial_write_read_response(&pHandle->pSerialPort, cmd, strlen(cmd), response, &recv_len, timeout, "+CFTPLIST: 0");
//
//	if((recv_len > 0) && (strstr(response, "OK") != NULL))
//	{
//		LREP("SIMCOM FTP client AT_CMD_SET_FTP_GETLIST OK!");
//		LREP_DUMP(response, recv_len, "Received data");
//
//		//TODO: process response data
//		ListIsEmpty(psFileList);
//		modem_ftp_file_attr_t attr;
//		for (int i = 0; i < recv_len; i ++)
//		{
//			// TODO: parse data to get file property
////			ListAddTail( SList *psList, void* pvNewElement );
//
//			ListAddTail(psFileList, (void*)&attr);
//		}
//		ret = FTP_ERR_OK;
//	}
//	else
//	{
//		LREP_WARNING("SIMCOM FTP CLient init [AT_CMD_SET_FTP_GETLIST] failed, recv %d bytes: %s", recv_len, response);
//		ret = FTP_ERR_FTP_TIMEOUT;
//	}
//
//

	return ret;
}

modem_ftp_err_code_t modem_ftp_get_file(char* filename, uint8_t* content, uint32_t* len, uint32_t timeout)
{
	modem_ftp_err_code_t ret = FTP_ERR_NOT_IMPLEMENT;
	modem_ftp_client_handle_t *pHandle = &g_modem_ftp_client;

//	char cmd[256] = {0};
//	char response[4096] = {0};
//	int recv_len = 0;
//	//get list files
//	recv_len = 0;
//	memset(cmd, 0, sizeof(cmd));
//	memset(response, 0, sizeof(response));
//
//	sprintf(cmd, AT_CMD_SET_FTP_GETLIST, (const char*)path);
//	LREP_WARNING("sending command: %s", cmd);
//	ret = exlib_platform_serial_write_read_response(&pHandle->pSerialPort, cmd, strlen(cmd), response, &recv_len, timeout, "+CFTPLIST: 0");
//
//	if((recv_len > 0) && (strstr(response, "OK") != NULL))
//	{
//		LREP("SIMCOM FTP client AT_CMD_SET_FTP_GETLIST OK!");
//		ret = FTP_ERR_OK;
//	}
//	else
//	{
//		LREP_WARNING("SIMCOM FTP CLient init [AT_CMD_SET_FTP_GETLIST] failed, recv %d bytes: %s", recv_len, response);
//		ret = FTP_ERR_FTP_TIMEOUT;
//	}
//
//
//
	return ret;
}


modem_ftp_err_code_t modem_ftp_put_file( char* filename, uint8_t* content, uint32_t len, uint32_t timeout)
{
	modem_ftp_err_code_t ret = FTP_ERR_UNKNOWN;
	modem_ftp_client_handle_t *pHandle = &g_modem_ftp_client;
	if(!pHandle || !filename || !content)
	{
		PRINTF("Param invalid!!!");
		ret = FTP_ERR_INVALID_PARAMS;
		return ret;
	}
	char cmd[128] = {0};

	memset (cmd, 0, sizeof(cmd));
	sprintf(cmd, "AT+QFTPPUT=\"%s\",\"COM:\",0\r\n", filename);

	if(modem_send_at_command(cmd, "CONNECT", 15000, 2) == TRUE)
	{
		MODEM_DEBUG("Begin Putting File");

		modem_tx_data(content, len);

		modem_delay_ms(1000);

		if(modem_send_at_command("+++", "OK", 5000, 2) == TRUE)
			ret = FTP_ERR_OK;

		MODEM_DEBUG("End Putting File");

	}
	else
	{
		ret = FTP_ERR_FTP_TIMEOUT;
	}

//
//	LREP_WARNING("Begin put file %s to FTP server", filename);
//	//TODO: [manhbt] add code to put file content to FTP server
//
//	char cmd[256] = {0};
//	char response[4096] = {0};
//	int recv_len = 0;
//
//	recv_len = 0;
//	memset(cmd, 0, sizeof(cmd));
//	memset(response, 0, sizeof(response));
//
//	sprintf(cmd, AT_CMD_SET_FTP_PUTFILE, (const char*)filename);
//	LREP_WARNING("sending command: %s", cmd);
//
//	int res = exlib_platform_serial_write_read_response(&pHandle->pSerialPort, cmd, strlen(cmd), response, &recv_len, timeout, "+CFTPPUT: BEGIN");
//
//	if(res != 0) // timed out
//	{
//		LREP_ERROR("AT+CFTPPUT command timed out!!!");
//		ret = FTP_ERR_FTP_TIMEOUT;
//		return ret;
//	}
//
//	// TODO: [manhbt] Check more return status code from SIMCOM FTP service
//
//
//	// begin send data
//	res = exlib_platform_serial_write(&pHandle->pSerialPort, content, len);
//	if(res != len)
//	{
//		LREP_WARNING("Send data error!");
//		ret = FTP_ERR_UNKNOWN;
//		return ret;
//	}
//
//	// send <Ctrl-Z> (0x1A) character and get response "OK"
//
//	recv_len = 0;
//	memset(cmd, 0x1A, 1);
//	memset(response, 0, sizeof(response));
//
////	sprintf(cmd, AT_CMD_SET_FTP_PUTFILE, (const char*)path);
//	res = exlib_platform_serial_write_read_response(&pHandle->pSerialPort, cmd, 1, response, &recv_len, timeout, "OK");
//
//	if(res == 0)
//	{
//		LREP("FTP Put file OK!!!");
//		ret = FTP_ERR_OK;
//	}
//
//	LREP_WARNING("Finish put file %s to FTP server with errcode %d", filename, ret);


	return ret;
}



modem_ftp_err_code_t modem_ftp_make_dir(char* dir)
{
	modem_ftp_err_code_t ret = FTP_ERR_UNKNOWN;
	modem_ftp_client_handle_t *pHandle = &g_modem_ftp_client;
	if(!pHandle || !dir)
	{
		PRINTF("Param invalid!!!");
		ret = FTP_ERR_INVALID_PARAMS;
		return ret;
	}


//	AT+QFTPMKDIR="2018/12"
//	OK
//
//	+QFTPMKDIR: 0,0
	char cmd[128] = {0};

	memset (cmd, 0, sizeof(cmd));
	sprintf(cmd, "AT+QFTPMKDIR=\"%s\"\r\n", dir);

	if(modem_send_at_command(cmd, "+QFTPMKDIR: 0,0", 5000, 2) == TRUE)
	{
		PRINTF("Create DIR OK!\r\n");
			ret = FTP_ERR_OK;
	}
	else
	{
		PRINTF("Create DIR FAILED!\r\n");
		ret = FTP_ERR_FTP_TIMEOUT;
	}

	return ret;
}

modem_ftp_err_code_t modem_ftp_remove_dir(char* dir)
{
	modem_ftp_err_code_t ret = FTP_ERR_UNKNOWN;
	modem_ftp_client_handle_t *pHandle = &g_modem_ftp_client;
	if(!pHandle || !dir)
	{
		PRINTF("Param invalid!!!");
		ret = FTP_ERR_INVALID_PARAMS;
		return ret;
	}

	//AT+QFTPRMDIR=<folder_name>
	char cmd[128] = {0};

	memset (cmd, 0, sizeof(cmd));
	sprintf(cmd, "AT+QFTPRMDIR=\"%s\"\r\n", dir);

	if(modem_send_at_command(cmd, "+QFTPRMDIR: 0,0", 5000, 2) == TRUE)
	{
		PRINTF("Remove DIR OK!\r\n");
			ret = FTP_ERR_OK;
	}
	else
	{
		PRINTF("Remove DIR FAILED!\r\n");
		ret = FTP_ERR_FTP_TIMEOUT;
	}

	return ret;
}


modem_ftp_err_code_t modem_ftp_rename(char* old_name, char* new_name)
{
//	AT+QFTPRENAME=<old_name>,<new_name>
	modem_ftp_err_code_t ret = FTP_ERR_UNKNOWN;
	modem_ftp_client_handle_t *pHandle = &g_modem_ftp_client;
	if(!pHandle || !old_name || !new_name)
	{
		PRINTF("Param invalid!!!");
		ret = FTP_ERR_INVALID_PARAMS;
		return ret;
	}

	//AT+QFTPRMDIR=<folder_name>
	char cmd[128] = {0};

	memset (cmd, 0, sizeof(cmd));
	sprintf(cmd, "AT+QFTPRENAME=\"%s\",\"%s\"\r\n", old_name, new_name);

	if(modem_send_at_command(cmd, "+QFTPRENAME: 0,0", 5000, 2) == TRUE)
	{
		PRINTF("Remove DIR OK!\r\n");
			ret = FTP_ERR_OK;
	}
	else
	{
		PRINTF("Remove DIR FAILED!\r\n");
		ret = FTP_ERR_FTP_TIMEOUT;
	}

	return ret;
}

modem_ftp_err_code_t modem_ftp_get_status()
{
	return FTP_ERR_OK;
}

