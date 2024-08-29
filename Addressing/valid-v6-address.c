#include <stdio.h>
#include <arpa/inet.h>

int is_valid_ipv6(const char *ip_str) {
    struct in6_addr ipv6_addr;

    // Convert the IPv6 string to a struct in6_addr
    if (inet_pton(AF_INET6, ip_str, &ipv6_addr) == 1) {
        return 1;  // Valid IPv6 address
    } else {
        return 0;  // Invalid IPv6 address
    }
}

int main() {
    const char *ipv6_address = "2001:0db8:85a3:0000:0000:8a2e:0370:7334";  // Replace with the IP address to check

    if (is_valid_ipv6(ipv6_address)) {
        printf("%s is a valid IPv6 address.\n", ipv6_address);
    } else {
        printf("%s is not a valid IPv6 address.\n", ipv6_address);
    }

    return 0;
}
