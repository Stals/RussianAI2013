#include "HealHelper.h"	
#include "TeamHelper.h"
#include "Logger.h"

// сколько хп при хиле медкитом может быть потеряно
#define MEDKIT_WASTE 0

// TODO тут много дублирования нужно вынести отдавая только стоимости определенных вещей,
// либо пусть по enum он сам определит стоимости

bool HealHelper::useMedkit(TurnData &turnData, HealTargetType targetType)
{
	if(!turnData.self.isHoldingMedikit()) return false;
	if(!(turnData.self.getActionPoints() >= turnData.game.getMedikitUseCost())) return false;

	if(targetType == SELF){
		if(healingNeeded(turnData.self, turnData.game.getMedikitHealSelfBonusHitpoints())){

			turnData.move.setAction(USE_MEDIKIT); 
			turnData.move.setX(turnData.self.getX());
			turnData.move.setY(turnData.self.getY());
			return true;   
		}
	}else if(OTHERS){

		Trooper trooperToHeal;
		if(!getMostDamagedTeammate(turnData, trooperToHeal)) return false;
				
		if(healingNeeded(trooperToHeal, turnData.game.getMedikitBonusHitpoints())){

			turnData.move.setAction(USE_MEDIKIT); 
			turnData.move.setX(trooperToHeal.getX());
			turnData.move.setY(trooperToHeal.getY());
			return true;
		}					
							
	}	
	
	return false;
}

bool HealHelper::useHeal(TurnData &turnData, HealTargetType targetType)
{
	if(turnData.self.getType() != FIELD_MEDIC) return false;
	if(!(turnData.self.getActionPoints() >= turnData.game.getFieldMedicHealCost())) return false;  
		
	if(targetType == SELF){
		if(healingNeeded(turnData.self, turnData.game.getFieldMedicHealSelfBonusHitpoints())){

			turnData.move.setAction(HEAL); 
			turnData.move.setX(turnData.self.getX());
			turnData.move.setY(turnData.self.getY());
			return true;  
		}

	}else if(OTHERS){

		Trooper trooperToHeal;
		if(!getMostDamagedTeammate(turnData, trooperToHeal)) return false;
		if(healingNeeded(trooperToHeal, turnData.game.getFieldMedicHealBonusHitpoints())){

			turnData.move.setAction(HEAL); 
			turnData.move.setX(trooperToHeal.getX());
			turnData.move.setY(trooperToHeal.getY());
			return true;
		}			
	}

	return false;
}

bool HealHelper::getMostDamagedTeammate(TurnData &turnData, model::Trooper& teammate)
{
	std::vector<model::Trooper> teammates = TeamHelper::getTeammatesAround(turnData.self, turnData.world);
	if(teammates.empty()) return false;


	size_t lowestHpIndex = 0;
	int lowestHP = teammates[0].getHitpoints();
	for(size_t i = 1; i < teammates.size(); ++i){
		if(teammates[i].getHitpoints() < lowestHP){
			lowestHpIndex = i;
			lowestHP = teammates[i].getHitpoints();
		}
	}
	teammate = teammates[lowestHpIndex];
	return true;
}

// TODO функцию neadHealing
// передается туда типок и сколько хилит
// + сколько можно вейстить


bool HealHelper::healingNeeded(const model::Trooper& trooper, int healAmount, int healToWaste)
{
	const int missingHP = (trooper.getMaximalHitpoints() - trooper.getHitpoints());
	return (missingHP + healToWaste) >= healAmount;
}
