# heating_control_via_openhab
Heizungssteuerung via OpenHAB 2

Compile Command:
clear;rm -frv ssh_command.o;rm -frv read_dht22.o;rm -frv read_ds18b20.o;rm -frv heizungssteuerung_lokal_via_openhab.o;rm -frv heizungssteuerung_lokal_via_openhab;g++ -g -fPIC -Wall -c ssh_command.cpp;g++ -g -fPIC -Wall -c read_ds18b20.cpp;g++ -g -fPIC -Wall -c read_dht22.cpp;g++ -g -fPIC -Wall -c heizungssteuerung_lokal_via_openhab.cpp;g++ -g -fPIC -Wall -o heizungssteuerung_lokal_via_openhab heizungssteuerung_lokal_via_openhab.o read_ds18b20.o read_dht22.o ssh_command.o -lwiringPi -lssh; date;	
