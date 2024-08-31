#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"

#define MAX_IP_LENGTH 16

void scan_range(const char *base_ip) {
    char ip[MAX_IP_LENGTH];
    char base_ip_copy[MAX_IP_LENGTH];
    int base;

    // Copy base_ip to a modifiable string
    strncpy(base_ip_copy, base_ip, MAX_IP_LENGTH);
    base_ip_copy[MAX_IP_LENGTH - 1] = '\0'; // Ensure null-termination

    // Find the last dot in the IP address
    char *last_dot = strrchr(base_ip_copy, '.');
    if (last_dot == NULL) {
        fprintf(stderr, "Invalid IP address format\n");
        return;
    }

    // Extract the base part of the IP address
    base = atoi(last_dot + 1);
    *last_dot = '\0'; // Terminate the string at the last dot

    for (int i = 1; i < 255; i++) {
        snprintf(ip, sizeof(ip), "%s.%d", base_ip_copy, base + i);
        scan_ip(ip);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <IP address or base IP>\n", argv[0]);
        return 1;
    }

    scan_range(argv[1]);

    return 0;
}