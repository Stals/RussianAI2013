#include "BonusHelper.h"
#include "MovementHelper.h"

bool BonusHelper::pickupBonus(const TurnData& turnData, double range)
{
	std::vector<Bonus> bonuses = getBonusesInRange(turnData, range);

	if(bonuses.empty()) return false;
	int closestBonusID = -1;
	double lowestDistance = 9999;

	for(size_t i = 0; i < bonuses.size(); ++i){
		Bonus bonus = bonuses[i];
		if(haveBonus(turnData.self, bonus.getType())) continue;
		const double distance = Point(turnData.self).getDistanceTo(Point(bonuses[i]));

		if(distance < lowestDistance){
			lowestDistance = distance;
			closestBonusID = i;
		}
	} 

	if(closestBonusID != -1){
		// TODO при том что true - нужно чтобы он не обходил милион ради него
		return MovementHelper::moveTo(Point(bonuses[closestBonusID]), turnData, true);
	}
	return false;
}

std::vector<Bonus> BonusHelper::getBonusesInRange(const TurnData& turnData, double range)
{
	std::vector<Bonus> bonusesInRange;

	std::vector<Bonus> bonuses = turnData.world.getBonuses();
	for(size_t i = 0; i < bonuses.size(); ++i){
		Bonus bonus = bonuses[i];
		if(Point(turnData.self).inRadius(Point(bonus), range)){
			bonusesInRange.push_back(bonus);
		}
	}

	return bonusesInRange;
}

bool BonusHelper::haveBonus(const model::Trooper& self, BonusType type)
{
	switch(type){
		case GRENADE:
			return self.isHoldingGrenade();
		case MEDIKIT:
			return self.isHoldingMedikit();
		case FIELD_RATION:
			return self.isHoldingFieldRation();
		case UNKNOWN_BONUS:
		default:
			return true;
	}
}