#include "server.hpp"
#include <iostream>

int main() {
	try {
		HttpServer server(8080); // Create a server on port 8080
		server.run();            // Start the server loop
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << "\n";
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
