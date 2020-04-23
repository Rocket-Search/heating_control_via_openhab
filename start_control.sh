#!/bin/bash

#set -x

lsof -a -p $(pgrep ssh-agent) -U -F n | sed -n 's/^n//p' | awk -F " " '{print $1}' > /dev/null

export SSH_AUTH_SOCK=$(lsof -a -p $(pgrep ssh-agent) -U -F n | sed -n 's/^n//p' | awk -F " " '{print $1}') 

temperatur_soll=$1

#ssh_kommando="/home/pi/scripte/heizkkoerpersteuerung_gpio $temperatur_soll 26"
#ssh_kommando="echo 666"

#ssh root@192.168.178.30 ${ssh_kommando}

/opt/openhab2/conf/scripts/heizungssteuerung_lokal_via_openhab/heizungssteuerung_lokal_via_openhab $temperatur_soll
