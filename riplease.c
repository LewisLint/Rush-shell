#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
    // Ensure at least one interface argument was passed (argv[0] is the program name)
    if (argc < 2) {
        printf("Usage: %s <interface1> <interface2> ...\n", argv[0]);
        return 1;
    }

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("[-] Socket creation failed");
        return 1;
    }

    // Seed the random number generator once
    srand(time(NULL) ^ getpid());

    // Loop through all interfaces passed as arguments ($1, $2, etc.)
    for (int i = 1; i < argc; i++) {
        struct ifreq ifr = {0};
        const char *iface = argv[i];

        strncpy(ifr.ifr_name, iface, IFNAMSIZ - 1);
        printf("[*] Processing interface: %s\n", iface);

        // 1. Bring the interface down
        if (ioctl(sock, SIOCGIFFLAGS, &ifr) < 0) {
            perror("[-] Failed to get interface flags");
            continue; // Skip to the next interface if this one doesn't exist
        }
        ifr.ifr_flags &= ~IFF_UP;
        ioctl(sock, SIOCSIFFLAGS, &ifr);

        // 2. Randomize and set the MAC address
        ifr.ifr_hwaddr.sa_family = 1; // ARPHRD_ETHER
        ifr.ifr_hwaddr.sa_data[0] = (rand() & 0xFC) | 0x02; 
        for (int j = 1; j < 6; j++) {
            ifr.ifr_hwaddr.sa_data[j] = rand() & 0xFF;
        }
        if (ioctl(sock, SIOCSIFHWADDR, &ifr) < 0) {
            perror("[-] Failed to set MAC address");
        }

        // 3. Randomize and set the Hostname (Only needs to happen once, but kept per target loop)
        char new_host[32];
        const char charset[] = "abcdefghijklmnopqrstuvwxyz0123456789";
        strcpy(new_host, "host-");
        for (int j = 5; j < 11; j++) {
            new_host[j] = charset[rand() % (sizeof(charset) - 1)];
        }
        new_host[11] = '\0';
        sethostname(new_host, strlen(new_host));

        // 4. Clear existing IP address to force a fresh lease
        struct sockaddr_in *sin = (struct sockaddr_in *)&ifr.ifr_addr;
        sin->sin_family = AF_INET;
        sin->sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
        ioctl(sock, SIOCSIFADDR, &ifr);

        // 5. Bring the interface back up
        ioctl(sock, SIOCGIFFLAGS, &ifr);
        ifr.ifr_flags |= IFF_UP;
        ioctl(sock, SIOCSIFFLAGS, &ifr);

        // 6. Request a new IP address via DHCP
        printf("[*] Requesting new DHCP lease for %s...\n", iface);
        char dhcp_cmd[128];
        snprintf(dhcp_cmd, sizeof(dhcp_cmd), "dhclient -r %s && dhclient %s", iface, iface);
        
        if (system(dhcp_cmd) == 0) {
            printf("[+] IP address successfully renewed for %s.\n", iface);
        } else {
            printf("[-] Failed to execute DHCP client for %s.\n", iface);
        }
        
        printf("\n");
    }

    close(sock);
    return 0;
}
