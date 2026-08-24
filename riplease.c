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
    // Ensure both the interface and the loop count are provided
    if (argc < 3) {
        printf("Usage: %s <interface> <loop_count>\n", argv[0]);
        printf("Example: %s wlan0 5\n", argv[0]);
        return 1;
    }

    const char *iface = argv[1];
    
    // Convert the second argument string (e.g., "5") into an integer
    int total_runs = atoi(argv[2]);
    if (total_runs <= 0) {
        printf("[-] Error: Loop count must be a positive integer.\n");
        return 1;
    }

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("[-] Socket creation failed");
        return 1;
    }

    // Seed the random number generator once
    srand(time(NULL) ^ getpid());

    printf("[*] Starting loop: Target interface '%s' for %d iterations.\n\n", iface, total_runs);

    // Controlled loop using the integer input
    for (int i = 0; i < total_runs; i++) {
        struct ifreq ifr = {0};
        strncpy(ifr.ifr_name, iface, IFNAMSIZ - 1);

        printf("[*] --- Iteration %d of %d ---\n", i + 1, total_runs);

        // 1. Bring the interface down
        if (ioctl(sock, SIOCGIFFLAGS, &ifr) < 0) {
            perror("[-] Failed to get interface flags (Is the device name correct?)");
            close(sock);
            return 1; // Exit early if the interface doesn't exist
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

        // 3. Randomize and set the Hostname
        char new_host[16];
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
            printf("[+] IP address successfully renewed.\n");
        } else {
            printf("[-] Failed to execute DHCP client.\n");
        }
        
        printf("\n");
        
        // Optional: If running multiple times, add a small sleep delay between iterations
        if (i < total_runs - 1) {
            sleep(2); 
        }
    }

    close(sock);
    return 0;
}
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
