/**
 * Network Commnucation Configuration
 **/
typedef enum eNetComConfig{
    NET_COM_USING_HW = 1,
    NET_COM_IP_METHOD,
    NET_COM_RECV_BUFF,
    NET_COM_SEND_BUFF,
    NET_COM_FTP_USER,
    NET_COM_FTP_PASSWD
} ENetComConfig;

/**
 * Network Status 
 **/
typedef enum eNetStatus {
    NET_ERR_NONE = 0, 
    NET_ERR_MODULE_NOT_INIT, 
    NET_ERR_PARAM, 
    NET_ERR_SEND_FAILD, 
    NET_ERR_LOST_CONNECTION, 
    NET_ERR_BUSY, 
    NET_ERR_UNKNOWN 
} NetStatus;

/**
 * Net interface status
 **/
typedef enum eNetIF {
    NET_IF_ETHERNET = 1,
    NET_IF_WIRELESS,
    NET_IF_UNKNOWN
} NetIF;

/**
 * Connection status 
 **/
typedef enum eNetConStat {
    NET_CON_CONNECTED,
    NET_CON_DISCONNECTED,
    NET_CON_ERR_UNKNOWN
} NetConStat;

typedef struct sNetStt {
    NetIF NetIF;
    NetConStat NetConStat;
} SNetStt;