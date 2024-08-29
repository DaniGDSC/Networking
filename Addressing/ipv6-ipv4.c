#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>

void convert_ipv6_to_ipv4(const char *ipv6_address) {
    struct in6_addr ipv6_addr;
    struct in_addr ipv4_addr;
    char ipv4_str[INET_ADDRSTRLEN];

    // Convert the IPv6 string to a struct in6_addr
    if (inet_pton(AF_INET6, ipv6_address, &ipv6_addr) != 1) {
        printf("Invalid IPv6 address\n");
        return;
    }

    // Check if the IPv6 address is an IPv4-mapped IPv6 address
    if (ipv6_addr.s6_addr[10] == 0xff && ipv6_addr.s6_addr[11] == 0xff) {
        // Extract the IPv4 address from the IPv4-mapped IPv6 address
        memcpy(&ipv4_addr, &ipv6_addr.s6_addr[12], sizeof(ipv4_addr));

        // Convert the IPv4 address to a string
        if (inet_ntop(AF_INET, &ipv4_addr, ipv4_str, sizeof(ipv4_str)) == NULL) {
            printf("Error converting to IPv4 address\n");
            return;
        }

        printf("IPv4 address: %s\n", ipv4_str);
    } else {
        printf("The IPv6 address is not an IPv4-mapped address.\n");
    }
}

int main() {
    const char *ipv6_address = "2001:0db8:85a3:0000:0000:8a2e:0370:7334";  // Replace with your IPv6 address

    convert_ipv6_to_ipv4(ipv6_address);
    return 0;
}
