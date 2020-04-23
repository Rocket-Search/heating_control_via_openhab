//clear; rm -frv ssh_command.o ;g++ -g -fPIC -Wall -c ssh_command.cpp 

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
//#include <algorithm>	//string erase
//GPIO wiring PI Header
//#include <wiringPi.h>
//#include <fcntl.h>		//write file descriptor
#include <libssh/libssh.h>	//SSH 

using namespace std;

extern string ssh_kommando;
extern string ssh_kommando_output;
extern string ssh_server;

void ssh_command()
	{
		//cout<<"ssh_command"<<endl;
		//cout<<"ssh"<<endl;
		
				//libssh inistialiseren####
		ssh_session my_ssh_session = ssh_new();
		
		if (my_ssh_session == NULL)
		{
			cout<<"Problem my_ssh_session#"<<my_ssh_session<<endl;
			exit(-1);
		}
		//cout<<"my_ssh_session#"<<my_ssh_session<<endl;
		
		//openhab_server = "localhost";
		//libssh Optionen##
		//int verbosity = SSH_LOG_PROTOCOL;
		int verbosity = SSH_LOG_NOLOG;
		int port = 22;
		ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, ssh_server.c_str());
		ssh_options_set(my_ssh_session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
		ssh_options_set(my_ssh_session, SSH_OPTIONS_PORT, &port);

		//libssh connect zu server###
		int rc;
		rc = ssh_connect(my_ssh_session);
		if (rc != SSH_OK)
		{
			//fprintf(stderr, "Error connecting to localhost: %s\n",ssh_get_error(my_ssh_session));
			cout<<"ssh_connect rc#"<<rc<<"#"<<endl;
			exit(-1);
		}
		//cout<<"ssh_connect rc#"<<rc<<endl;
		
		//libssh authentification####
//		enum ssh_known_hosts_e state;
		unsigned char *hash = NULL;
		ssh_key srv_pubkey = NULL;
		size_t hlen;
		//char buf[10];
		//char *hexa;
		//char *p;
		//int cmp;
		
		rc = ssh_get_server_publickey(my_ssh_session, &srv_pubkey);
		if (rc < 0) 
		{
			//return -1;
			cout<<"ssh_get_server_publickey rc#"<<rc<<"#"<<endl;
			exit(-1);
		}
		//cout<<"ssh_get_server_publickey rc#"<<rc<<endl; 
		
		rc = ssh_get_publickey_hash(srv_pubkey,SSH_PUBLICKEY_HASH_SHA1,&hash,&hlen);
		ssh_key_free(srv_pubkey);
		if (rc < 0)
		{
			//return -1;
			cout<<"ssh_get_publickey_hash rc#"<<rc<<"#"<<endl;
			exit(-1);
		}		
		//cout<<"ssh_get_publickey_hash rc#"<<rc<<endl; 
/*		
state = ssh_session_is_known_server(my_ssh_session);
if (state != 1)
{
	cout<<"ssh_session_is_known_server state#"<<state<<endl;
	exit(-1);
}
*/
		//cout<<"ssh_session_is_known_server state#"<<state<<endl;

		/*
		char *password;
		password = getpass("Password: ");
		rc = ssh_userauth_password(my_ssh_session, NULL, password);
		if (rc != SSH_AUTH_SUCCESS)
		{
			fprintf(stderr, "Error authenticating with password: %s\n",	ssh_get_error(my_ssh_session));
			ssh_disconnect(my_ssh_session);
			ssh_free(my_ssh_session);
			exit(-1);
		}
		cout<<"ssh_userauth_password rc#"<<rc<<endl; 
		*/
	
		//Public Keys auth##
		rc = ssh_userauth_publickey_auto(my_ssh_session, NULL, NULL);
		if (rc == SSH_AUTH_ERROR)
		{
			//fprintf(stderr, "Authentication failed: %s\n",ssh_get_error(my_ssh_session));
			//return SSH_AUTH_ERROR;
			cout<<"ssh_userauth_publickey_auto rc#"<<rc<<"#"<<endl;
			exit(-1);
		}
		//cout<<"ssh_userauth_publickey_auto rc#"<<rc<<endl; 
		
		//Kommando ausführen##
		ssh_channel channel;
		channel = ssh_channel_new(my_ssh_session);
		
		rc = ssh_channel_open_session(channel);
		//cout<<"ssh_channel_open_session rc#"<<rc<<endl; 
		
		//ssh_kommando = "/bin/date";
		//ssh_kommando = " curl -X POST --header \"Content-Type: text/plain\" --header \"Accept: application/json\" -d \"ON\" \"http://172.17.190.89:8080/rest/items/exec_command_licht_an_aus_wohnzimmer_input\" ";
		//cout<<"ssh_kommando#"<<ssh_kommando<<"#"<<endl;
		const char * ssh_kommando_char = ssh_kommando.c_str();
		//rc = ssh_channel_request_exec(channel, "/bin/date");
		rc = ssh_channel_request_exec(channel, ssh_kommando_char);
		//cout<<"ssh_channel_request_exec rc#"<<rc<<endl; 
		if (rc != SSH_OK)
		{
			ssh_channel_close(channel);
			ssh_channel_free(channel);
			//return rc;
			cout<<"ssh_channel_request_exec rc#"<<rc<<"#"<<endl;
			exit(-1);
		}
		//cout<<"ssh_channel_request_exec rc#"<<rc<<endl; 
		
		char buffer[256];
		int nbytes;
		nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
		
		string tempertaur_temp;
		//cout<<"nbytes#"<<nbytes<<"#"<<endl;

		//string temperatur_ist;
		//extern string ssh_kommando_output;
		while (nbytes > 0)
		{
			//if (write(1, buffer, nbytes) != (unsigned int) nbytes)
			//{
			//	ssh_channel_close(channel);
			//	ssh_channel_free(channel);
			//	return SSH_ERROR;
			//}
			nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
			//cout<<"buffer#"<<buffer<<"#"<<endl;
			tempertaur_temp = buffer;
			//cout<<"tempertaur_temp#"<<tempertaur_temp<<"#"<<endl;
			ssh_kommando_output.append(tempertaur_temp);
			//cout<<"temperatur_ist#"<<temperatur_ist<<"#"<<endl;
		}
		
		//cout<<"SSH OUTPUT#"<<ssh_kommando_output<<"#"<<endl;
		
		/*
		cout<<"nbytes#"<<nbytes<<"#"<<endl;
		if (nbytes < 0)
		{
			ssh_channel_close(channel);
			ssh_channel_free(channel);
			//return SSH_ERROR;
			//exit(-1);
			cout<<"nbytes < 0 #"<<nbytes<<"#"<<endl;
		}
		*/
		ssh_channel_send_eof(channel);
		ssh_channel_close(channel);
		ssh_channel_free(channel);
		
		ssh_disconnect(my_ssh_session);
		ssh_free(my_ssh_session);
		
		nbytes = 0;
		
		//Filter Output
		int wo_ist_newline = -1;
		wo_ist_newline = ssh_kommando_output.find("\n");
		//cout<<"wo_ist_newline#"<<wo_ist_newline<<endl;
		//cout<<"VORHER SSH OUTPUT#"<<ssh_kommando_output<<"#"<<endl;
		string temp = ssh_kommando_output;
		if (wo_ist_newline > 0)
		{
			//try{temp = rohdaten_string_2.substr(wo_ist_t_gleich,rohdaten_string_2.length());}catch( std::out_of_range& exception ){cout<<"Error rohdaten_string_2 local_block std::out_of_range"<<endl;}
			try{temp = ssh_kommando_output.substr(0,wo_ist_newline);}catch( std::out_of_range& exception ){cout<<"Error wo_ist_newline std::out_of_range"<<endl;}
			//try{temp = rohdaten_string_2.substr(2 + wo_ist_t_gleich,5);}catch( std::out_of_range& exception ){cout<<"Error rohdaten_string_2 local_block std::out_of_range"<<endl;}
			//try{temp = rohdaten_string_2.substr(2 + wo_ist_t_gleich,14);}catch( std::out_of_range& exception ){cout<<"Error rohdaten_string_2 local_block std::out_of_range"<<endl;}
			//try{temp = rohdaten_string_2.substr(wo_ist_t_gleich + 2, wo_ist_endl - 2);}catch( std::out_of_range& exception ){cout<<"Error rohdaten_string_2 local_block std::out_of_range"<<endl;}
		}
		//cout<<"VORHER SSH OUTPUT#"<<temp<<"#"<<endl;
		ssh_kommando_output = temp;
		//return;
	}
	