//clear; rm -frv gpio_stellen_single.o; g++ -g -fPIC -Wall gpio_stellen_single.cpp -o gpio_stellen_single -lwiringPi

// ./gpio_stellen_single 26 1
//Physical 	| V | Mode | Name    | wPi | BCM |
//| 32 		| 1 | OUT  | GPIO.26 | 26  | 12  |

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
	
	
	//cout<<"gpio_pruefen"<<endl;
	
	string gpio_port;
	gpio_port = argv[1];
	
	string gpio_zustand;
	gpio_zustand = argv[2];
	
	int wiringPiSetup_api_return_code = 0;
	
	//wiringPiSetup_api_return_code = wiringPiSetupGpio();
	wiringPiSetup_api_return_code = wiringPiSetup();
	
			
	if (wiringPiSetup_api_return_code == -1)
	{	
		cout<<"GPIO wiringPi Error"<<endl;
		exit(-1);
	}
	
	int wiring_pi_nummer_int = - 1;
	try{wiring_pi_nummer_int = stoi(gpio_port);}catch( std::invalid_argument& exception ){cout<<"Error wiring_pi_nummer_int stoi std::invalid_argument"<<endl; exit(-1);}
												catch( std::out_of_range& exception ){cout<<"Error wiring_pi_nummer_int stoi std::out_of_range"<<endl; exit(-1);}
												catch( std::exception& exception ){cout<<"Error wiring_pi_nummer_int stoi exception"<<endl; exit(-1);} 
	
	int gpio_zustand_int = - 1;
	try{gpio_zustand_int = stoi(gpio_zustand);}catch( std::invalid_argument& exception ){cout<<"Error wiring_pi_nummer_int stoi std::invalid_argument"<<endl; exit(-1);}
											   catch( std::out_of_range& exception ){cout<<"Error wiring_pi_nummer_int stoi std::out_of_range"<<endl; exit(-1);}
											   catch( std::exception& exception ){cout<<"Error wiring_pi_nummer_int stoi exception"<<endl; exit(-1);} 
						
						
	pinMode(wiring_pi_nummer_int, OUTPUT);							   
	digitalWrite(wiring_pi_nummer_int, gpio_zustand_int);
	
	
	return(0);
	
}