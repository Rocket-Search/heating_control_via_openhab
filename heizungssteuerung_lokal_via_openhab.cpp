//Mehrzeiler DEBUG
//clear;
//rm -frv read_ds18b20.o;
//rm -frv heizungssteuerung_lokal_via_openhab.o;
//rm -frv heizungssteuerung_lokal_via_openhab; 
//rm -frv read_dht22.o; 
//rm -frv ssh_command.o;
//#
//g++ -g -fPIC -Wall -c read_ds18b20.cpp
//g++ -g -fPIC -Wall -c read_dht22.cpp
//g++ -g -fPIC -Wall -c heizungssteuerung_lokal_via_openhab.cpp
//g++ -g -fPIC -Wall -c ssh_command.cpp 
//#
//g++ -g -fPIC -Wall -o heizungssteuerung_lokal_via_openhab heizungssteuerung_lokal_via_openhab.o read_ds18b20.o read_dht22.o ssh_command.o -lwiringPi -lssh;
//#
 
//Einzeiler DEBUG
//clear;rm -frv ssh_command.o;rm -frv read_dht22.o;rm -frv read_ds18b20.o;rm -frv heizungssteuerung_lokal_via_openhab.o;rm -frv heizungssteuerung_lokal_via_openhab;g++ -g -fPIC -Wall -c ssh_command.cpp;g++ -g -fPIC -Wall -c read_ds18b20.cpp;g++ -g -fPIC -Wall -c read_dht22.cpp;g++ -g -fPIC -Wall -c heizungssteuerung_lokal_via_openhab.cpp;g++ -g -fPIC -Wall -o heizungssteuerung_lokal_via_openhab heizungssteuerung_lokal_via_openhab.o read_ds18b20.o read_dht22.o ssh_command.o -lwiringPi -lssh; date;	

//Ausführung
//wird über openhab (lokal) aufgerufen
//./heizungssteuerung_lokal_via_openhab 25

//Manueller Test am Pi Zero
//gpio export 12 out
//gpio -g write 12 0
//gpio -g write 12 1
//1 = AN
//0 = AUS

//TO DO
//Loggiing to File
//Email bei Fehler
//Rückwerte nach openhab
//Statistik in rrdtool und ggf. grafana/informix
//Refernez Check über Heizungsthermometer ob die Heizung wirklich an oder aus ist
//ssh agent starten bzw. variablen mit Leben füllen
//SOLL Temperatur justieren

//C_C++ Header
//#include <dirent.h>
//#include <sys/stat.h>
//#include <sys/types.h>
//#include unistd.h für sleep 
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>
//#include <iomanip>
#include <iostream>
#include <vector>
//#include <sys/time.h>
//#include <sstream>
#include <fstream>
//#include <pthread.h>
//#include <thread>
//#include <time.h>
#include <errno.h>
#include <signal.h>
//#include <malloc.h>
//#include <errno.h>
//#include <sys/mman.h>
//#include <csignal>
//#include <iterator>
//#include <pthread.h>
//Header für setrlimit (ulimits setzen)
//#include <sys/resource.h>
#include <cstring>		//strlen
#include <algorithm>	//string erase
//GPIO wiring PI Header
#include <wiringPi.h>
//#include <libssh/libssh.h>	//SSH 

using namespace std;

string temperatur_ist;
string temperatur_soll;
string feuchtigkeits_messung;
//string config_file_pfad;
string wiring_pi_nummer;
//Für logging
string ds18b20_path_id;
string ssh_kommando;
string ssh_kommando_output;
string ssh_server;
float humidity;

//vector<string> config_file_parameter;
bool zu_kalt = false;
bool zu_warm = false;
bool dht22_falsche_werte = true;

void signalHandler( int signum) 
{
	cout << "Interrupt signal (" << signum << ") received.\n";
	cout<<"PROBLEM Heater Control Daemon"<<endl;
	
	//exit (666);
	exit (- 1);
}

int main(int argc, char *argv[]) 
{
//####################################################################
	//Error Signals

	signal(SIGSEGV, signalHandler);
	signal(SIGABRT, signalHandler);
	signal(SIGFPE, signalHandler);
	signal(SIGILL, signalHandler);
	signal(SIGTERM, signalHandler);
	signal(SIGINT, signalHandler);

//####################################################################
//SOLL Temp #Thermo im Raum #Kommt von OpenHAB
	//string temperatur_soll;
	//prüfen auf 2 stellen, werte zwischen 15 und 45 grad nur zahlen etc.
	temperatur_soll = argv[1];
	//cout<<"Eingabe Temeperatur SOLL: "<<temperatur_soll<<endl;
//####################################################################



//####################################################################
//config für PI zero Sensoren auslesen
	//Config aus Datei lesen
	//config_file_pfad = "/opt/openhab2/conf/scripts/heizungssteuerung_lokal_via_openhab/pi_zero_sensor.cfg";
	//config_file_pfad = "pi_zero_sensor.cfg";
	//void read_config_file();
	//read_config_file();
	//cout<<"config_file_parameter#"<<config_file_parameter[0]<<endl;
	
//####################################################################
//IST Temp ds18b20 und DHT22


//Externe Sensoren am Pi-2 Zero###
	//Initial  eval `ssh-agent -s` und  ssh-add /root/.ssh/id_ed25519
	//anosnten
	//lsof -a -p $(pgrep ssh-agent) -U -F n | sed -n 's/^n//p' | awk -F " " '{print $1}' > /dev/null
	//export SSH_AUTH_SOCK=$(lsof -a -p $(pgrep ssh-agent) -U -F n | sed -n 's/^n//p' | awk -F " " '{print $1}') 
	//SSH ENV(agent) mit einbeziehen (Systemcall)

	ssh_kommando = "/root/mnt/pi-1/conf/scripts/heizungssteuerung_lokal_via_openhab/read_ds18b20_single 28-01191388fac6";
	ssh_server = "192.168.178.30";
	void ssh_command();
	ssh_command();
	//cout<<"SSH OUTPUT#"<<ssh_kommando_output<<"#"<<endl;
	//cout<<"Externer Sensor 1 (Heizung)#"<<ssh_kommando_output<<"#"<<endl;

	int sensor_1_heizung_int = - 1;
	try{sensor_1_heizung_int = stoi(ssh_kommando_output);}catch( std::invalid_argument& exception ){cout<<"Error wiring_pi_nummer_int stoi std::invalid_argument"<<endl; exit(-1);}
															   catch( std::out_of_range& exception ){cout<<"Error wiring_pi_nummer_int stoi std::out_of_range"<<endl; exit(-1);}
															   catch( std::exception& exception ){cout<<"Error wiring_pi_nummer_int stoi exception"<<endl; exit(-1);} 
	ssh_kommando_output = "";
	
	//#################
	
	ssh_kommando = "/root/mnt/pi-1/conf/scripts/heizungssteuerung_lokal_via_openhab/read_ds18b20_single 28-01191408f33d";
	ssh_server = "192.168.178.30";
	void ssh_command();
	ssh_command();
	//cout<<"SSH OUTPUT#"<<ssh_kommando_output<<"#"<<endl;
	//cout<<"Externer Sensor 2 (Fenster)#"<<ssh_kommando_output<<"#"<<endl;

	int sensor_2_fenster_int = - 1;
	try{sensor_2_fenster_int = stoi(ssh_kommando_output);}catch( std::invalid_argument& exception ){cout<<"Error wiring_pi_nummer_int stoi std::invalid_argument"<<endl; exit(-1);}
															   catch( std::out_of_range& exception ){cout<<"Error wiring_pi_nummer_int stoi std::out_of_range"<<endl; exit(-1);}
															   catch( std::exception& exception ){cout<<"Error wiring_pi_nummer_int stoi exception"<<endl; exit(-1);} 
	ssh_kommando_output = "";

//Lokale Sensoren direkt an Pi-2###

	//Sensor 1 ds18b20 Thermometer# Der an der Heizung
	ds18b20_path_id = "/sys/bus/w1/devices/28-01191401266b/w1_slave";
	void read_ds18b20();
	read_ds18b20();
	//cout<<"Lokaler Sensor 1 (Außenwand Süden)#"<<temperatur_ist<<"#"<<endl;
	int sensor_3_aussenwand_sueden_int = - 1;
	try{sensor_3_aussenwand_sueden_int = stoi(temperatur_ist);}catch( std::invalid_argument& exception ){cout<<"Error wiring_pi_nummer_int stoi std::invalid_argument"<<endl; exit(-1);}
															   catch( std::out_of_range& exception ){cout<<"Error wiring_pi_nummer_int stoi std::out_of_range"<<endl; exit(-1);}
															   catch( std::exception& exception ){cout<<"Error wiring_pi_nummer_int stoi exception"<<endl; exit(-1);} 

	//Sensor 2 ds18b20 Thermometer# Im Raum
	ds18b20_path_id = "/sys/bus/w1/devices/28-0119140a3083/w1_slave";
	void read_ds18b20();
	read_ds18b20();
	//cout<<"Lokaler Sensor 2(Außenwand Osten)#"<<temperatur_ist<<"#"<<endl;
	int sensor_4_aussenwand_osten_int = - 1;
	try{sensor_4_aussenwand_osten_int = stoi(temperatur_ist);}catch( std::invalid_argument& exception ){cout<<"Error wiring_pi_nummer_int stoi std::invalid_argument"<<endl; exit(-1);}
															   catch( std::out_of_range& exception ){cout<<"Error wiring_pi_nummer_int stoi std::out_of_range"<<endl; exit(-1);}
															   catch( std::exception& exception ){cout<<"Error wiring_pi_nummer_int stoi exception"<<endl; exit(-1);} 


	//Sensor 3 DHT22 Feuchtigkeitsmesser# Im Raum
	//cout<<"dht22_falsche_werte#"<<dht22_falsche_werte<<endl;
	//while(dht22_falsche_werte == true)
	do
	{
		//cout<<"dht22_falsche_werte#"<<dht22_falsche_werte<<endl;
		void read_dht22();
		read_dht22();
		//cout<<"dht22_falsche_werte#"<<dht22_falsche_werte<<endl;
		//cout<<"humidity#"<<humidity<<endl;
		//dht22_falsche_werte = false;
		sleep (2);
		//cout<<"dht22_falsche_werte#"<<dht22_falsche_werte<<endl;
	}
	while(dht22_falsche_werte == true);
	//cout<<"humidity#"<<humidity<<endl;
	
	//int sensor_5_humidity_int = - 1;
	//sensor_5_humidity_int = humidity;
	//try{sensor_5_humidity_int = stoi(humidity);}catch( std::invalid_argument& exception ){cout<<"Error wiring_pi_nummer_int stoi std::invalid_argument"<<endl; exit(-1);}
	//														   catch( std::out_of_range& exception ){cout<<"Error wiring_pi_nummer_int stoi std::out_of_range"<<endl; exit(-1);}
	//														   catch( std::exception& exception ){cout<<"Error wiring_pi_nummer_int stoi exception"<<endl; exit(-1);} 




//####################################################################
//Ist Temp durschnitt berechnen 
	//cout<<"sensor_1_heizung_int#"<<sensor_1_heizung_int<<endl;
	//cout<<"sensor_2_fenster_int#"<<sensor_2_fenster_int<<endl;
	//cout<<"sensor_3_aussenwand_sueden_int#"<<sensor_3_aussenwand_sueden_int<<endl;
	//cout<<"sensor_4_aussenwand_osten_int#"<<sensor_4_aussenwand_osten_int<<endl;
	//cout<<"humidity#"<<humidity<<endl;
			
	int durchschnittstemperatur_IST_ankleide_zimmer = -1;
	durchschnittstemperatur_IST_ankleide_zimmer = (sensor_2_fenster_int + sensor_3_aussenwand_sueden_int + sensor_4_aussenwand_osten_int) / 3 ;
	//cout<<"durchschnittstemperatur_IST_ankleide_zimmer#"<<durchschnittstemperatur_IST_ankleide_zimmer<<endl;

//Aktueller GPIO Zustand ermitten und ggf. hoch oder runterdrehen
//GPIO Stellmotor hochdrehen oder runterdrehen
	
	temperatur_soll = temperatur_soll + "000" ;
	//cout<<"temperatur_soll#"<<temperatur_soll<<endl;

	int temperatur_soll_int = - 1;
	try{temperatur_soll_int = stoi(temperatur_soll);}catch( std::invalid_argument& exception ){cout<<"Error wiring_pi_nummer_int stoi std::invalid_argument"<<endl; exit(-1);}
															   catch( std::out_of_range& exception ){cout<<"Error wiring_pi_nummer_int stoi std::out_of_range"<<endl; exit(-1);}
															   catch( std::exception& exception ){cout<<"Error wiring_pi_nummer_int stoi exception"<<endl; exit(-1);} 
	if (temperatur_soll_int > durchschnittstemperatur_IST_ankleide_zimmer)
	{
		//cout<<"ZU KALT"<<endl;
		
		wiring_pi_nummer = "12"; //Ggf. als übergabeparameter ARGV ???
		//void gpio_pruefen();
		//gpio_pruefen();
		//Physical 	| V | Mode | Name    | wPi | BCM |
		//| 32 		| 1 | OUT  | GPIO.26 | 26  | 12  |
		ssh_kommando = "/root/mnt/pi-1/conf/scripts/heizungssteuerung_lokal_via_openhab/gpio_pruefen_single 26";
		ssh_server = "192.168.178.30";
		void ssh_command();
		ssh_command();
		//cout<<"SSH OUTPUT Zustand Heizung#"<<ssh_kommando_output<<"#"<<endl;
	
		//int set_gpio_state = -1

		int ssh_kommando_output_int = - 1;
		try{ssh_kommando_output_int = stoi(ssh_kommando_output);}catch( std::invalid_argument& exception ){cout<<"Error wiring_pi_nummer_int stoi std::invalid_argument"<<endl; exit(-1);}
																 catch( std::out_of_range& exception ){cout<<"Error wiring_pi_nummer_int stoi std::out_of_range"<<endl; exit(-1);}
																 catch( std::exception& exception ){cout<<"Error wiring_pi_nummer_int stoi exception"<<endl; exit(-1);} 
		
		//0 = AUS				   
		if(ssh_kommando_output_int == 0)
		{
			cout<<"ZU KALT, HEIZUNG IST AUS. HEIZUNG ANMACHEN"<<endl;
			ssh_kommando = "/root/mnt/pi-1/conf/scripts/heizungssteuerung_lokal_via_openhab/gpio_stellen_single 26 1";
			ssh_server = "192.168.178.30";
			void ssh_command();
			ssh_command();
			//cout<<"SSH OUTPUT Zustand Heizung#"<<ssh_kommando_output<<"#"<<endl;
		}

		//1 = AN
		if(ssh_kommando_output_int == 1)
		{
			cout<<"ZU KALT, HEIZUNG IST BEREITS AN"<<endl;
			//ssh_kommando = "/root/mnt/pi-1/conf/scripts/heizungssteuerung_lokal_via_openhab/gpio_stellen_single 26 1";
			//ssh_server = "192.168.178.30";
			//void ssh_command();
			//ssh_command();
			//cout<<"SSH OUTPUT Zustand Heizung#"<<ssh_kommando_output<<"#"<<endl;
		}

	}
	
	if (temperatur_soll_int < durchschnittstemperatur_IST_ankleide_zimmer)
	{
		//cout<<"ZU WARM"<<endl;
		
		//Heizungs Status (ist der Stellmotor AN oder AUS ?)
		
		wiring_pi_nummer = "12"; //Ggf. als übergabeparameter ARGV ???
		//void gpio_pruefen();
		//gpio_pruefen();
		//Physical 	| V | Mode | Name    | wPi | BCM |
		//| 32 		| 1 | OUT  | GPIO.26 | 26  | 12  |
		ssh_kommando = "/root/mnt/pi-1/conf/scripts/heizungssteuerung_lokal_via_openhab/gpio_pruefen_single 26";
		ssh_server = "192.168.178.30";
		void ssh_command();
		ssh_command();
		//cout<<"SSH OUTPUT Zustand Heizung#"<<ssh_kommando_output<<"#"<<endl;
		int ssh_kommando_output_int = - 1;
		try{ssh_kommando_output_int = stoi(ssh_kommando_output);}catch( std::invalid_argument& exception ){cout<<"Error wiring_pi_nummer_int stoi std::invalid_argument"<<endl; exit(-1);}
																 catch( std::out_of_range& exception ){cout<<"Error wiring_pi_nummer_int stoi std::out_of_range"<<endl; exit(-1);}
																 catch( std::exception& exception ){cout<<"Error wiring_pi_nummer_int stoi exception"<<endl; exit(-1);} 
		
		//Heizung ausschalten
		//1 = AN
		if(ssh_kommando_output_int == 1)
		{
			cout<<"ZU WARM, HEIZUNG AUSSCHALTEN"<<endl;
			ssh_kommando = "/root/mnt/pi-1/conf/scripts/heizungssteuerung_lokal_via_openhab/gpio_stellen_single 26 0";
			ssh_server = "192.168.178.30";
			void ssh_command();
			ssh_command();
			//cout<<"SSH OUTPUT Zustand Heizung#"<<ssh_kommando_output<<"#"<<endl;
		}
		
		//Heizung ist bereits aus
		//0 = AUS
		if(ssh_kommando_output_int == 0)
		{
			cout<<"ZU WARM, HEIZUNG IST BERITS AUS"<<endl;
		}
		
	}
	
//Humidity zu hoch -> heizen######
	
//####################################################################

//Refernz Temp #Thermo an der heizung


//GPIO 12 für stellmotor
	//Rückfrage auf GPIO statzus (ob an oder aus)

//logging + timestamp ggf. rrdtool

//rückgabe status an openhab 
	
	return (0);
}
