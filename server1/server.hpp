#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <netinet/in.h> // For sockaddr_in

class HttpServer {
private:
	int _server_fd; // File descriptor for the server socket
	struct sockaddr_in _address;
	const int _port;

public:
	HttpServer(int port = 8080); // Constructor with default port 8080
	~HttpServer();              // Destructor to clean up resources

	void run(); // Starts the server loop

private:
	void handleClient(int client_socket); // Handles a single client connection
};

#endif
