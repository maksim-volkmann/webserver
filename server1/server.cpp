#include "server.hpp"
#include <iostream>
#include <cstring>    // For memset
#include <sys/socket.h>
#include <unistd.h>   // For close()

HttpServer::HttpServer(int port) : _server_fd(-1), _port(port) {
	// Initialize the socket
	if ((_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	// Configure the address structure
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons(_port);
	memset(_address.sin_zero, '\0', sizeof(_address.sin_zero));

	// Bind the socket to the port
	if (bind(_server_fd, (struct sockaddr*)&_address, sizeof(_address)) < 0) {
		perror("Bind failed");
		exit(EXIT_FAILURE);
	}

	// Start listening for connections
	if (listen(_server_fd, 10) < 0) {
		perror("Listen failed");
		exit(EXIT_FAILURE);
	}

	std::cout << "Server started on port " << _port << "\n";
}

HttpServer::~HttpServer() {
	// Close the server socket when the object is destroyed
	if (_server_fd != -1) {
		close(_server_fd);
		std::cout << "Server stopped.\n";
	}
}

void HttpServer::run() {
	while (true) {
		std::cout << "\n+++++++ Waiting for new connection ++++++++\n\n";

		int addrlen = sizeof(_address);
		int new_socket = accept(_server_fd, (struct sockaddr*)&_address, (socklen_t*)&addrlen);
		if (new_socket < 0) {
			perror("Accept failed");
			continue;
		}

		handleClient(new_socket);
	}
}

void HttpServer::handleClient(int client_socket) {
	char buffer[30000] = {0};
	long valread = read(client_socket, buffer, sizeof(buffer));

	if (valread > 0) {
		std::cout << "Received request:\n" << buffer << "\n";

		// Send a minimal HTTP response
		std::string response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
		send(client_socket, response.c_str(), response.size(), 0);

		std::cout << "Response sent.\n";
	} else {
		std::cerr << "Failed to read from client.\n";
	}

	close(client_socket); // Close client connectiona
}
