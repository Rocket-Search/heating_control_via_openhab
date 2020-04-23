//clear; rm -frv read_ds18b20_single.o; g++ -g -fPIC -Wall read_ds18b20_single.cpp -o read_ds18b20_single

//C_C++ Header
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <cstring>
//algorithm fürs string klein machen
#include <algorithm>
//Zum Casten von Char Hex Array nach string (bei md5sum)
#include <iomanip>
#include <iostream>
#include <vector>
#include <sys/time.h>
#include <sstream>
#include <fstream>
#include <pthread.h>
#include <thread>
#include <time.h>
#include <errno.h>
#include <signal.h>

using namespace std;

//Public Variablen (für Object Files)

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
	//cout<<"READ DS18B20"<<endl;
	//cout<<"\e[1m"<<"read_ds18b20.cpp"<<"\e[0m"<<endl;
	
	//cout<<"ds18b20_path_id"<<ds18b20_path_id<<endl;
	
	//Temperatur auslesen
//while(true)
//{
		// Temepratur vom Sensor via sysFS auslesen 
		//cat /sys/bus/w1/devices/28-030597790d9b/w1_slave 
		
		//prüfen auf 2 stellen, werte zwischen 15 und 45 grad nur zahlen etc.
		
		string ds18b20_path_id;
		string temperatur_ist;


		ds18b20_path_id = argv[1];
		ds18b20_path_id = "/sys/bus/w1/devices/" + ds18b20_path_id + "/w1_slave";
		//cout<<"ds18b20_path_id#"<<ds18b20_path_id<<endl;
		
		ifstream ifs (ds18b20_path_id.c_str(), ifstream::in);
		char temp_rohdaten_char = ifs.get();
		string rohdaten_string_1, rohdaten_string_2;

		while (ifs.good()) 
		{
			//cout<<temp_rohdaten_char;
			//cout<<"temp_rohdaten_char#"<<temp_rohdaten_char<<endl;
			temp_rohdaten_char = ifs.get();
			rohdaten_string_1 = temp_rohdaten_char;
			rohdaten_string_2.append(rohdaten_string_1);
		}

		ifs.close();
		

		//cout<<"rohdaten_string_2#"<<rohdaten_string_2<<endl;
		
		//Temeperaturdaten filtern##################################
		//string temperatur_ist;	//Public
		int wo_ist_t_gleich = -1 ; 

		wo_ist_t_gleich = rohdaten_string_2.find("t=");
		
		//cout<<"wo_ist_t_gleich#"<<wo_ist_t_gleich<<"#"<<endl;
		//cout<<"wo_ist_endl#"<<wo_ist_endl<<"#"<<endl;
		if (wo_ist_t_gleich > 0)
		{
			//try{temp = rohdaten_string_2.substr(wo_ist_t_gleich,rohdaten_string_2.length());}catch( std::out_of_range& exception ){cout<<"Error rohdaten_string_2 local_block std::out_of_range"<<endl;}
			try{temperatur_ist = rohdaten_string_2.substr(wo_ist_t_gleich + 2,rohdaten_string_2.length());}catch( std::out_of_range& exception ){cout<<"Error rohdaten_string_2 local_block std::out_of_range"<<endl;}
			//try{temp = rohdaten_string_2.substr(2 + wo_ist_t_gleich,5);}catch( std::out_of_range& exception ){cout<<"Error rohdaten_string_2 local_block std::out_of_range"<<endl;}
			//try{temp = rohdaten_string_2.substr(2 + wo_ist_t_gleich,14);}catch( std::out_of_range& exception ){cout<<"Error rohdaten_string_2 local_block std::out_of_range"<<endl;}
			//try{temp = rohdaten_string_2.substr(wo_ist_t_gleich + 2, wo_ist_endl - 2);}catch( std::out_of_range& exception ){cout<<"Error rohdaten_string_2 local_block std::out_of_range"<<endl;}
		}
		
		//cout<<"temperatur_ist#"<<temperatur_ist<<"#"<<endl;

		int wo_ist_endl = -1 ; 
		wo_ist_endl = temperatur_ist.find("\n");
		//cout<<"wo_ist_endl#"<<wo_ist_endl<<"#"<<endl;
		if (wo_ist_endl > 0)
		{
			try{temperatur_ist = temperatur_ist.substr(0,wo_ist_endl);}catch( std::out_of_range& exception ){cout<<"Error temperatur_ist local_block std::out_of_range"<<endl;}
		}
		
		//cout<<"temperatur_ist #"<<temperatur_ist<<"#"<<endl;
		cout<<temperatur_ist<<endl;
//}
		//####################################
/*		
		//Beim Eingabeparameter (temperatur_soll = argv[1];) den Punkt wegmachen.
		char chars2[] = ".";
		for (unsigned int j = 0; j < strlen(chars2); ++j)
		{
			temperatur_soll.erase(std::remove(temperatur_soll.begin(), temperatur_soll.end(), chars2[j]), temperatur_soll.end());
		}
		
		//cout<<"temperatur_soll#"<<temperatur_soll<<"#"<<endl;
*/
	//####################################################################

	return (0);
}
	
	