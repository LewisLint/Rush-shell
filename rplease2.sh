#!/bin/zsh

# Check if an argument was passed
if [[ -z "$1" ]]; then
    echo "Usage: $0 <number_of_iterations>"
    exit 1
fi

cpl=0
for ((i=1; i<=$1; i++)); do
        ((cpl++))
        # Silence the interface text so your custom loop messages pop out
        sudo ip link set dev wlan0 down >/dev/null 2>&1 && \
        sudo macchanger -r wlan0 >/dev/null 2>&1 && \
        sudo ip link set dev wlan0 up >/dev/null 2>&1
    
        # A tiny pause lets the physical Wi-Fi hardware catch its breath after coming up
        sleep 0.5

        # Cleaner random generation that won't throw SIGPIPE errors
        HOSTNAME=$(LC_ALL=C tr -dc 'a-z0-9' < /dev/urandom | dd bs=1 count=12 2>/dev/null)
    
        # sudo dhclient -r wlan0 
        # fuck the router. This script would be kinda fucking useless if we just 
        # said "yo bro, im not using this anymore" when we're trying to exhaust the lease pool
    
        if sudo dhclient -H "$HOSTNAME" wlan0 >/dev/null 2>&1; then
            echo "--> loop $cpl: Successfully claimed a new IP lease <---"
        else
            echo "--> loop $cpl: DHCP request failed <---"
        fi
        #sleep 60
        # sleep is for the weak (people and wlan cards alike)
done
    
    # Keep in mind that this changes the interface identity and
    # repeatedly requests DHCP leases.
    # this might piss off the DHCP server lol
    
