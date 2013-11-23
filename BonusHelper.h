#pragma once

#include "Point.h"
#include "TurnData.h"

class BonusHelper{
public:
	// подбирает ближайший к игроку бонус(которого у него нет) в радиусе range
	static bool pickupBonus(const TurnData& turnData, double range);
	// Возвращает бонусы вокруг игрока в радиусе range
	static std::vector<Bonus> getBonusesInRange(const TurnData& turnData, double range);
	// возвращает true если у игрока есть бонус типа type
	static bool haveBonus(const model::Trooper& self, BonusType type);

private:
	BonusHelper();
};
