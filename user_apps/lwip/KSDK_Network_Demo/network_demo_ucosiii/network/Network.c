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
/**
 * @brief Khoi tao module SIM + Ethernet, Enable DHCP
 *
 * @return int
 */
int Net_ModuleInitHw() {
	Network_LWIP_TCP_Init();
	Network_LWIP_DHCP_Init();
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
