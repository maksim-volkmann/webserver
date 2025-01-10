#include <sys/socket.h>

if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
{
	perror(“cannot create socket”); 
	return 0; 
}