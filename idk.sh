#!/bin/zsh

while true;
do 
	sudo ip link set dev wlan0 down && \
	sudo macchanger -r wlan0 && \
	sudo ip link set dev wlan0 up && \
	sudo dhclient wlan0
	# sleep 60
done

# Keep in mind that this may or may not make your router run out of IP addresses
