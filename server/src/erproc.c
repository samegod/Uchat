#include "server.h"

int Socket(int domain, int type, int protocol) {
    int res = socket(domain, type, protocol);
    printf("Creating socket:");
    if (res == -1) {
        perror(" failed\n");
        exit(EXIT_FAILURE);
    }

    printf(" successfully\n");
    return res;
}

void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    int res = bind(sockfd, addr, addrlen);
    printf ("Binding:");
    if (res == -1) {
        perror(" failed\n");
        exit(EXIT_FAILURE);
    }

    printf (" successfully\n");
}

void Listen(int sockfd, int backlog) {
    int res = listen(sockfd, backlog);
    printf("Listening:");
    if (res == -1) {
        perror(" failed");
        exit(EXIT_FAILURE);
    }
    printf (" successfully\n\n");
}

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
    int res = accept(sockfd, addr, addrlen);
    printf("Accepting:");
    if (res == -1) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    printf (" successfully\n\n");
    return res;
}
