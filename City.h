#include <string>
#include <vector>
//#include "Time.h"
using namespace std;

class Time;
class Flight;

class City{
	public:
		City();
		City(string name);
		
		~City();
		string name;
		int color;
		
		int flightToGetHereIdx;
		float price;		
		
		City* predecessor;
		Time* arrivalTime;
		vector<Flight*> outgoingFlights;
		
		string DFSflightTakenDepTime;
		string DFSflightTakenArrTime;
		
		
		float DFSflightTakenCost;
};
