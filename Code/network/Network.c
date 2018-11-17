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

SNetStt g_Network_Status;

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
	if (ret1)
		return true;
	else return false;
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
	lwtcp_result_t ret1 = Network_LWFTP_Start(ip, port, usrname, passwd);
		if (ret1 != LWTCP_RESULT_OK)
			return NET_ERR_LWIP_FTPCLIENT;
	return NET_ERR_NONE;
}
/**
 * @brief If connected then send file.
 * If file send failed retry 3 times
 *
 * @param dirPath
 * @param fileName
 * @return NetStatus
 */
NetStatus Net_FTPClientSendFile(const char *dirPath, const char *fileName) {
	lwftp_result_t ret1;
	if (g_Network_Status.NetIF == NET_IF_ETHERNET) {
		ret1 = Network_LWFTP_SendFile(dirPath, fileName);
		if (ret1 == LWFTP_RESULT_OK) {
			return NET_ERR_NONE;
		}
	}
	return NET_ERR_UNKNOWN;
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
