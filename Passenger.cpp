#include "Passenger.h"

Passenger::Passenger(){
	//cout << "Departure date: ";
	//cin >> depDate;	
	cout << endl << "Departure city: ";
	cin >> depCity;
	cout << endl << "Destination city: ";
	cin >> destCity;
	//cout << endl << "Return Date: ";
	//cin >> retDate;
	cout << endl << "Earliest accepted departure time: ";
	cin >> earliestDep;
	cout << endl << "Earliest accepted return departure time: ";
	cin >> earliestRet;
	cout << endl;

	this->leaveTime = new Time(earliestDep);
	this->returnTime = new Time(earliestRet);
	
/*
	
	
	
	

	retDate = constraintArray[4];
	earliestRet = constraintArray[5];
*/
}

Passenger::~Passenger(){
	delete this->leaveTime;
	delete this->returnTime;
}
