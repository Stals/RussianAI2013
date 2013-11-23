#include "TeamHelper.h"
#include "Logger.h"

#include <complex>
#include "Point.h"

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

bool TeamHelper::teammatesInRadius(double radius, const TurnData& turnData)
{
	bool allInRange = true;
	// Note: пробегаем и по себе потому что лень
	std::vector<model::Trooper> teammates = getTeammates(turnData.world);
		for(size_t i = 0; i < teammates.size(); ++i){
		Trooper trooper = teammates[i];
		allInRange &= Point(turnData.self).inRadius(trooper, radius);
	}

	return allInRange;
}


bool TeamHelper::getTrooperByUniqueIndex(TrooperIndex index, model::Trooper& trooper, const model::World& world)
{
	std::vector<Trooper> troopers = world.getTroopers();

	for (size_t i = 0; i < troopers.size(); ++i) { 
		Trooper t = troopers.at(i);
		if(index == TrooperIndex(t)){
			trooper = t;
			return true;
		}
	}

	return false;
}

