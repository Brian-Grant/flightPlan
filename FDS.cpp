#include "FDS.h"

#include "City.h"

FDS::FDS(string scheduleFile){
	string depCity;
	string destCity;
	string depTime;
	string arrTime;
	string cost;	
	float costFl;
	
	string line;	
	//get all flights and store in vect
	ifstream input(scheduleFile);
	if(input.is_open()){
		while(getline(input, line)){
			istringstream buff(line);
			buff >> depCity;
			buff >> destCity;
			buff >> depTime;
			buff >> arrTime;
			buff >> cost;			
			cost.erase(0,1);
			costFl = stof(cost);
			Flight * fl = new Flight(depCity, destCity, depTime, arrTime, costFl);
			this->unorderedFlightSchedule.push_back(fl);
		}
	}
	input.close();
	// put all cities in a vector
	for(unsigned int i = 0; i < unorderedFlightSchedule.size(); i++){
		string dep = unorderedFlightSchedule[i]->depCity;		
		string dest = unorderedFlightSchedule[i]->destCity;
		if(!this->inCityVect(dep)){
			City* tmpCity1 = new City(dep);			
			this->cityVect.push_back(tmpCity1);
		}
		if(!this->inCityVect(dest)){
			City* tmpCity2 = new City(dest);			
			this->cityVect.push_back(tmpCity2);
		}
	}
	// populate each city obj with outgoing flights
	unsigned int flightCount = 0;	
	for(unsigned int i = 0; i < cityVect.size(); i++){
		for(unsigned int j = 0; j < unorderedFlightSchedule.size(); j++){
			if(cityVect[i]->name == unorderedFlightSchedule[j]->depCity){
				this->cityVect[i]->outgoingFlights.push_back(unorderedFlightSchedule[j]); 
				flightCount++;
			}
		}		
	}
	// check to make sure that all flights were accounted for
	if(unorderedFlightSchedule.size() != flightCount){
		cout << "problem populating the outgoing flights in the city objs" << endl;
	}
	
	// populate each flight with departure and dest city objs....maybe just dest
	// objs because the flights will be held in a dep city obj
	// (ended up populating both)
	for(unsigned int i = 0; i < cityVect.size(); i++){
		for(unsigned int j = 0; j < cityVect[i]->outgoingFlights.size(); j++){
			for(unsigned int k = 0; k < cityVect.size(); k++){
				if(cityVect[i]->outgoingFlights[j]->depCity == cityVect[k]->name){
					cityVect[i]->outgoingFlights[j]->depObj = cityVect[k];
				}
				if(cityVect[i]->outgoingFlights[j]->destCity == cityVect[k]->name){
					cityVect[i]->outgoingFlights[j]->destObj = cityVect[k];
				}				
			}
		}
	}
	// order the outgoing flights in order of arrival time at dest City
	int maxTimeIndex = -1;
	Time maxTime = Time("12:00am");
	Flight* maxTimeFlight;	
	for(unsigned int i = 0; i < cityVect.size(); i++){
		int upperBound = 0;		
		for(unsigned int j = 0; j < cityVect[i]->outgoingFlights.size()-upperBound;){
			for(unsigned int k = 0; k < cityVect[i]->outgoingFlights.size() - upperBound; k++){			
				if(maxTime < *cityVect[i]->outgoingFlights[k]->arrTime){
					maxTime = *cityVect[i]->outgoingFlights[k]->arrTime;
					maxTimeIndex = k;
					maxTimeFlight = cityVect[i]->outgoingFlights[k];
				}
			}
			cityVect[i]->outgoingFlights.erase(cityVect[i]->outgoingFlights.begin() +	maxTimeIndex);
			cityVect[i]->outgoingFlights.insert((cityVect[i]->outgoingFlights.end() - upperBound), maxTimeFlight);
			maxTime = Time("12:00am");		
			upperBound++;	
		}
	}
}

FDS::~FDS(){
	for(unsigned int i = 0; i < this->unorderedFlightSchedule.size(); i++){
		delete this->unorderedFlightSchedule[i];
	}
	for(unsigned int i = 0; i < this->cityVect.size(); i++){
		delete this->cityVect[i];
	}
}

void FDS::findAny(Passenger* passenger){
	int cityIndex = 0;	
	// reset color, and predecessors	
	for(unsigned int i = 0; i < this->cityVect.size(); i++){
		cityVect[i]->color = 0;
		cityVect[i]->predecessor = nullptr;
		
		// get departure city index
		if(cityVect[i]->name == passenger->depCity){
			cityIndex = i;
		}
	}
		
	cityVect[cityIndex]->color = 1;	
	cityVect[cityIndex]->arrivalTime = passenger->leaveTime;

	bfsQueue.push(cityVect[cityIndex]);
	City* frontier;	
	
	int count = 0;
	while(bfsQueue.size() != 0){
		count++;		
		frontier = bfsQueue.front();
		bfsQueue.pop();
		for(unsigned int i = 0; i < frontier->outgoingFlights.size(); i++){			
			if((frontier->outgoingFlights[i]->destObj->color == 0)){ 
				frontier->outgoingFlights[i]->destObj->color = 1;		
				frontier->outgoingFlights[i]->destObj->predecessor = frontier;						
				frontier->outgoingFlights[i]->destObj->arrivalTime = frontier->outgoingFlights[i]->arrTime;			
				frontier->DFSflightTakenDepTime = frontier->outgoingFlights[i]->depTime->toString();
				frontier->DFSflightTakenArrTime = frontier->outgoingFlights[i]->arrTime->toString();		
				frontier->DFSflightTakenCost = frontier->outgoingFlights[i]->cost;			
				bfsQueue.push(frontier->outgoingFlights[i]->destObj);
			}	
		}
		frontier->color = 3;
	}
	int finalDestIdx = 0;	
	for(unsigned int i = 0; i < cityVect.size(); i++){
		if(passenger->destCity == cityVect[i]->name){
			finalDestIdx = i;
			break;
		}	
	}
	
	//cout << "final dest name: " << cityVect[finalDestIdx]->name << endl;
	//cout << "final dest color: " << cityVect[finalDestIdx]->color << endl;
	City* tmp = cityVect[finalDestIdx];

	vector<City*> path;
	
	while(tmp != nullptr){
		path.push_back(tmp);
		tmp = tmp->predecessor;
	}
	cout << "---------------------------TRIP---------------------------------" << endl;
	if(path.size() == 1){
		cout << "No flight path possible" << endl;
	}
	else{
		for(int i = (path.size() - 1); i >= 0; i--){
			cout << "\t" << path[i]->name << endl;
		}
	}

	
	this->goHomeAny(passenger);
	//cout << "DUN DUN DUN------------------------------------------------------" << endl;
}

void FDS::goHomeAny(Passenger* passenger){
	cout << "-----------------------RETURN TRIP------------------------------" << endl;	
	int cityIndex = 0;	
	// reset color, and predecessors	
	for(unsigned int i = 0; i < this->cityVect.size(); i++){
		cityVect[i]->color = 0;
		cityVect[i]->predecessor = nullptr;
		
		// get departure city index
		if(cityVect[i]->name == passenger->destCity){
			cityIndex = i;
		}
	}
	cityVect[cityIndex]->color = 1;	
	cityVect[cityIndex]->arrivalTime = passenger->returnTime;
	
	bfsQueue.push(cityVect[cityIndex]);
	City* frontier;	
	
	int count = 0;
	while(bfsQueue.size() != 0){
		count++;		
		frontier = bfsQueue.front();
		bfsQueue.pop();
		for(unsigned int i = 0; i < frontier->outgoingFlights.size(); i++){			
			if((frontier->outgoingFlights[i]->destObj->color == 0)){ 
				frontier->outgoingFlights[i]->destObj->color = 1;		
				frontier->outgoingFlights[i]->destObj->predecessor = frontier;						
				frontier->outgoingFlights[i]->destObj->arrivalTime = frontier->outgoingFlights[i]->arrTime;			
				frontier->DFSflightTakenDepTime = frontier->outgoingFlights[i]->depTime->toString();
				frontier->DFSflightTakenArrTime = frontier->outgoingFlights[i]->arrTime->toString();		
				frontier->DFSflightTakenCost = frontier->outgoingFlights[i]->cost;			
				bfsQueue.push(frontier->outgoingFlights[i]->destObj);
			}	
		}
		frontier->color = 3;
	}
	int finalDestIdx = 0;	
	for(unsigned int i = 0; i < cityVect.size(); i++){
		if(passenger->depCity == cityVect[i]->name){
			finalDestIdx = i;
			break;
		}	
	}
	
	//cout << "final dest name: " << cityVect[finalDestIdx]->name << endl;
	//cout << "final dest color: " << cityVect[finalDestIdx]->color << endl;
	City* tmp = cityVect[finalDestIdx];

	vector<City*> path;
	
	while(tmp != nullptr){
		path.push_back(tmp);
		tmp = tmp->predecessor;
	}
	
	if(path.size() == 1){
		cout << "No flight path possible" << endl;
	}
	else{
		for(int i = (path.size() - 1); i >= 0; i--){
			cout << "\t" << path[i]->name << endl;
		}
	}
	cout << endl;
}


void FDS::findEarliest(Passenger* passenger){
	int cityIndex = 0;	
	// reset color, and predecessors	
	for(unsigned int i = 0; i < this->cityVect.size(); i++){
		cityVect[i]->color = 0;
		cityVect[i]->predecessor = nullptr;
		// get departure city index
		if(cityVect[i]->name == passenger->depCity){
			cityIndex = i;
		}
	}
	cityVect[cityIndex]->color = 1;	
	cityVect[cityIndex]->arrivalTime = passenger->leaveTime;
	bfsQueue.push(cityVect[cityIndex]);
	City* frontier;	
	int count = 0;
	while(bfsQueue.size() != 0){
		count++;		
		frontier = bfsQueue.front();
		bfsQueue.pop();
		for(unsigned int i = 0; i < frontier->outgoingFlights.size(); i++){			
			if((frontier->outgoingFlights[i]->destObj->color == 0) && (*frontier->arrivalTime <= *frontier->outgoingFlights[i]->depTime)){
				frontier->outgoingFlights[i]->destObj->color = 1;		
				frontier->outgoingFlights[i]->destObj->predecessor = frontier;		
				frontier->outgoingFlights[i]->destObj->flightToGetHereIdx = i;				
				frontier->outgoingFlights[i]->destObj->arrivalTime = frontier->outgoingFlights[i]->arrTime;			
				frontier->DFSflightTakenDepTime = frontier->outgoingFlights[i]->depTime->toString();
				frontier->DFSflightTakenArrTime = frontier->outgoingFlights[i]->arrTime->toString();		
				frontier->DFSflightTakenCost = frontier->outgoingFlights[i]->cost;			
				//frontier->price = city->price + city->outgoingFlights[i]->cost; //new				
				bfsQueue.push(frontier->outgoingFlights[i]->destObj);
			}	
			else if((frontier->outgoingFlights[i]->destObj->color == 1) && (*frontier->outgoingFlights[i]->arrTime < *frontier->outgoingFlights[i]->destObj->arrivalTime)){
				frontier->outgoingFlights[i]->destObj->predecessor = frontier;		
				frontier->outgoingFlights[i]->destObj->arrivalTime = frontier->outgoingFlights[i]->arrTime;
				frontier->DFSflightTakenDepTime = frontier->outgoingFlights[i]->depTime->toString();
				frontier->DFSflightTakenArrTime = frontier->outgoingFlights[i]->arrTime->toString();		
				frontier->DFSflightTakenCost = frontier->outgoingFlights[i]->cost;			
				//frontier->price = city->price + city->outgoingFlights[i]->cost; //new								
				bfsQueue.push(frontier->outgoingFlights[i]->destObj);
			}
		}
		frontier->color = 3;
	}
	int finalDestIdx = 0;	
	for(unsigned int i = 0; i < cityVect.size(); i++){
		if(passenger->destCity == cityVect[i]->name){
			finalDestIdx = i;
			break;
		}	
	}
	
//	cout << "final dest name: " << cityVect[finalDestIdx]->name << endl;
//	cout << "final dest color: " << cityVect[finalDestIdx]->color << endl;
	
	City* tmp = cityVect[finalDestIdx];
	vector<City*> path;	
	while(tmp != nullptr){
		path.push_back(tmp);
		tmp = tmp->predecessor;
	}
	cout << "---------------------------TRIP---------------------------------" << endl;

	if(path.size() == 1){
		cout << "No flight path possible" << endl;
	}
	else{
		for(int i = (path.size() - 1); i >= 0; i--){
			cout << "\t" << path[i]->name << endl;
		}
		cout << "Arrived at destination at " << cityVect[finalDestIdx]->arrivalTime->toString() << endl;
	}

	
	
	this->goHomeEarliest(passenger);
	//cout << "DUN DUN DUN---------------------------------------------------" << endl;
}


void FDS::goHomeEarliest(Passenger* passenger){
	cout << "-----------------------RETURN TRIP------------------------------" << endl;

	int cityIndex = 0;	
	// reset color, and predecessors	
	for(unsigned int i = 0; i < this->cityVect.size(); i++){
		cityVect[i]->color = 0;
		cityVect[i]->predecessor = nullptr;
		// get departure city index
		if(cityVect[i]->name == passenger->destCity){ // change from depCity to destCity
			cityIndex = i;
		}
	}
	cityVect[cityIndex]->color = 1;	
	cityVect[cityIndex]->arrivalTime = passenger->returnTime; // change from leaveTime to returnTime
	bfsQueue.push(cityVect[cityIndex]);
	City* frontier;	
	int count = 0;
	while(bfsQueue.size() != 0){
		count++;		
		frontier = bfsQueue.front();
		bfsQueue.pop();
		for(unsigned int i = 0; i < frontier->outgoingFlights.size(); i++){			
			if((frontier->outgoingFlights[i]->destObj->color == 0) && (*frontier->arrivalTime <= *frontier->outgoingFlights[i]->depTime)){
				frontier->outgoingFlights[i]->destObj->color = 1;		
				frontier->outgoingFlights[i]->destObj->predecessor = frontier;		
				frontier->outgoingFlights[i]->destObj->arrivalTime = frontier->outgoingFlights[i]->arrTime;			
				bfsQueue.push(frontier->outgoingFlights[i]->destObj);
			}	
			else if((frontier->outgoingFlights[i]->destObj->color == 1) && (*frontier->outgoingFlights[i]->arrTime < *frontier->outgoingFlights[i]->destObj->arrivalTime)){
				frontier->outgoingFlights[i]->destObj->predecessor = frontier;		
				frontier->outgoingFlights[i]->destObj->arrivalTime = frontier->outgoingFlights[i]->arrTime;
				bfsQueue.push(frontier->outgoingFlights[i]->destObj);
			}
		}
		frontier->color = 3;
	}
	int finalDestIdx = 0;	
	for(unsigned int i = 0; i < cityVect.size(); i++){
		if(passenger->depCity == cityVect[i]->name){ 
			finalDestIdx = i;
			break;
		}	
	}
	
//	cout << "final dest name: " << cityVect[finalDestIdx]->name << endl;
//	cout << "final dest color: " << cityVect[finalDestIdx]->color << endl;
	City* tmp = cityVect[finalDestIdx];
	vector<City*> path;	
	while(tmp != nullptr){
		path.push_back(tmp);
		tmp = tmp->predecessor;
	}
	
	if(path.size() == 1){
		cout << "No flight path possible" << endl;
	}
	else{
		for(int i = (path.size() - 1); i >= 0; i--){
			cout << "\t" << path[i]->name << endl;
		}
		cout << "Arrived at destination at " << cityVect[finalDestIdx]->arrivalTime->toString() << endl;
	}

	cout << endl;
}

//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

void FDS::findCheapest(Passenger* passenger){
	int cityIndex = 0;	
	// reset color, and predecessors	
	for(unsigned int i = 0; i < this->cityVect.size(); i++){
		cityVect[i]->color = 0;
		cityVect[i]->predecessor = nullptr;
		cityVect[i]->price = 0.0;
		// get departure city index
		if(cityVect[i]->name == passenger->depCity){
			cityIndex = i;
		}
	}
	cityVect[cityIndex]->color = 1;	
	cityVect[cityIndex]->arrivalTime = passenger->leaveTime;
	//price is already set to 0.0
	cout << "dep arr time: " << cityVect[cityIndex]->arrivalTime->toString() << endl;
	City* firstCity = cityVect[cityIndex];
	this->DFSvisit(firstCity, passenger);
	cout << "_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-" << endl;
	cout << "here" << endl;
	//City* finalDest;
	int finalDestIdx = 0;	
	for(unsigned int i = 0; i < cityVect.size(); i++){		
		if(passenger->destCity == cityVect[i]->name){
			//finalDest = cityVect[i];
			finalDestIdx = i;
			break;
		}	
	}
	cout << "final dest name: " << cityVect[finalDestIdx]->name << endl;
	cout << "final dest color: " << cityVect[finalDestIdx]->color << endl;
	cout << "final dest price: " << cityVect[finalDestIdx]->price << endl;	
	City* tmp = cityVect[finalDestIdx];
	while(tmp->predecessor != nullptr){
		cout << tmp->name << endl;
		tmp = tmp->predecessor;
	}
	cout << tmp->name << endl;
	cout << "NOW GO HOME" << endl;
	cout << "AlpathTests" << endl;
	cout << "allPaths size: " << this->allPaths.size() << endl;

	for(unsigned int j = 0; j < this->allPaths.size(); j++){	
		cout << "\t" << j << endl;	
		DFSnode* tmmp = this->allPaths[j];	
				
		while(tmmp != nullptr){
			cout << "city " << tmmp->city << "\t" << "tmmp->nextCityDepTime " << tmmp->nextCityDepTime << endl;
			cout << "price " << tmmp->price << endl;			
			tmmp = tmmp->next;
		}	
	}
	cout << "_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-" << endl;
}

void FDS::DFSvisit(City* city, Passenger* passenger){
	cout <<"\t\t\t\t\t\tVisit city: " << city->name << endl;
	city->color = 1;
	bool madeIt = false;
	for(unsigned int i = 0; i < city->outgoingFlights.size(); i++){
		//cout << "\t" << city->outgoingFlights[i]->destObj->name << endl;	
		//cout << "\t" << city->outgoingFlights[i]->depTime->toString() << endl;	
		//cout << "city->outgoingFlights.size() " << city->outgoingFlights.size() << endl;		
		cout << "CONSIDERED" << endl;
		cout << "destObj->name " << city->outgoingFlights[i]->destObj->name << endl;		
		//cout << "city->outgoingFlights[i]->destObj->color " << city->outgoingFlights[i]->destObj->color << endl;	
		//cout << "city->arrivalTime " << city->arrivalTime->toString() << endl;
		if(((city->outgoingFlights[i]->destObj->color == 0) && (*city->arrivalTime <= *city->outgoingFlights[i]->depTime)) || ((city->outgoingFlights[i]->destObj->color != 0) && (*city->arrivalTime <= *city->outgoingFlights[i]->depTime) && (city->outgoingFlights[i]->destObj->name != passenger->depCity) && (city->name != city->outgoingFlights[i]->destObj->predecessor->name))){
			cout << "TAKEN" << endl;	
			cout << "\t" << city->outgoingFlights[i]->destObj->name << endl;	
			cout << "\t" << city->outgoingFlights[i]->depTime->toString() << endl;	
			cout << "city->outgoingFlights.size() " << city->outgoingFlights.size() << endl;		
			cout << "city->outgoingFlights[i]->destObj->name " << city->outgoingFlights[i]->destObj->name << endl;		
			cout << "city->outgoingFlights[i]->destObj->color " << city->outgoingFlights[i]->destObj->color << endl;	
			cout << "city->arrivalTime " << city->arrivalTime->toString() << endl;		
			City* frontier = city->outgoingFlights[i]->destObj;
			frontier->predecessor = city;
			frontier->price = city->price + city->outgoingFlights[i]->cost;
			frontier->arrivalTime = city->outgoingFlights[i]->arrTime;
			city->DFSflightTakenDepTime = city->outgoingFlights[i]->depTime->toString();
			city->DFSflightTakenArrTime = city->outgoingFlights[i]->arrTime->toString();
			//------------------------------------------------------------------------------------------------
			//	if we are at dest add it to vect and add all predecessors
			City* tmp;
			if(frontier->name == passenger->destCity){
				madeIt = true;				
				cout << "made it from dep to dest" << endl;				
				City* countTmp = frontier;
				int count = 0;				
				while(countTmp->predecessor != nullptr){
					count++;
					countTmp = countTmp->predecessor;
				}
				cout << "Xcount " << count << endl;			
				DFSnode* dest = new DFSnode();
				dest->city = frontier->name;
				dest->prevCity = frontier->predecessor->name;
				dest->nextCity = "NONE";
				dest->arrTime = frontier->arrivalTime->toString();
				dest->nextCityDepTime = "NONE";
				dest->nextCityArrTime = "NONE";
				dest->price = frontier->price;					
				dest->next = nullptr;				
	
				// for each predececcor make a node and push it to path
				// then push path to all path
				// then reset the node features;
				tmp = frontier;
				City* tmpPrev;
				DFSnode* tmpDFSnode = dest;				
				while(tmp->predecessor != nullptr){
					cout << "XX " << tmp->name << endl;			
					tmpPrev = tmp->predecessor;
					DFSnode* node = new DFSnode();
					tmpDFSnode->next = node;
					node->city = tmpPrev->name;
					if(tmpPrev->predecessor != nullptr){					
						node->prevCity = tmpPrev->predecessor->name;
					}
					else{
						node->prevCity = "NONE";
					}					
					node->nextCity = tmp->name;
					node->arrTime = tmpPrev->arrivalTime->toString();
					node->nextCityDepTime = tmpPrev->DFSflightTakenDepTime;
					node->nextCityArrTime = tmpPrev->DFSflightTakenArrTime;	
					node->price = tmp->predecessor->price;	
					node->next = nullptr;
					tmpDFSnode = node;				
					tmp = tmpPrev;
				}
				cout << "XXX " << tmp->name << endl;
				this->allPaths.push_back(dest);
				// reset the path
				
				//tmp = frontier; //uncomment this and the next two lines
				//tmp->color = 0;
				//tmp->price = 0.0;
				//tmp->arrivalTime = new Time("12:00am");			 ///	
/*				
				tmpPrev = tmp->predecessor;
				tmp->predecessor = nullptr;
				while(tmpPrev != nullptr){
					tmp = tmpPrev;
					//tmp->arrivalTime = new Time("12:00am");			 ///	
					tmp->color = 0;
					tmp->price = 0.0;
					tmpPrev = tmp->predecessor;
					tmp->predecessor = nullptr;
				}
				cout << "XOXO " << tmp->name << endl;
				//tmp->color = 1;
				tmp->arrivalTime = passenger->leaveTime;
*/
//*/
			}
			//--------------------------------------------------------------------------------------------------------
			if(!madeIt){			
				this->DFSvisit(frontier, passenger);
			} else{
			}
		}
	}
	if(!madeIt){
		city->color = 2;
	}
	else{
	
	}
}

void goHomeCheapest(Passenger* passenger){

}

bool FDS::inCityVect(string city){
	bool retVal = false;	
	for(unsigned int i = 0; i < this->cityVect.size(); i++){
		if(this->cityVect[i]->name == city){
			retVal = true;
			break;
		}
	}
	return retVal;
}


FDS::DFSnode::DFSnode(){

}

FDS::DFSnode::~DFSnode(){

}


