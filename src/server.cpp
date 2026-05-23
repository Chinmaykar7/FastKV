#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define PORT "6379"
#define BACKLOG 10

int main(void) {
    struct addrinfo hints, *res, *p;
    int sockfd, new_fd;
    int rv;
    int yes = 1;
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    char buffer[4096];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;

    rv = getaddrinfo(NULL, PORT, &hints, &res);
    if (rv != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for (p = res; p != NULL; p = p->ai_next) {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == -1) {
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
            perror("setsockopt");
            close(sockfd);
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            continue;
        }

        break;
    }

    freeaddrinfo(res);

    if (p == NULL) {
        fprintf(stderr, "failed to bind\n");
        return 2;
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        close(sockfd);
        return 3;
    }

    printf("server listening on port %s\n", PORT);

    addr_size = sizeof their_addr;
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr,&addr_size);
    if (new_fd == -1) {
        perror("accept");
        close(sockfd);
        return 4;
    }
    printf("client connected\n");
    int bytes_received =
        recv(new_fd,
             buffer,
             sizeof(buffer),
             0);

    if (bytes_received == -1) {
        perror("recv");
    }
    else if (bytes_received == 0) {
        printf("client disconnected\n");
    }
    else {

        buffer[bytes_received] = '\0';

        printf("received: %s\n", buffer);
    }

    close(new_fd);
    close(sockfd);
    return 0;
}
