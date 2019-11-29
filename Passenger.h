#include <string>
#include <iostream>
#include "Flight.h"

using namespace std;

class Passenger {
	public:
		Passenger();
		~Passenger();
	//private:
		string depCity;
		string destCity;
		string depDate;
		string earliestDep;

		string retDate;
		string earliestRet;

		Time* leaveTime;
		Time* returnTime;
	
};
