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


#ifdef WALA
#define FILENAME 	"test1.txt"
#define SERVER_IP 	"203.162.94.57"
#define SERVER_PORT 	21
#else
// Server in Local
#define FILENAME 	"test1.txt"
#define SERVER_IP 	"127.0.0.1"
#define SERVER_PORT 	21
#endif

int main(int argc , char **argv)
{
	int 	socket_desc;
	struct 	sockaddr_in server;
	char 	request_msg[BUFSIZ],
		reply_msg[BUFSIZ];

	// Variables for the file being received
	int	file_size,
		file_desc;
	char	*data;
		
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1)
	{
		perror("Could not create socket");
		return 1;
	}

	server.sin_addr.s_addr = inet_addr(SERVER_IP);
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);

	// Connect to server
	if (connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("Connection failed");
		return 1;
	}

	// Get a file from server
	strcpy(request_msg, "Get ");
	strcat(request_msg, FILENAME);
	write(socket_desc, request_msg, strlen(request_msg));
	recv(socket_desc, reply_msg, 2, 0);
	
	// Start receiving file
	if (strcmp(reply_msg, "OK") == 0) {

		recv(socket_desc, &file_size, sizeof(int), 0);
		data = malloc(file_size);
		file_desc = open(FILENAME, O_CREAT | O_EXCL | O_WRONLY, 0666);
		recv(socket_desc, data, file_size, 0);
		write(file_desc, data, file_size);
		close(file_desc);
	}
	else {

		fprintf(stderr, "Bad request\n");
	}

	return 0;
}