#include "client.h"

void Socket(int domain, int type, int protocol) {
    fd = socket(domain, type, protocol);
    if (fd == -1) {
        perror(" failed");
        exit(EXIT_FAILURE);
    }

}

void Inet_pton(int af, const char *src, void *dst) {
    int res = inet_pton(af, src, dst);
    if (res == 0) {
        exit(EXIT_FAILURE);
    }
    if (res == -1) {
        perror(" failed");
        exit(EXIT_FAILURE);
    }
}

int Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    int res = connect(sockfd, addr, addrlen);
    if (res == -1) {
        perror(" CONNECTION failed");
        //exit(EXIT_FAILURE);
        return 0;
    }

    return 1;
}
