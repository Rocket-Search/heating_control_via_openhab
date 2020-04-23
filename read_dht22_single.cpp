//clear; rm -frv read_dht22_single.o; g++ -g -fPIC -Wall read_dht22_single.cpp -o read_dht22_single -lwiringPi

//INO: keine cout oder printf während des Zugriffs auf den Sensor, da der Output auf stdout zu lange dauert und der Sensor flaschen Daten bekommt/sendet
//     keine GPIO abfragen während der Sende und Empfangsphase da auch hier der Sensor sonst durcheinander kommt.
//Fork from https://github.com/danichoi737/DHT22/blob/master/dht22.c

//Manueller Aufruf
//./read_dht22_single 24
// 	|Physical | V | Mode | Name    | wPi | BCM |
//	| 18      | 1 | IN   | GPIO. 5 | 5   | 24  |

//TO DO
//Bei falscher GPIO Nummer Fehler abfangen (PI stürtzt ab)

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
//#include <vector>
//#include <sys/time.h>
//#include <sstream>
//#include <fstream>
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
//#include <cstring>		//strlen
//#include <algorithm>	//string erase
//GPIO wiring PI Header
#include <wiringPi.h>
//#include <libssh/libssh.h>	//SSH 
#include <time.h>
//Für Binär anzeige
#include <bitset>

using namespace std;

//int gpio_port = 24;
short humidity_and_temper[5] = {0, 0, 0, 0, 0};	
float humidity;
bool dht22_falsche_werte = true;;

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

	string gpio_port;
	gpio_port = argv[1];
	
	int wiring_pi_nummer_int = - 1;
	try{wiring_pi_nummer_int = stoi(gpio_port);}catch( std::invalid_argument& exception ){cout<<"Error wiring_pi_nummer_int stoi std::invalid_argument"<<endl; exit(-1);}
													   catch( std::out_of_range& exception ){cout<<"Error wiring_pi_nummer_int stoi std::out_of_range"<<endl; exit(-1);}
													   catch( std::exception& exception ){cout<<"Error wiring_pi_nummer_int stoi exception"<<endl; exit(-1);} 
	
	int wiringPiSetup_api_return_code = 0;
	
	wiringPiSetup_api_return_code = wiringPiSetupGpio();
	
	//cout<<"wiringPiSetup_api_return_code: "<<wiringPiSetup_api_return_code<<endl;
	
	if (wiringPiSetup_api_return_code == -1)
	{	
		cout<<"GPIO wiringPi Error"<<endl;
		exit(-1);
	}
	
	//unsigned short val = 0x00;
	int werte = 0;
	unsigned short signal_laenge = 0;
	unsigned short werte_zaehler = 0;
	unsigned short high_signal_zaehler = 0;
	//float humidity;
	short checksum;
	

	
	//for (unsigned char i = 0; i < 10; i++)
	for (int i = 0; i < 10; i++)
	{
		//cout<<"Runde#"<<i<<endl;
		
		pinMode(wiring_pi_nummer_int, OUTPUT);

		// Send out start signal
		digitalWrite(wiring_pi_nummer_int, LOW);
		delay(20);					// Stay LOW for 5~30 milliseconds
		pinMode(wiring_pi_nummer_int, INPUT);		// 'INPUT' equals 'HIGH' level. And signal read mode
		
		//Daten lesen########################################################################################


		while (1)
		{
			// Count only HIGH signal
			while (digitalRead(wiring_pi_nummer_int) == HIGH)
			{
				signal_laenge++;

				// When sending humidity_and_temper ends, high signal occur infinite.
				// So we have to end this infinite loop.
				if (signal_laenge >= 200)
					{
					//cout<<"signal_laenge>200 #"<<signal_laenge<<endl; 
					//return(-1);
					//ende Haupt while
					//ende_haupt_while = true;
					//cout<<"ende_haupt_while#"<<ende_haupt_while<<endl;
					//continue;
					//break;
					goto ende;
				}

				delayMicroseconds(1);
				
				//cout<<"signal_laenge #"<<signal_laenge<<endl; 
			}
			
			// If signal is HIGH
			if (signal_laenge > 0)
			{
				high_signal_zaehler++;	// HIGH signal counting

				//cout<<"signal_laenge#"<<signal_laenge<<endl;
				//cout<<"high_signal_zaehler#"<<high_signal_zaehler<<endl;
				
				// The DHT22 sends a lot of unstable signals.
				// So extended the counting range.
				if (signal_laenge < 10)
				{
					// Unstable signal
					//cout<<"Unstable signal 1 #"<<signal_laenge<<endl;
					werte <<= 1;		// 0 bit. Just shift left
					//cout<<"werte signal_laenge < 10#"<<werte<<endl;
					
				}

				else if (signal_laenge < 30)
				{
					// 26~28us means 0 bit
					//cout<<"26~28us means 0 bit#"<<signal_laenge<<endl;
					werte <<= 1;		// 0 bit. Just shift left
					//cout<<"werte signal_laenge < 30#"<<werte<<endl;
				}

				else if (signal_laenge < 85)
				{
					// 70us means 1 bit
					// Shift left and input 0x01 using OR operator
					//cout<<"70us means 1 bit#"<<signal_laenge<<endl;
					werte <<= 1;
					//cout<<"werte signal_laenge < 30#"<<werte<<endl;
					werte |= 1;
					//cout<<"werte signal_laenge < 30#"<<werte<<endl;
				}

				else
				{
					// Unstable signal
					cout<<"Kein Stabiles Singnal#"<<endl;
					dht22_falsche_werte = true;
					//exit(-1);
					break;
					
				}

				signal_laenge = 0;	// Initialize signal length for next signal
				werte_zaehler++;		// Count for 8 bit data
			}

			// The first and second signal is DHT22's start signal.
			// So ignore these data.
			if (high_signal_zaehler < 3)
			{
				//werte = 0x00;
				werte = 0;
				werte_zaehler = 0;
				//cout<<"werte high_signal_zaehler < 3#"<<werte<<endl;
			}

			// If 8 bit data input complete
			if (werte_zaehler >= 8)
			{
				// 8 bit data input to the data array
				//cout<<"werte #"<<werte<<endl;
				//cout<<"binär#"<<bitset<16>(werte)<<endl;
				//cout<<"high_signal_zaehler#"<<high_signal_zaehler<<endl;
				
				humidity_and_temper[(high_signal_zaehler / 8) - 1] = werte;		//humidity_and_temper Array [0,1,2,3,4] mit werte füllen. Indexnummer für Array zuweisung
				
				//cout<<"humidity_and_temper#"<<humidity_and_temper<<endl;
				//werte = 0x00;
				werte = 0;				//Variablen für den nächsten durchlauf leeren.
				werte_zaehler = 0;
				
				//cout<<"werte werte_zaehler >= 8#"<<werte<<endl;
				//cout<<"binär#"<<bitset<16>(werte)<<endl;
	
			
			}

		}
//###################################################################################################
ende:
		
		//cout<<"humidity_and_temper[0]#"<<humidity_and_temper[0]<<endl;
		//cout<<"humidity_and_temper[1]#"<<humidity_and_temper[1]<<endl;
		//cout<<"humidity_and_temper[2]#"<<humidity_and_temper[2]<<endl;
		//cout<<"humidity_and_temper[3]#"<<humidity_and_temper[3]<<endl;
		//cout<<"humidity_and_temper[4]#"<<humidity_and_temper[4]<<endl;
		//cout<<"0xFF"<<0xFF<<endl;
		checksum = (humidity_and_temper[0] + humidity_and_temper[1] + humidity_and_temper[2] + humidity_and_temper[3]) & 0xFF;
		//cout<<"checksum#"<<checksum<<endl;
		
		// If Check-sum humidity_and_temper is correct (NOT 0x00), display humidity and temperature
		if (humidity_and_temper[4] == checksum && checksum != 0x00)
		{
			humidity = ((humidity_and_temper[0] * 256) + humidity_and_temper[1]) / 10.0;
			cout<<"humidity#"<<humidity<<endl;
			//cout<<humidity<<endl;
			//return(0);
			dht22_falsche_werte = false;
			break;
		}
		else
		{
			//cout<<"dht22_falsche_werte#"<<dht22_falsche_werte<<endl;
			//cout<<"falsche Werte empfangen"<<endl;
			dht22_falsche_werte = true;
			cout<<"dht22_falsche_werte#"<<dht22_falsche_werte<<endl;
			//exit(-1);
			//continue;
			break;
						
		}
	}

	return(0);
}