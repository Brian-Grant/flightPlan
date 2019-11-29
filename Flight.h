#include <string>
#include "Time.h"


using namespace std;

class City;

class Flight {
	public:
		Flight(string depCity, string destCity, string depTime, string arrTime, float cost);
		~Flight();
		bool operator<(Flight other);
		bool operator!=(Flight other);
	//private:
		string depCity;
		string destCity;
		
		float cost;
		
		Time* depTime;
		Time* arrTime;
	
		City* depObj;
		City* destObj;

};
