#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <csignal>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>

/*
 * Minimal non-blocking HTTP server (C++98) using exactly one poll().
 * - Only handles GET (or any) request and responds with "Hello World!".
 * - Never inspects errno after read/write.
 * - Each client is read once for the request, then we send a small response.
 * - Then we close the connection (Connection: close).
 *
 * DISCLAIMER: This is a bare-minimum demonstration. Real-world servers must
 * handle partial reads/writes more robustly, parse HTTP properly, etc.
 */

//--------------------------------------------------
// Global/Configuration
//--------------------------------------------------
#define PORT 8080
#define MAX_CLIENTS 1024

static volatile int g_server_fd = -1;

//--------------------------------------------------
// Graceful shutdown on signals
//--------------------------------------------------
extern "C" void handle_signal(int) {
    std::cout << "\nShutting down server..." << std::endl;
    if (g_server_fd != -1) {
        close(g_server_fd);
        std::cout << "Server socket closed." << std::endl;
    }
    std::exit(0);
}

//--------------------------------------------------
// Client Data
//--------------------------------------------------
struct ClientInfo {
    bool    hasRequest;       // True once we've read the request
    bool    hasResponse;      // True once we've queued a response
    bool    doneWriting;      // True once the response is fully written
    std::string requestData;  // Storage for request data
    std::string responseData; // Storage for response data
};

//--------------------------------------------------
// Main
//--------------------------------------------------
int main() {
    // Setup signals for graceful shutdown
    signal(SIGINT,  handle_signal);
    signal(SIGTERM, handle_signal);
    signal(SIGTSTP, handle_signal);

    // Create listening socket (non-blocking)
    g_server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (g_server_fd < 0) {
        std::cerr << "socket() failed." << std::endl;
        return 1;
    }

    int opt = 1;
    setsockopt(g_server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    fcntl(g_server_fd, F_SETFL, O_NONBLOCK); // Non-blocking

    // Bind
    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port        = htons(PORT);
    if (bind(g_server_fd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "bind() failed." << std::endl;
        close(g_server_fd);
        return 1;
    }

    // Listen
    if (listen(g_server_fd, 10) < 0) {
        std::cerr << "listen() failed." << std::endl;
        close(g_server_fd);
        return 1;
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    // Poll array
    struct pollfd fds[MAX_CLIENTS];
    std::memset(fds, 0, sizeof(fds));

    // Client info array
    ClientInfo clients[MAX_CLIENTS];

    // Initialize listening socket in poll array
    fds[0].fd = g_server_fd;
    fds[0].events = POLLIN; // watch for inbound connections
    int nfds = 1;

    // Main loop
    for (;;) {
        // Single poll() call for all fds
        int ret = poll(fds, nfds, -1);
        if (ret < 0) {
            std::cerr << "poll() failed." << std::endl;
            break;
        }

        // Loop over fds
        for (int i = 0; i < nfds; i++) {
            // If nothing happened on this fd, skip
            if (fds[i].revents == 0)
                continue;

            // Check for new connections on listening socket
            if (fds[i].fd == g_server_fd && (fds[i].revents & POLLIN)) {
                // Accept as many as possible
                for (;;) {
                    int cfd = accept(g_server_fd, NULL, NULL);
                    if (cfd < 0) {
                        // No more inbound connections
                        break;
                    }
                    // Make it non-blocking
                    fcntl(cfd, F_SETFL, O_NONBLOCK);

                    // Add to poll array
                    fds[nfds].fd     = cfd;
                    fds[nfds].events = POLLIN | POLLOUT; // track both read & write
                    clients[nfds].hasRequest   = false;
                    clients[nfds].hasResponse  = false;
                    clients[nfds].doneWriting  = false;
                    clients[nfds].requestData.clear();
                    clients[nfds].responseData.clear();
                    nfds++;
                }
            }
            // Otherwise, handle existing client
            else {
                int cfd = fds[i].fd;
                // If POLLIN is set, attempt to read
                if (fds[i].revents & POLLIN) {
                    if (!clients[i].hasRequest) {
                        char buf[1024];
                        int r = recv(cfd, buf, sizeof(buf), 0);
                        if (r == 0) {
                            // client closed
                            close(cfd);
                            // remove from poll array
                            fds[i] = fds[nfds - 1];
                            clients[i] = clients[nfds - 1];
                            nfds--;
                            i--; // re-check this index in next iteration
                            continue;
                        }
                        if (r > 0) {
                            // Append to request
                            clients[i].requestData.append(buf, r);

                            // Minimal parse: once we see "\r\n\r\n", consider request complete
                            if (clients[i].requestData.find("\r\n\r\n") != std::string::npos) {
                                // Build minimal response
                                std::string response(
                                    "HTTP/1.1 200 OK\r\n"
                                    "Content-Type: text/plain\r\n"
                                    "Content-Length: 12\r\n"
                                    "Connection: close\r\n"
                                    "\r\n"
                                    "Hello World!"
                                );
                                clients[i].responseData = response;
                                clients[i].hasResponse  = true;
                                clients[i].hasRequest   = true;
                            }
                        }
                    }
                }

                // If POLLOUT is set, attempt to send any response
                if (fds[i].revents & POLLOUT) {
                    if (clients[i].hasResponse && !clients[i].doneWriting) {
                        // Send what's left of responseData
                        const char* data = clients[i].responseData.c_str();
                        int total = (int)clients[i].responseData.size();
                        if (total > 0) {
                            int s = send(cfd, data, total, 0);
                            if (s > 0 && s < total) {
                                // We only sent partial
                                clients[i].responseData.erase(0, s);
                            }
                            else if (s == total) {
                                // Sent all
                                clients[i].doneWriting = true;
                            }
                            else if (s <= 0) {
                                // Either error or EAGAIN-like scenario
                                // We'll let poll() handle re-tries, or close if next round
                            }
                        }
                    }
                }

                // If we've finished sending, close connection
                if (clients[i].hasResponse && clients[i].doneWriting) {
                    close(cfd);
                    // remove from poll array
                    fds[i] = fds[nfds - 1];
                    clients[i] = clients[nfds - 1];
                    nfds--;
                    i--; // re-check same index
                }
            }
        }
    }

    // Cleanup
    for (int i = 0; i < nfds; i++) {
        if (fds[i].fd >= 0)
            close(fds[i].fd);
    }
    return 0;
}
