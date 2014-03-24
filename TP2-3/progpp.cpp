#include <iostream>
#include <sstream>
#include <unordered_map>
#include "eth_lib.h"

using namespace std;

struct triplet
{
	triplet() : destination(""), source(""), type("")
	{}

	triplet(char* d, char* s, char* t) : destination(d), source(s), type(t)
	{}

	string destination;
	string source;
	string type;
};

bool operator==(triplet const& t1, triplet const& t2)
{
	return true;
}

int hexaToInt(string hexa)
{
	int a;
	stringstream ss;
	ss << std::hex << hexa;
	ss >> a;

	return a;
}

string charToHexa(char a)
{
	string hexa;
	
	stringstream ss;
	ss << std::hex << a;
	ss >> hexa;
	
	return hexa;
}

void recupHexa(char* res, int taille)
{
	string lecture;
	int a;

	for(int i = 0; i < taille; i++)
	{
		cin >> lecture;
		res[i] = hexaToInt(lecture);
	}

}

struct eth_frame lectureTrame()
{
	int taille; 
	string lecture;
	char* destination;
	char* source;
	char* type;
	char* donnee;
	
	cin >> lecture;
	cin >> lecture;
	cin >> lecture; 
	//Byte rcv :

	cin >> taille;

	struct eth_frame frame;
	recupHexa(frame.adr_dest, 6);
	recupHexa(frame.adr_send, 6);
	recupHexa(frame.type, 2);
	recupHexa(frame.data, taille-6-6-2);

	return frame;
}

int main()
{
	unordered_map<struct triplet, int> cpt;	

	struct eth_frame frame = lectureTrame();

	triplet t;
	t.destination = frame.adr_dest;
	t.source = frame.adr_source;
	t.type = frame.type;

	if(cpt.count(t) > 0)
		cpt[t]++;
	else
		cpt[t] = 1;

	cout << static_cast<int>(frame.adr_dest[1]);

	return 0;
}
