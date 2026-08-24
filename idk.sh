#!/bin/zsh
cpl=0
while true; do
    ((cpl++))
    sudo ip link set dev wlan0 down && \
    sudo macchanger -r wlan0 && \
    sudo ip link set dev wlan0 up 

    HOSTNAME=$(LC_ALL=C tr -dc 'a-z0-9' < /dev/urandom | head -c 12)

    # sudo dhclient -r wlan0 
    # fuck the router. This script would be kinda fucking useless if we just 
    # said "yo bro, im not using this anymore" when we're trying to exhaust the lease pool
    sudo dhclient -H "$HOSTNAME" wlan0

    if [ $? -eq 0 ]; then
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
