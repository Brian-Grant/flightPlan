#include "City.h"
#include "Flight.h"

City::City(){

}

City::City(string name){
	this->name = name;
	this->color = 0;
	this->predecessor = nullptr;
	this->arrivalTime = nullptr;
	this->price = 0.0;
	this->flightToGetHereIdx = -1;
}
	
City::~City(){
	
}

