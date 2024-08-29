#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

void convert_ipv4_to_ipv6(const char *ipv4_address) {
    struct in_addr ipv4_addr;
    struct in6_addr ipv6_addr;
    char ipv6_str[INET6_ADDRSTRLEN];

    // Convert the IPv4 string to a struct in_addr
    if (inet_pton(AF_INET, ipv4_address, &ipv4_addr) != 1) {
        printf("Invalid IPv4 address\n");
        return;
    }

    // Create an IPv4-mapped IPv6 address (::ffff:IPv4_address)
    memset(&ipv6_addr, 0, sizeof(ipv6_addr));
    ipv6_addr.s6_addr[10] = 0xff;
    ipv6_addr.s6_addr[11] = 0xff;
    memcpy(&ipv6_addr.s6_addr[12], &ipv4_addr, sizeof(ipv4_addr));

    // Convert the IPv6 address to a string
    if (inet_ntop(AF_INET6, &ipv6_addr, ipv6_str, sizeof(ipv6_str)) == NULL) {
        printf("Error converting to IPv6 address\n");
        return;
    }

    printf("IPv6 address: %s\n", ipv6_str);
}

int main() {
    const char *ipv4_address = "192.168.1.1";  // Replace with your IPv4 address
    convert_ipv4_to_ipv6(ipv4_address);
    return 0;
}
