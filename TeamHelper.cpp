#include "TeamHelper.h"
#include "Logger.h"

#include <complex>

std::vector<model::Trooper> TeamHelper::getTeammates(const model::World& world){
	std::vector<Trooper> troopers = world.getTroopers();
	std::vector<Trooper> teammates;

	for (size_t i = 0; i < troopers.size(); ++i) { 
		Trooper trooper = troopers.at(i);
		
		if(trooper.isTeammate()){
			teammates.push_back(trooper);
		}
	}

	return teammates;		
}

std::vector<model::Trooper> TeamHelper::getTeammatesAround(const model::Trooper& self,  const model::World& world){
	std::vector<Trooper> teammates = getTeammates(world);

	std::vector<Trooper> closeTeammates;
	for (size_t i = 0; i < teammates.size(); ++i) {
		Trooper trooper = teammates.at(i);

		// if not self
		if(self.getTeammateIndex() != trooper.getTeammateIndex()){
			// if in adjecent cells
			if(isAround(self, trooper)){
				closeTeammates.push_back(trooper);
			}
		}	
	}

	return closeTeammates;
}

#include <fstream>
bool TeamHelper::isAround(const model::Trooper& self, const model::Trooper& other)
{

//	Logger::log() << "Self: " << self.getX() << self.getY();
//	Logger::log() << "Other: " << other.getX() << other.getY();
	/*
	std::ofstream myfile;
	myfile.open ("test123.txt", std::ios::out | std::ios::app);
	myfile << "Writing this to a file.\n";*/
	const int deltaX = std::abs(self.getX() - other.getX());
	const int deltaY = std::abs(self.getY() - other.getY());

	// diff on x and y is less then 2
//	Logger::log() << "Result: " << ((deltaX <= 1) && (deltaY <= 1));
	return (deltaX <= 1) && (deltaY <= 1);
}