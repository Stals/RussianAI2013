#pragma once

#include "Strategy.h"
#include "TurnData.h"
#include <vector>

// stores unique trooper index
struct TrooperIndex{
	TrooperIndex():playerIndex(-1), teammateIndex(-1){}

	TrooperIndex(const model::Trooper& trooper):
		playerIndex(trooper.getPlayerId()), teammateIndex(trooper.getTeammateIndex()){}

	 bool operator==(const TrooperIndex &other) const {
		 if(teammateIndex == other.teammateIndex){
			 return playerIndex == other.playerIndex;
		 }
		 return false;
	 }

	 bool operator!=(const TrooperIndex &other) const {
		 return !(*this == other);
	 }

	long long playerIndex;
	int teammateIndex;


	static TrooperIndex noIndex(){ return TrooperIndex();}
};

class TeamHelper{
public:
	static std::vector<model::Trooper> getTeammates(const model::World& world);
	static std::vector<model::Trooper> getTeammatesAround(const model::Trooper& self,  const model::World& world);

	static bool isAround(const model::Trooper& self, const model::Trooper& other); 

	// Возвращает true если все живые находятся в радиусе radius
	static bool teammatesInRadius(double radius, const TurnData& turnData);

	// методы общие и для других типочков, возврващает в trooper
	// returns true если получилось найти
	static bool getTrooperByUniqueIndex(TrooperIndex index, model::Trooper& trooper, const model::World& world);

};
