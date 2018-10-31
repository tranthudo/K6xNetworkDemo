/*
 *  client.c
 *
 *  Simple FTP Client
 *  ====================
 *  Connects the server, and sends a command "Get FILENAME" to retrieve
 *  the file. If the file exists on the server, client retrieves it.
 *
 *  Compile & link  :   gcc client.c -o client
 *  Execute         :   ./client
 *
 *  Ozcan Ovunc <ozcan_ovunc@hotmail.com>
 */

#include <stdio.h>
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define THINHNTPC


#ifdef THINHNTPC
#define FILENAME 	"a.txt"
#define SERVER_IP 	"192.168.0.101"
#define SERVER_PORT 	21
#define USER_NAME "thinhnt\r\n"
#define PASSWORD "123456a@\r\n"
#define PASV "PASV\r\n"
#else
// Server in Local
#define FILENAME 	"test1.txt"
#define SERVER_IP 	"127.0.0.1"
#define SERVER_PORT 	21
#endif

#define BUFSIZ 1024

int main(int argc , char **argv)
{
	int 	socket_ctrl, socket_dat;
	struct 	sockaddr_in server, server_dat;
	char 	request_msg[BUFSIZ], reply_msg[BUFSIZ];
	char 	data_buf[BUFSIZ]; // buffer for data
	unsigned int a,b,c,d,e,f;
	int 	resp_code;
	
	unsigned long port; // port for data
		
	printf("Buffer Size = %d\r\n", BUFSIZ);

	// Variables for the file being received
	int	file_size,
		file_desc;
	char	*data;
	char *ptr;
		
	// Step 1: Connect to the server
	socket_ctrl = socket(AF_INET, SOCK_STREAM, 0);
	struct timeval tv;
	tv.tv_sec = 10;
	tv.tv_usec = 0;
	setsockopt(socket_ctrl, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);	
	if (socket_ctrl == -1)
	{
		perror("Could not create socket");
		return 1;
	}

	server.sin_addr.s_addr = inet_addr(SERVER_IP);
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);

	// Connect to server
	if (connect(socket_ctrl, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("Connection failed");
		return 1;
	} else {
		printf("Connected\r\n");
	}
	memset(reply_msg,0x00,BUFSIZ);
	recv(socket_ctrl, reply_msg, BUFSIZ, 0);
	printf("reply_msg: %s\r\n ", reply_msg);
	
	// Enter logging information
	strcpy(request_msg, "USER ");
	strcat(request_msg, USER_NAME);
	printf("Enter Loggin INformation: %s\r\n", request_msg);
	write(socket_ctrl, request_msg, strlen(request_msg));
	recv(socket_ctrl, reply_msg, BUFSIZ, 0);
	printf("reply_msg:  %s\r\n", reply_msg);
	
	strcpy(request_msg, "PASS ");
	strcat(request_msg, PASSWORD);
	printf("Enter Password: %s\r\n", PASSWORD);
	write(socket_ctrl, request_msg, strlen(request_msg));
	memset(reply_msg,0x00,BUFSIZ);
	recv(socket_ctrl, reply_msg, BUFSIZ, 0);
	printf("reply_msg:  %s\r\n", reply_msg);
	
	printf("\r\n=============TEST LIST DIRECTORY=========\r\n");
	// Getting the PASV port
	strcpy(request_msg, PASV);
	write(socket_ctrl, request_msg, strlen(request_msg));
	memset(reply_msg,0x00,BUFSIZ);
	recv(socket_ctrl, reply_msg, BUFSIZ, 0);
	resp_code = strtoul(reply_msg, NULL, 10);	
	printf("reply_msg:  %s resp_code = %d\r\n", reply_msg, resp_code);
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
	port = e*256+f; 
	printf("pasv port =%d\r\n", port);	

	
	// connect to data port connection
	socket_dat = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(socket_dat, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);	
	if (socket_dat == -1)
	{
		perror("Could not create socket");
		return 1;
	}
	server_dat.sin_addr.s_addr = inet_addr(SERVER_IP);
	server_dat.sin_family = AF_INET;
	server_dat.sin_port = htons(port);
	// Connect to server for data 
	if (connect(socket_dat, (struct sockaddr *)&server_dat, sizeof(server_dat)) < 0)
	{
		perror("Connection data port failed");
		return 1;
	} else {
		printf("Connected  data port\r\n");
	}
	
	// Send to get list files port 21
	strcpy(request_msg, "LIST -a\r\n");
	write(socket_ctrl, request_msg, strlen(request_msg));
	// Recv data port data
	memset(data_buf,0x00,BUFSIZ);
	recv(socket_dat, data_buf, BUFSIZ, 0);	
	printf("data_buf:  %s \r\n", data_buf);
	// recv ctrl data
	recv(socket_ctrl, reply_msg, BUFSIZ, 0);	
	printf("reply_msg:  %s\r\n", reply_msg);
	
	
	//===================TEST GET DATA===============
	printf("\r\n=============TEST GET DATA=========\r\n");
	// Getting the PASV port
	strcpy(request_msg, PASV);
	write(socket_ctrl, request_msg, strlen(request_msg));
	memset(reply_msg,0x00,BUFSIZ);
	recv(socket_ctrl, reply_msg, BUFSIZ, 0);
	resp_code = strtoul(reply_msg, NULL, 10);	
	printf("reply_msg:  %s resp_code = %d\r\n", reply_msg, resp_code);
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
	port = e*256+f; 
	printf("pasv port =%d\r\n", port);	
	// connect to data port connection
	socket_dat = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(socket_dat, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);	
	if (socket_dat == -1)
	{
		perror("Could not create socket");
		return 1;
	}
	server_dat.sin_addr.s_addr = inet_addr(SERVER_IP);
	server_dat.sin_family = AF_INET;
	server_dat.sin_port = htons(port);
	// Connect to server for data 
	if (connect(socket_dat, (struct sockaddr *)&server_dat, sizeof(server_dat)) < 0)
	{
		perror("Connection data port failed");
		return 1;
	} else {
		printf("Connected  data port\r\n");
	}
	
	// Send to get list files port 21
	strcpy(request_msg, "RETR b.zip\r\n");
	write(socket_ctrl, request_msg, strlen(request_msg));
	// Recv data port data
	memset(data_buf,0x00,BUFSIZ);
	int len = recv(socket_dat, data_buf, BUFSIZ, 0);	
	for (int i = 0; i <len; i++) {
		printf("%.2X ", (unsigned char) data_buf[i]); 
		if ((i+1)%16 == 0) printf("\r\n");
	} 
	
	// recv ctrl data
	printf("\r\nreply_msg:\r\n");
	recv(socket_ctrl, reply_msg, BUFSIZ, 0);
	printf("reply_msg:  %s \r\n", reply_msg);
	
	// Close two sections
	close(socket_dat);	
	close(socket_ctrl);
	printf("Goodbye\r\n");

	return 0;
}