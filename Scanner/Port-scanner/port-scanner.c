#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/select.h>

#define TIMEOUT 1 // Timeout for the connection attempt

// Function to create a socket and check if a port is open
int scan_port(const char *target_ip, int port)
{
    int sockfd;
    struct sockaddr_in server_addr;
    struct timeval tv;
    fd_set set;
    int result;
    int flags;

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        return 0;
    }

    // Set the socket to non-blocking mode
    flags = fcntl(sockfd, F_GETFL, 0);
    if (flags < 0)
    {
        perror("fcntl(F_GETFL)");
        close(sockfd);
        return 0;
    }
    if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) < 0)
    {
        perror("fcntl(F_SETFL)");
        close(sockfd);
        return 0;
    }

    // Set up server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    if (inet_pton(AF_INET, target_ip, &server_addr.sin_addr) <= 0)
    {
        perror("inet_pton");
        close(sockfd);
        return 0;
    }
    server_addr.sin_port = htons(port);

    // Attempt to connect
    result = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (result < 0 && errno != EINPROGRESS)
    {
        close(sockfd);
        return 0;
    }

    // Set up the timeout
    tv.tv_sec = TIMEOUT;
    tv.tv_usec = 0;
    FD_ZERO(&set);
    FD_SET(sockfd, &set);

    // Wait for the connection to complete or timeout
    result = select(sockfd + 1, NULL, &set, NULL, &tv);
    if (result <= 0)
    {
        close(sockfd);
        return 0;
    }

    // Check if the socket is connected
    int so_error;
    socklen_t len = sizeof(so_error);
    getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &so_error, &len);
    close(sockfd);

    // Return 1 if the port is open, 0 otherwise
    return so_error == 0;
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s <IP> <start_port> <end_port>\n", argv[0]);
        return 1;
    }

    const char *target_ip = argv[1];
    int start_port = atoi(argv[2]);
    int end_port = atoi(argv[3]);

    if (start_port < 1 || end_port > 65535 || start_port > end_port)
    {
        fprintf(stderr, "Invalid port range\n");
        return 1;
    }

    printf("Scanning ports %d-%d on %s...\n", start_port, end_port, target_ip);

    for (int port = start_port; port <= end_port; port++)
    {
        if (scan_port(target_ip, port))
        {
            printf("Port %d is open\n", port);
            fflush(stdout);
        }
    }

    return 0;
}