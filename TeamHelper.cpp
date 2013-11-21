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

bool TeamHelper::isAround(const model::Trooper& self, const model::Trooper& other)
{
	const int deltaX = std::abs(self.getX() - other.getX());
	const int deltaY = std::abs(self.getY() - other.getY());

	// diff on x and y is less then 2
	// ((deltaX - deltaY) != 0) - check not diagonal
	return ((deltaX - deltaY) != 0) && (deltaX <= 1) && (deltaY <= 1);
}