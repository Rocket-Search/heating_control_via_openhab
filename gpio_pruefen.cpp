//clear; rm -frv gpio_pruefen.o ;g++ -g -fPIC -Wall -c gpio_pruefen.cpp 

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

extern string wiring_pi_nummer;

void extern gpio_pruefen()
{
	
	//cout<<"gpio_pruefen"<<endl;
	
	int wiringPiSetup_api_return_code = 0;
	
	//wiringPiSetup_api_return_code = wiringPiSetupGpio();
	wiringPiSetup_api_return_code = wiringPiSetup();
	
			
		if (wiringPiSetup_api_return_code == -1)
		{	
			cout<<"GPIO wiringPi Error"<<endl;
			exit(-1);
		}
	
	int wiring_pi_nummer_int = - 1;
	try{wiring_pi_nummer_int = stoi(wiring_pi_nummer);}catch( std::invalid_argument& exception ){cout<<"Error wiring_pi_nummer_int stoi std::invalid_argument"<<endl; exit(-1);}
													   catch( std::out_of_range& exception ){cout<<"Error wiring_pi_nummer_int stoi std::out_of_range"<<endl; exit(-1);}
													   catch( std::exception& exception ){cout<<"Error wiring_pi_nummer_int stoi exception"<<endl; exit(-1);} 
	
	cout<<"wiring_pi_nummer_int#"<<wiring_pi_nummer_int<<endl;
	
	int gpio_zustand = - 1 ;
	gpio_zustand = digitalRead(wiring_pi_nummer_int); 									   
	cout<<"Heizung Zustand#"<<gpio_zustand<<endl;
	
}