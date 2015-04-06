#ifndef BASE_H
#define BASE_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <linux/sockios.h>
#include <math.h>
#include <map>
#include <string>
#include <iostream>
#include <stdlib.h>

using namespace std;

int defaultPORT=8672;
int socketFD,n;
int chatSocketFD, heartBeatSocketFD, electionSocketFD, sequencerSocketFD;
struct sockaddr_in joinClientAddress, clientAddress, selfAddress;
char msg[1000];
char response[1000];

struct participant									//holds the data for one participant
{
	struct sockaddr_in address;
	int seqNumber;
	string username;
};

std::map <string, struct participant * > participantList;					//map of key - IP:PORT value - participant struct
std::map <string, struct participant * >::iterator participantListIterator; 	//iterator for the participant list
	
bool compareParticipants()					//TODO - fill this comparison function for the map
{
	bool result=false;
	
	return result;
}

struct participant * createParticipant(struct sockaddr_in address, int seqNumber, string name)		//create a participant based on raw data
{
	struct participant * participant = new struct participant;
	participant->address=address;
	participant->seqNumber=seqNumber;
	participant->username=name;
	return participant;
}


in_port_t getPort(struct sockaddr *address)					//get port number in raw format
{
	if (address->sa_family == AF_INET) 
	{
        return (((struct sockaddr_in*)address)->sin_port);
    }
    return (((struct sockaddr_in6*)address)->sin6_port);
}

int getPort(struct participant *participant)				//get port number in integer format
{
	int port=ntohs(getPort((struct sockaddr*)&participant->address));
	return port;
}

char * getIP(struct participant *participant)				//get IP from participant
{
	char *ip=new char[INET_ADDRSTRLEN];
	
	if(inet_ntop(AF_INET,&(participant->address.sin_addr),ip, INET_ADDRSTRLEN)==NULL)
	{
		cout<<"Error in inet_ntop\n";
	}
	return ip;
}

char * getIP(struct sockaddr_in address)				//get IP from address data structure
{
	char *ip=new char[INET_ADDRSTRLEN];
	
	if(inet_ntop(AF_INET,&(address.sin_addr),ip, INET_ADDRSTRLEN)==NULL)
	{
		cout<<"Error in inet_ntop\n";
	}
	return ip;
}

char *createKey(struct sockaddr_in address)
{
	char *ip=new char[INET_ADDRSTRLEN];
	if(inet_ntop(AF_INET,&(address.sin_addr),ip, INET_ADDRSTRLEN)==NULL)
	{
		cout<<"Error in inet_ntop\n";
	}
	char *key=new char[INET_ADDRSTRLEN+6];
	snprintf(key,INET_ADDRSTRLEN+6,"%s:%d",ip,ntohs(getPort((struct sockaddr*)&address)));
	return key;
}
void printParticipant(struct participant *participant)
{
	
	//cout<<"-------------------------------\n";
	char *seq=new char[10];
	snprintf(seq,10,"%d",participant->seqNumber);
	cout<<participant->username<<endl<<getIP(participant)<<":"<<getPort(participant)<<endl<<seq;
	//cout<<"-------------------------------\n";
}
void printParticipantList()
{
	cout<<"-------------------------------\n";
	for(participantListIterator=participantList.begin(); participantListIterator!=participantList.end();participantListIterator++)
	{
		cout<<participantListIterator->first<<endl;
		printParticipant(participantListIterator->second);
		cout<<endl;
	}
	cout<<"-------------------------------\n";
}

char * serializeParticipant(struct participant *participant)
{
	char *result=new char[1000];
	char *seq=new char[10];
	snprintf(seq,10,"%d",participant->seqNumber);
	strcat(result,createKey(participant->address));
	strcat(result,":");
	strcat(result, seq);
	strcat(result,":");
	strcat(result,(participant->username).c_str());
	//cout<<result<<endl;
	return result;
}

#endif