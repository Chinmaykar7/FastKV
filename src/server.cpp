#include "server.h"

#include <arpa/inet.h>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

namespace {
constexpr int kInvalidFd = -1;
constexpr int kListenBacklog = 128;
constexpr std::size_t kReceiveBufferSize = 4096;

bool closeSocket(int& fd) {
    if (fd == kInvalidFd) {
        return true;
    }

    if (close(fd) == -1) {
        std::cerr << "close() failed: " << std::strerror(errno) << '\n';
        fd = kInvalidFd;
        return false;
    }

    fd = kInvalidFd;
    return true;
}
}

Server::Server(int port)
    : port_(port), server_fd_(kInvalidFd) {}

Server::~Server() {
    closeSocket(server_fd_);
}

bool Server::start() {
    if (server_fd_ != kInvalidFd) {
        std::cerr << "RedisLite server is already started\n";
        return false;
    }

    if (port_ <= 0 || port_ > 65535) {
        std::cerr << "Invalid TCP port: " << port_ << '\n';
        return false;
    }

    std::cout << "RedisLite starting on port " << port_ << '\n';

    server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd_ == kInvalidFd) {
        std::cerr << "socket() failed: " << std::strerror(errno) << '\n';
        return false;
    }

    int reuse_addr = 1;
    if (setsockopt(server_fd_,
                   SOL_SOCKET,
                   SO_REUSEADDR,
                   &reuse_addr,
                   sizeof(reuse_addr)) == -1) {
        std::cerr << "setsockopt(SO_REUSEADDR) failed: "
                  << std::strerror(errno) << '\n';
        closeSocket(server_fd_);
        return false;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(static_cast<uint16_t>(port_));

    if (bind(server_fd_,
             reinterpret_cast<sockaddr*>(&server_addr),
             sizeof(server_addr)) == -1) {
        std::cerr << "bind() failed on port " << port_ << ": "
                  << std::strerror(errno) << '\n';
        closeSocket(server_fd_);
        return false;
    }

    if (listen(server_fd_, kListenBacklog) == -1) {
        std::cerr << "listen() failed: " << std::strerror(errno) << '\n';
        closeSocket(server_fd_);
        return false;
    }

    std::cout << "RedisLite listening on 0.0.0.0:" << port_ << '\n';
    std::cout << "Waiting for client connections\n";

    for (;;) {
        sockaddr_in client_addr{};
        socklen_t client_addr_len = sizeof(client_addr);

        int client_fd = accept(server_fd_,
                               reinterpret_cast<sockaddr*>(&client_addr),
                               &client_addr_len);
        if (client_fd == kInvalidFd) {
            if (errno == EINTR) {
                continue;
            }

            std::cerr << "accept() failed: " << std::strerror(errno) << '\n';
            closeSocket(server_fd_);
            return false;
        }

        char client_ip[INET_ADDRSTRLEN]{};
        const char* formatted_ip = inet_ntop(AF_INET,
                                             &client_addr.sin_addr,
                                             client_ip,
                                             sizeof(client_ip));
        if (formatted_ip == nullptr) {
            std::cerr << "inet_ntop() failed: " << std::strerror(errno) << '\n';
            closeSocket(client_fd);
            continue;
        }

        const uint16_t client_port = ntohs(client_addr.sin_port);

        std::cout << "Client connected: ip=" << client_ip
                  << " port=" << client_port
                  << " fd=" << client_fd << '\n';

        char buffer[kReceiveBufferSize]{};
        const ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
        if (bytes_received > 0) {
            std::cout << "Received " << bytes_received << " bytes\n";
            std::cout.write(buffer, bytes_received);
            std::cout << '\n';
        } else if (bytes_received == 0) {
            std::cout << "Client disconnected before sending data\n";
        } else {
            std::cerr << "recv() failed: " << std::strerror(errno) << '\n';
        }

        const int accepted_fd = client_fd;
        if (closeSocket(client_fd)) {
            std::cout << "Client closed: fd=" << accepted_fd << '\n';
        }
    }
}
