#include <string>
#include "Passenger.h"
#include <vector>
//#include "Flight.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <queue>
#include <algorithm>
//#include "City.h"

using namespace std;

class FDS {
	public:
		FDS(string scheduleFile);
		~FDS();
		
					
		class DFSnode{
			public:			
				DFSnode();
				~DFSnode();
			
				string prevCity;								
				string city;
				string nextCity;
				string arrTime;
				
				string nextCityDepTime;
				string nextCityArrTime;
				float nextFlightCost; // new   //TODO incorporate nextFlightCost in "cheapest"				
				float price; // cumulative cost at that city
				
				DFSnode* next;

				// should make a reverse LL method
				// should create a to string --which is called by the FDS obj by printItinerary()
				
		};
		
		void findAny(Passenger* passenger);
		void goHomeAny(Passenger* passenger);		

		void findEarliest(Passenger* passenger);
		void goHomeEarliest(Passenger* passenger);
		
		void findCheapest(Passenger* passenger);
		void goHomeCheapest(Passenger* passenger);
		void DFSvisit(City* city, Passenger* passenger);
		
		bool inCityVect(string city);		
	
		// should create printItinerary()
		
		
		vector<Flight*> flightSchedule;
		vector<Flight*> unorderedFlightSchedule;
		
		vector<City*> cityVect;
		queue<City*> bfsQueue;
		vector<DFSnode*> allPaths;
		vector<DFSnode*> allPathsHome;
};
