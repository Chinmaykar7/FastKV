#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define PORT "6379"

int main() {
    struct addrinfo hints, *res, *p;
    int sockfd = -1;
    int rv;

    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;     // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;  // TCP
    // no AI_PASSIVE for client

    rv = getaddrinfo("localhost", PORT, &hints, &res);
    if (rv != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(rv) << std::endl;
        return 1;
    }

    for (p = res; p != nullptr; p = p->ai_next) {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == -1) {
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            sockfd = -1;
            continue;
        }

        break;
    }

    freeaddrinfo(res);

    if (p == nullptr) {
        std::cerr << "failed to connect" << std::endl;
        return 2;
    }

    std::cout << "Connection done" << std::endl;

    std::string msg = "Hello World";
    ssize_t bytes_sent = send(sockfd, msg.c_str(), msg.size(), 0);

    if (bytes_sent == -1) {
        perror("send");
        close(sockfd);
        return 3;
    }

    std::cout << "Sent " << bytes_sent << " bytes" << std::endl;

    close(sockfd);
    return 0;
}