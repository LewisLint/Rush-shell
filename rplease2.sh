#!/bin/zsh
# yo fuck you if you use bash or fish.

# Define the usage menu
usage() { 
    echo -e "Usage: sudo $0 -s <SSID> [-c <count>] [-d <delay>]\n   -s : Wi-Fi ESSID\n   -c : Count of fake devices\n   -d : seconds of sleep between each connection. unfortunately, this is necessary\n\n"
    exit 1 
}

# Default values if flags are omitted
c=10
d=10

# Parse the flags
while getopts "s:c:d:h" opt; do
    case ${opt} in
        s ) s=$OPTARG ;;
        c ) c=$OPTARG ;;
        d ) d=$OPTARG ;;
        h ) usage ;;
        \? ) usage ;;
    esac
done

# Check if the target SSID was provided (Notice the exact spacing here)
if [ -z "$s" ]; then
    echo -e "Use -s for an ESSID -- Required\n"
    usage
fi

# Main simulation loop
for ((shit=1; shit<=$c; shit++)); do
    echo "Simulating device #$shit of $c..."
    
    sudo ip link set wlan0 down
    sudo macchanger -r wlan0
    sudo ip link set wlan0 up
    
    # Connect with a fixed timeout so it doesn't hang forever on failures
    sudo nmcli --timeout 8 device wifi connect "$s"
    
    # Wait for the router to log the connection and issue an IP
    echo "Sleeping for $d seconds..."
    sleep "$d"
done

echo "Fleet simulation complete!"
cleanup() {
    echo -e "\n\n[!] Resetting interface to factory default. "
    sudo ip link set wlan0 down
    sudo macchanger -p wlan0        # <- Native reset flag
    sudo ip link set wlan0 up
    exit 0
}
# Trap Ctrl+C (SIGINT) and terminal termination (SIGTERM)
trap cleanup SIGINT SIGTERM
