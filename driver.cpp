#include "driver.h"

using namespace std;

int main(int argc, char **argv){

	if(argc != 3){
		cout << "Must be executed with two command line arguments" << endl;
		cout << "as: ./FDS <schedule.txt> <constraint>" << endl;
		return -1;
	}
	
	string scheduleFileName;
	string constraint;
	
	// grab the goods 
	istringstream buf1(argv[1]);
	buf1 >> scheduleFileName;	
	istringstream buf2(argv[2]);
	buf2 >> constraint;

	FDS* fds = new FDS(scheduleFileName);
	Passenger* p = new Passenger();
	//cout << constraint << endl;

	if(constraint == "any"){
		fds->findAny(p);
	}
	else if(constraint == "earliest"){
		fds->findEarliest(p);
	}
	else if(constraint == "cheapest"){
		fds->findCheapest(p);
	}

	delete p;
	delete fds;
	return 0;
}
