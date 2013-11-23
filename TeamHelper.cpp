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

 std::vector<model::Trooper> TeamHelper::getTeammatesInRadius(double radius, const TurnData& turnData)
 {
	std::vector<model::Trooper> inRadius;
	std::vector<model::Trooper> teammates = getTeammates(turnData.world);
	for(size_t i = 0; i < teammates.size(); ++i){
		Trooper trooper = teammates[i];

		if(turnData.self.getTeammateIndex() != trooper.getTeammateIndex()){
			
			if(Point(turnData.self).inRadius(trooper, radius)){
				inRadius.push_back(trooper);
			}
		}
	}
	return inRadius;
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

bool TeamHelper::getTeammateToHeal(const TurnData& turnData, model::Trooper& teammateToHeal)
{
	double minDistance = 9999;
	int teammateID = -1;

	// нужно кто ближе наверное
	std::vector<model::Trooper> teammates = getTeammates(turnData.world);
	for(size_t i = 0; i < teammates.size(); ++i){
		Trooper trooper = teammates[i];
		// if not self
		if(turnData.self.getTeammateIndex() != trooper.getTeammateIndex()){
			const double distance = Point(turnData.self).getDistanceTo(trooper);
			const bool notFullHP = trooper.getHitpoints() < trooper.getMaximalHitpoints();
			if( notFullHP && (distance < minDistance)){
				minDistance = distance;
				teammateID = i;
			}
		}
	}

	if(teammateID != -1){
		teammateToHeal = teammates[teammateID];
		return true;
	}

	return false;
}

bool TeamHelper::getTeammateWithType(TrooperType type, model::Trooper& teammate, const model::World& world)
{
	std::vector<model::Trooper> teammates = TeamHelper::getTeammates(world);

	for(size_t i = 0; i < teammates.size(); ++i){
		Trooper trooper = teammates[i];

		if(trooper.getType() == type){
			teammate = trooper;
			return true;
		}
	}

	return false;
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

