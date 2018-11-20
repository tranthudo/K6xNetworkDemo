/**
 * @file Network.h
 * @author ThinhNT (tienthinh@gmail.com)
 * @brief Module Network 
 * @version 0.1
 * @date 2018-10-17
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#include "NetCommon.h"
#include "Network_LWIP.h"
#include "Network_LWFTP.h"
#include "Network_LWTCP.h"
#include "Network_LWFTP_Test.h"
/**
 * Split a string
 * @param a_str pointer to string
 * @param a_delim delimeter to split
 * @return
 */
char** str_split(char* a_str, const char a_delim);
/**
 * @brief Khoi tao module SIM + Ethernet, Enable DHCP
 * 
 * @return int 
 */
int Net_ModuleInitHw();
/**
 * @brief Start 2 TCP Server SIMCOM + LWIP 
 * Allow maximum 5 connection
 * @param port 
 * @return NetStatus 
 */
NetStatus Net_TCPServerStart(int port);
/**
 * Set the client callback for each client connect
 * @param fn the soket id
 * @return
 */
NetStatus Net_TCPServerSetCallback(ClientThread fn);
/**
 * Start and TCP client
 * @param ip
 * @param port
 * @return
 */
NetStatus Net_TCPClientStart(const char* ip, int port);
/**
 * @brief Return Netstatus of IF & connection status
 * 
 * @return SNetStt 
 */
SNetStt Net_ModuleGetStatus();
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
NetStatus Net_FTPClientStart(const char *ip, int port, const char* usrname, const char* passwd);
/**
 * @brief If connected then send file.
 * If file send failed retry 3 times
 * 
 * @param dirPath 
 * @param fileName 
 * @return NetStatus 
 */
NetStatus Net_FTPClientSendFile(const char *dirPath, const char *fileName);
/**
 * @brief Delete file in FTP Server
 *
 * @param dirPath
 * @param fileName
 * @return NetStatus
 */
NetStatus Net_FTPClientDeleteFile(const char *path);
/**
 *
 * @return NET_ERR_NONE if Ethernet is up or SIMCOM is up
 */
bool Net_Is_Up();

/**
 * @brief Send data to server 
 * 
 * @param data 
 * @param length 
 * @return NetStatus 
 */
NetStatus Net_TCPClientSendData(const uint8_t *data, uint32_t length);
/**
 * @brief Send data to all client
 * 
 * @param data 
 * @param length 
 * @return NetStatus 
 */
NetStatus Net_TCPServerSendDataToAllClient(const uint8_t *data, uint32_t length);
