#include "Time.h"


// the assignment states that everything should be assumed to be the correct
// format thus it's assumed that the paramater sTime will be of the form
// hh:mmxx  where xx is either am or pm
Time::Time(string sTime){
	this->stringRep = sTime;	
	string hourString = "";
	string minuteString = "";
	hourString += sTime[0];
	hourString += sTime[1];
	minuteString += sTime[3];
	minuteString += sTime[4];
	this->meridiem = "";
	this->meridiem += sTime[5];
	this->meridiem += sTime[6];
	
	this->hour = stoi(hourString);
	this->minute = stoi(minuteString);
	//set hour rank to ease the comparator implementation
	this->hourRank = -1;	
	//cout << "MERIDIEm: " << this->meridiem << endl;	
	if(this->meridiem == "am"){
		if(this->hour == 12){
			this->hourRank = 0;
		}		
		else {
			this->hourRank = this->hour;
		}
	}
	else if(this->meridiem == "pm"){
		if(this->hour == 12){
			this->hourRank = 12;
		}
		else{
			this->hourRank = 0;
			this->hourRank += 12;
			this->hourRank += hour;
		}
	}
	if(hourRank == -1){
		cout << "Problem with hourRank" << endl;
	}		
	
	//cout << stringRep << endl;
	//cout << this->hour << endl;
	//cout << this->minute << endl;
	//cout << this->meridiem << endl;

}

Time::~Time(){

}

string Time::toString(){
	return this->stringRep;

}

bool Time::operator==(Time other){
	bool retVal = false;	
	if(this->stringRep == other.toString())	{
		retVal = true;
	}	
	return retVal;
}

bool Time::operator!=(Time other){
	bool retVal = false;
	if(!(this->operator==(other))){
		retVal = true;
	}
	return retVal;
	
}

bool Time::operator<=(Time other){
	if(this->operator==(other)){
		return true;
	}
	return this->operator<(other);
}


bool Time::operator<(Time other){
	bool retVal = false;
	/*
	if(this->meridiem == "am" && other.meridiem == "pm"){
		return true;
	}
	
	if(this->meridiem == other.meridiem){
		if(this->hour != 12 && other.hour != 12){			
			if(this->hour < other.hour){
				return true;
			}
			if((this->hour == other.hour) && (this->minute < other.minute)){
				return true;
			}
		}
	}
	*/
	if(this->hourRank < other.hourRank){
		retVal = true;
	} else if((this->hourRank == other.hourRank) && (this->minute < other.minute)){
		retVal = true;
	}
	return retVal;
}

















