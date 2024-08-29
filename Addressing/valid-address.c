#include <stdio.h>
#include <arpa/inet.h>

int is_valid_ipv4(const char *ip_str) {
    struct in_addr ipv4_addr;

    // Convert the IPv4 string to a struct in_addr
    if (inet_pton(AF_INET, ip_str, &ipv4_addr) == 1) {
        return 1;  // Valid IPv4 address
    } else {
        return 0;  // Invalid IPv4 address
    }
}

int main() {
    const char *ipv4_address = "192.168.1.1";  // Replace with the IP address to check

    if (is_valid_ipv4(ipv4_address)) {
        printf("%s is a valid IPv4 address.\n", ipv4_address);
    } else {
        printf("%s is not a valid IPv4 address.\n", ipv4_address);
    }

    return 0;
}
