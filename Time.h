#include <string>
#include <iostream>

using namespace std;

class Time{
	public:
		Time(string sTime);
		~Time();
		string toString();
		bool operator==(Time other);
		bool operator<(Time other);
		bool operator!=(Time other);
		bool operator<=(Time other);


	//private:
		string meridiem;
		int hour;
		int minute;
		string stringRep;
		int hourRank;

};
