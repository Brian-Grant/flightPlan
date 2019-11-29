#include "Flight.h"
#include "City.h"

Flight::Flight(string depCity, string destCity, string depTime, string arrTime, float cost){
	this->depCity = depCity;
	this->destCity = destCity;
	
	this->depTime = new Time(depTime);
	this->arrTime = new Time(arrTime);

	this->cost = cost;
	
	this->depObj = nullptr;
	this->destObj = nullptr;
	
}

Flight::~Flight(){
	// delete the time objs
	delete this->depTime;
	delete this->arrTime;
}


bool Flight::operator<(Flight other){
	return this->arrTime < other.arrTime;
}

bool Flight::operator!=(Flight other){
	return this->arrTime != other.arrTime;
}
