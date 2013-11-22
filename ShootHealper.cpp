#include "ShootHealper.h"


bool ShootHelper::shoot(TurnData& turnData)
{

	if (!(turnData.self.getActionPoints() >= turnData.self.getShootCost())) return false;
	std::vector<Trooper> troopers = turnData.world.getTroopers(); 
	
	// 1) ѕровер€ем может кого-то успеем убить
	for (size_t i = 0; i < troopers.size(); ++i) {
        Trooper trooper = troopers.at(i);
		if(!trooper.isTeammate() && isVisible(turnData.world, turnData.self, trooper)){
			
			// сколько раз сможет выстрелить
			const int possibleShots = turnData.self.getActionPoints() / turnData.self.getShootCost();
			const int possibleDMG = possibleShots * turnData.self.getDamage();

			if(possibleDMG >= trooper.getHitpoints()){
			    turnData.move.setAction(SHOOT);                       
                turnData.move.setX(trooper.getX());                       
                turnData.move.setY(trooper.getY()); 
				return true;
			}			
		}
	}	
	
	// 2) ѕровер€ем можем ли стрел€ть в медика
	// TODO в медика с меньшим хп
	for (size_t i = 0; i < troopers.size(); ++i) {
		Trooper trooper = troopers.at(i);
		if(!trooper.isTeammate() && isVisible(turnData.world, turnData.self, trooper)){
			if(trooper.getType() == FIELD_MEDIC){
				turnData.move.setAction(SHOOT);                       
				turnData.move.setX(trooper.getX());                       
				turnData.move.setY(trooper.getY()); 
				return true;
			}
		}
	}	
	
	// 3) —трел€ем в цель с наименьшим здоровьем
	int lowestHP = 999;
	int lowestHP_id = -1;
	for (size_t i = 0; i < troopers.size(); ++i) {
		Trooper trooper = troopers.at(i);
		if(!trooper.isTeammate() && isVisible(turnData.world, turnData.self, trooper)){
			if(trooper.getHitpoints() < lowestHP){
				lowestHP = trooper.getHitpoints();
				lowestHP_id = i;
			}
		}
	}
	// если  не осталс€ -1 то стрел€ем
	if(lowestHP_id != -1){
		Trooper trooper = troopers.at(lowestHP_id);
		turnData.move.setAction(SHOOT);                       
		turnData.move.setX(trooper.getX());                       
		turnData.move.setY(trooper.getY()); 
		return true;
	}


	return false;
}


bool ShootHelper::isVisible(const model::World& world, const model::Trooper& self, const model::Trooper& target)
{
	return world.isVisible(self.getShootingRange(),    
            self.getX(), self.getY(), self.getStance(),            
            target.getX(), target.getY(), target.getStance());
}