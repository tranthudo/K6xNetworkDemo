#include "Network_LWTCP.h"
static int parentfd; /* parent socket */
//static int childfd; /* child socket */
//static int portno; /* port to listen on */
static int clientlen; /* byte size of client's address */
static struct sockaddr_in serveraddr; /* server's addr */
static struct sockaddr_in clientaddr; /* client addr */
//static struct hostent *hostp; /* client host info */
//static char buf[BUFSIZE]; /* message buffer */
//static char *hostaddrp; /* dotted decimal host addr string */
static int optval; /* flag value for setsockopt */
//static int n; /* message byte size */
static bool tcp_server_running = false;
static int number_of_connected_clients = 0;
// client callback
static ClientThread clientCallback=NULL;

static task_handler_t taskHandler[LWTCP_SERVER_MAX_CLIENT];
static task_stack_t *stackMem[LWTCP_SERVER_MAX_CLIENT] ;
static bool clientThreadActivate[LWTCP_SERVER_MAX_CLIENT];
static int childfds[LWTCP_SERVER_MAX_CLIENT];

mutex_t tcp_Mutex;
osa_status_t status;
int client_stack_size = 1024;


lwtcp_result_t Network_LWTCP_Set_Callback(ClientThread fn)
{
	clientCallback = fn;
}

/*
 * Starts a new thread with priority "prio" that will begin its execution in the
 * function "thread()". The "arg" argument will be passed as an argument to the
 * thread() function. The argument "ssize" is the requested stack size for the
 * new thread. The id of the new thread is returned. Both the id and the
 * priority are system dependent.
 */

static sys_thread_t lwtcp_create_thread(task_handler_t taskHandler, task_stack_t* stackMem, const char *name, lwip_thread_fn thread, void *arg, int stacksize, int prio)
{
    int fd = *((int*)(arg));
    PRINTF("fd = %d\r\n", fd);
    osa_status_t error = OSA_TaskCreate((task_t)thread ,(uint8_t*) name,(uint16_t) stacksize, stackMem,prio,(task_param_t)arg,false,&taskHandler);
    if(error == kStatus_OSA_Success)
        return taskHandler;
    else
        return (sys_thread_t)0;
}

void tcp_connected_client_thread(void* arg) {
	OS_ERR err;
	osa_status_t status;

	Tcp_Client_Arg* tcp_arg = (Tcp_Client_Arg*)arg;
//	/int fd = tcp_arg->fd;
	int idx = tcp_arg->idx; // idx will not change but childfd will be changed
	PRINTF("\r\n Start tcp_connected_client_thread fd=%d, idx=%d \r\n", childfds[idx], idx);
	while (true) {
		//PRINTF("\r\n client_thread[%d] call clientCallbac(%d) \r\n", idx, childfds[idx]);
		if (clientThreadActivate[idx]) {
			if (clientCallback != NULL) {
				clientCallback(childfds[idx]);
			} else {
				PRINTF("clientCallback not exist\r\n");
			}
			OSA_MutexLock(&tcp_Mutex, OSA_WAIT_FOREVER);
			if (number_of_connected_clients > 0)
				number_of_connected_clients -= 1;
			clientThreadActivate[idx] = false;
			if (childfds[idx] > 0)
				close(childfds[idx]);
			PRINTF("number_of_connected_clients changed to %d\r\n", number_of_connected_clients);
			OSA_MutexUnlock(&tcp_Mutex);
			PRINTF("\r\n client_thread[%d] call clientCallbac(%d) \r\n", idx, childfds[idx]);

		} else {
			PRINTF("\r\n client_thread[%d] is not active \r\n", idx);
			//OSA_TimeDelay(2000);
		}
		// Close socket
		OSA_TimeDelay(2000);
	}
	//PRINTF("MemFree stackMem %d\r\n", stackMem);
	/* Task body ... do work! */
//	if (tcp_arg->stackMem) {
//		PRINTF("MemFree stackMem %d\r\n", tcp_arg->stackMem);
//		status = OSA_MemFree((void*)tcp_arg->stackMem);
//		if (status == kStatus_OSA_Success) {
//			PRINTF("OSA_Memfree OK\r\n");
//		} else {
//			PRINTF("OSA_Memfree NOK\r\n");
//		}
//	}
//	if (stackMem) {
//		PRINTF("MemFree stackMem %d\r\n", tcp_arg->stackMem);
//		status = OSA_MemFree((void*)stackMem);
//		if (status == kStatus_OSA_Success) {
//			PRINTF("OSA_Memfree OK\r\n");
//		} else {
//			PRINTF("OSA_Memfree NOK\r\n");
//		}
//	}
//	if (tcp_arg->taskHandler) {
//		PRINTF("OSA_TaskDestroy taskHandler %d, %s\r\n", tcp_arg->taskHandler);
//		status =  OSA_MemFree((void*)tcp_arg->taskHandler);
//		//status = OSA_TaskDestroy(tcp_arg->taskHandler);
//		if (status == kStatus_OSA_Success) {
//			PRINTF("OSA_TaskDestroy OK\r\n");
//		} else {
//			PRINTF("OSA_TaskDestroy NOK\r\n");
//		}
//	}
//	OSTaskDel(0, &err);
//	if (tcp_arg->taskHandler) {
//		PRINTF("OSA_TaskDestroy taskHandler %d, %s\r\n", tcp_arg->taskHandler);
//		//status =  OSA_MemFree((void*)tcp_arg->taskHandler);
//		status = OSA_TaskDestroy(tcp_arg->taskHandler);
//		if (status == kStatus_OSA_Success) {
//			PRINTF("OSA_TaskDestroy OK\r\n");
//		} else {
//			PRINTF("OSA_TaskDestroy NOK\r\n");
//		}
//	}

	//OSA_TaskDestroy(tcp_arg->taskHandler);
}

static void tcp_server_thread(void *arg)
{
	OS_ERR err;
	sys_thread_t threadID;
	Tcp_Client_Arg tcp_arg[2];
	int childfd;
	int idx = 0, i;
	struct timeval tv;
	//bool threadClientAvailable = false;
	/* Local variables */
	// These task handler & stackmem are allocated for tcp_client_thread created later
    static task_handler_t taskHandler[LWTCP_SERVER_MAX_CLIENT];
    osa_status_t error;
    static task_stack_t *stackMem[LWTCP_SERVER_MAX_CLIENT] ;

    for (i = 0; i < LWTCP_SERVER_MAX_CLIENT; i++) {
    	char tmpStr[128];
    	taskHandler[i] = (task_handler_t)OSA_MemAlloc(sizeof(OS_TCB));
    	if(!taskHandler[i])
    	{
    		PRINTF("OSA_MemAlloc taskHandler[%d] failed\r\n", i);
    		return;
    	} else {
    		PRINTF("OSA_MemAlloc taskHandler[%d] OK = %d\r\n", i, taskHandler);
    	}

    	stackMem[i] =  (task_stack_t *)OSA_MemAlloc((size_t)1024);
		if(!stackMem[i])
		{
			PRINTF("OSA_MemAlloc stackMem[%d] failed\r\n", i);
			return;
		} else {
			PRINTF("OSA_MemAlloc stackMem[%d] OK = %d\r\n", i, stackMem);
		}
		clientThreadActivate[i] = false;
		tcp_arg[i].idx = i;
		sprintf(tmpStr,"Client Thread %d\r\n", tcp_arg[i].idx);
		PRINTF(tmpStr);
		threadID = lwtcp_create_thread(taskHandler[i], stackMem[i],
				tmpStr, tcp_connected_client_thread, &tcp_arg[i], client_stack_size, 4);
    }
	/* Do something with ‘p_arg’ */
	/* Task initialization */

	PRINTF("Start TCP SERVER THREAD\r\n");
	status = OSA_MutexCreate(&tcp_Mutex);
	if (status != kStatus_OSA_Success) {
		PRINTF("tcp_Mutex creation failed\r\n");
	}

	tcp_server_running = true;
	tv.tv_sec = 3000; //dkm co gi do sai sai o day khi dung lwip 3000s ma nhu la 3s
	tv.tv_usec = 10000;
	// set timeout of socket

	while (tcp_server_running == true) {
		//PRINTF("TCP_SERVER is RUNING\r\n");
		OSA_TimeDelay(300);
		//threadClientAvailable = false;
		OSA_MutexLock(&tcp_Mutex, OSA_WAIT_FOREVER);
		if (number_of_connected_clients < LWTCP_SERVER_MAX_CLIENT) {
			// Check which IDX to be use
			for (idx = 0; idx < LWTCP_SERVER_MAX_CLIENT; idx++) {
				if (clientThreadActivate[idx] == false) {
					break;
				}
			}
			OSA_MutexUnlock(&tcp_Mutex);
			if (idx < LWTCP_SERVER_MAX_CLIENT) {
				// Wait for new connection and for new thread for new client connection
				/*
				 * accept: wait for a connection request
				 */
				childfd = accept(parentfd, (struct sockaddr *) &clientaddr, &clientlen);
				setsockopt(childfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
				if (childfd < 0) {
					//perror("ERROR on accept");
				}
				else {
					PRINTF("Client childfd =  %d\r\n", childfd);
					OSA_MutexLock(&tcp_Mutex, OSA_WAIT_FOREVER);
					clientThreadActivate[idx] = true;
					childfds[idx] = childfd;
					number_of_connected_clients += 1;
					OSA_MutexUnlock(&tcp_Mutex);
					PRINTF("number_of_connected_clients changed to %d\r\n", number_of_connected_clients);
				}
			} else {
				PRINTF("idx(%d) < LWTCP_SERVER_MAX_CLIENT(%d)\r\n", idx, LWTCP_SERVER_MAX_CLIENT);
			}
		} else {
			OSA_MutexUnlock(&tcp_Mutex);
		}

	}
	/* Task body ... do work! */
	OSTaskDel((OS_TCB *)0, &err);
}

lwtcp_result_t Network_LWTCPServer_SetClientStackSize(int _sz)
{
	client_stack_size = _sz;
}
lwtcp_result_t Network_LWTCPServer_Start(int portno) {
	// Do not allow create multiple TCP Server
	if (tcp_server_running == true)
		return LWTCP_RESULT_OK;
	sys_thread_t threadID;
	lwtcp_result_t ret = LWTCP_RESULT_OK;
	/*
	 * socket: create the parent socket
	 */
	parentfd = socket(AF_INET, SOCK_STREAM, 0);
	if (parentfd < 0) {
		perror("ERROR opening socket");
		ret = LWTCP_RESULT_ERR_OPENING;
	}
	/* setsockopt: Handy debugging trick that lets
	 * us rerun the server immediately after we kill it;
	 * otherwise we have to wait about 20 secs.
	 * Eliminates "ERROR on binding: Address already in use" error.
	 */
	optval = 1;
	setsockopt(parentfd, SOL_SOCKET, SO_REUSEADDR,
			(const void *)&optval , sizeof(int));

	/*
	 * build the server's Internet address
	 */
	bzero((char *) &serveraddr, sizeof(serveraddr));

	/* this is an Internet address */
	serveraddr.sin_family = AF_INET;

	/* let the system figure out our IP address */
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

	/* this is the port we will listen on */
	serveraddr.sin_port = htons((unsigned short)portno);

	/*
	 * bind: associate the parent socket with a port
	 */
	if (bind(parentfd, (struct sockaddr *) &serveraddr,
			sizeof(serveraddr)) < 0) {
		ret = LWTCP_RESULT_ERR_OPENING;
		perror("ERROR on binding");
	}

	/*
	 * listen: make this socket ready to accept connection requests
	 */
	if (listen(parentfd, LWTCP_SERVER_MAX_CLIENT) < 0) /* allow 2 requests to queue up */ {
		perror("ERROR on listen");
		ret = LWTCP_RESULT_ERR_LISTENING;
	}
	PRINTF("TCPServer Start Result = %d\r\n", ret);
	if (ret == LWTCP_RESULT_OK)
	{
		threadID = sys_thread_new("tcp_server_thread", tcp_server_thread, NULL, 2048, 4);
		if (threadID == NULL) {
			PRINTF("Server thread Created failed, ID = %d\r\n", threadID);
			ret = LWTCP_RESULT_ERR_UNKNOWN;
		} else {
			PRINTF("Server thread created OK, ID = %d\r\n", threadID);
		}
	}
	if (ret == LWTCP_RESULT_OK) {
		tcp_server_running = true;
	}
	return ret;
}
/**
 * Send to client
 * @param clientSocketfd
 * @param data
 * @param length
 * @return
 */
int Network_LWTCP_Send(int clientSocketfd, const uint8_t *data, uint32_t length)
{
	return write(clientSocketfd, data, length);
}
/**
 * Receive from socket a number of bytes
 * @param clientSocketfd socket to receive
 * @param buf buffer to receive
 * @param max_received_bytes max number of bytes to receive
 * @return
 */
int Network_LWTCP_Receive(int clientSocketfd, uint8_t *buf, uint32_t max_received_bytes) {
	return recv(clientSocketfd, buf, max_received_bytes, 0);
}
