#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/ip_icmp.h>
#include <netinet/in.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>

#include "scanner.h"

#define TIMEOUT 1
#define ICMP_ECHO_REQUEST 8

// Function to calculate the checksum
unsigned short checksum(void *b, int len) {    
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    while (len > 1) {
        sum += *buf++;
        len -= 2;
    }

    if (len == 1) {
        sum += *(unsigned char *)buf;
    }

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;

    return result;
}

void scan_ip(const char *ip_address) {
    int sockfd;
    struct sockaddr_in sa;
    struct icmphdr icmp_hdr;
    struct timeval tv;
    fd_set fdset;
    int result;
    char packet[64];
    
    // Create a socket
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
        perror("socket");
        return;
    }

    // Set socket options
    tv.tv_sec = TIMEOUT;
    tv.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    // Prepare ICMP packet
    memset(&icmp_hdr, 0, sizeof(icmp_hdr));
    icmp_hdr.type = ICMP_ECHO_REQUEST;
    icmp_hdr.un.echo.id = getpid();
    icmp_hdr.un.echo.sequence = 1;
    icmp_hdr.checksum = checksum(&icmp_hdr, sizeof(icmp_hdr));

    // Set up the address structure
    sa.sin_family = AF_INET;
    sa.sin_port = 0;
    inet_pton(AF_INET, ip_address, &sa.sin_addr);

    // Send the ICMP packet
    result = sendto(sockfd, &icmp_hdr, sizeof(icmp_hdr), 0, (struct sockaddr *)&sa, sizeof(sa));
    if (result < 0) {
        perror("sendto");
        close(sockfd);
        return;
    }

    // Set up for receiving response
    FD_ZERO(&fdset);
    FD_SET(sockfd, &fdset);
    result = select(sockfd + 1, &fdset, NULL, NULL, &tv);

    if (result > 0) {
        printf("IP %s is active\n", ip_address);
    } else {
        printf("IP %s is not reachable\n", ip_address);
    }

    // Close the socket
    close(sockfd);
}
