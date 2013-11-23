#include "MyStrategy.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include <iostream>

#include "HealHelper.h"
#include "ShootHealper.h"
#include "MovementHelper.h"
#include "TeamHelper.h"
#include "BonusHelper.h"



using namespace model;
using namespace std;


bool MyStrategy::firstMove = true;
TrooperIndex MyStrategy::currentEnemyID = TrooperIndex::noIndex();

MyStrategy::MyStrategy() { 
	// TODO нужно в начале выбрать первую цель - чтобы идти не в противоположный

}

void MyStrategy::doFirstMoveStuff(const TurnData& turnData)
{
	MovementHelper::chooseFirstTarget(turnData);
}
/*
	TODO за движение должен отвечать прям отдельный класс который движется
	Потому что это большой кусок мне кажется

	причем он должен выбирать куда двигаться в зависимости от текущей задачи - типо нужно хп - нужно идти к медпаку например
*/
void MyStrategy::move(const Trooper& self, const World& world, const Game& game, Move& move) {
	TurnData turnData(self, world, game, move);

	// if first turn
	if(firstMove){
		firstMove = false;
		doFirstMoveStuff(turnData);
	}

	
	if(useGrenade(turnData)) return;
	if(HealHelper::useMedkit(turnData, OTHERS)) return;
	if(HealHelper::useMedkit(turnData, SELF)) return;
	if(HealHelper::useHeal(turnData, OTHERS)) return;

	// TODO выбирать цель, по тому что видно (среди видимых), а не по тем что можно стрелять
	if(self.getType() != FIELD_MEDIC){

		// if previous enemy is not in sight of anyone - remove him
		Trooper enemyTrooper;
		if(currentEnemyID != TrooperIndex::noIndex()){
			if(!TeamHelper::getTrooperByUniqueIndex(currentEnemyID, enemyTrooper, world)){
				currentEnemyID = TrooperIndex::noIndex();
			}
		}

		if(currentEnemyID == TrooperIndex::noIndex()){
			if(ShootHelper::getTarget(turnData, enemyTrooper)){
				if(ShootHelper::shoot(turnData, enemyTrooper)){
					currentEnemyID = TrooperIndex(enemyTrooper);
					return;
				}//else{
					// if( moveTo(enemyTrooper)) return;
				//}
			}
		}

		else{ // have curent target
			if(ShootHelper::shoot(turnData, enemyTrooper)){ // try shooting him
				useRation(turnData);
				return;
			}else{
				if(ShootHelper::shoot(turnData)){	//if(enemies in sight){
					useRation(turnData);			//  pick one and shoot him}
					return;
				}else{
					if(MovementHelper::moveTo(Point(enemyTrooper), turnData, true))
						return;
				}
			}
	
		}

	}else{
		if(self.getHitpoints() > 50){ // тк как иначе пусть себя хилит
			Trooper teammateToHeal;
			if(TeamHelper::getTeammateToHeal(turnData, teammateToHeal)){
				if(MovementHelper::moveTo(Point(teammateToHeal), turnData, false))
					return;
			}
		}
	}


	// TODO не забывать есть рацион
	// если несколько противников - он цель должен выбирать как shoot
	// - тоесть shoot ее должен вернуть чтобы я id сохранил!!
	// тупо по Shoot нельзя, потому что некоторые типы видят дальше чем могут стрелять - например медик
	// соответствено цель должен уметь ставить даже медик?
	// по нему палят например если?

	// TODO уметь цель ставить не через shoot?
	// типо есть метод выбора цели. 
	// он возвращает цель которую видно, но нужно же еще стрелять цель которую ты сам можешь выстрелить - range!


	if(ShootHelper::shoot(turnData)) {
		// Если не получилось съесть он всеравно выстрелит
		// TODO есть только если больше 1 противника, либо за 1 раз не убиваю полностью того кто есть
		useRation(turnData);
		return;
	}

	if(HealHelper::useHeal(turnData, SELF)) return;

	// if sees pickup and dont have one - go for it
	if(BonusHelper::pickupBonus(turnData, 3)) return;

	// follow
	if(self.getType() == FIELD_MEDIC){
		if(MovementHelper::follow(SOLDIER, turnData)) return;
		else if(MovementHelper::follow(COMMANDER, turnData)) return;
	}else if(self.getType() == COMMANDER){
		if(MovementHelper::follow(SOLDIER, turnData)) return;
	}


	// TODO только если остальные типочки вокруг тебя
	if(TeamHelper::teammatesInRadius(5, turnData)){
		if(MovementHelper::simpleMove(turnData)) return;
	}
}


bool MyStrategy::useRation(const TurnData& turnData)
{
	if(!turnData.self.isHoldingFieldRation()) return false;
	if(!(turnData.self.getActionPoints() >= turnData.game.getFieldRationEatCost())) return false;

	const int additionalPoints = turnData.game.getFieldRationBonusActionPoints() - turnData.game.getFieldRationEatCost();

	// только если ничгео не будет wasted
	if((turnData.self.getActionPoints() + additionalPoints) <= turnData.self.getInitialActionPoints()){
		turnData.move.setAction(EAT_FIELD_RATION);
		turnData.move.setX(turnData.self.getX());
		turnData.move.setY(turnData.self.getY());
		return true;
	}

	return false;
}

bool MyStrategy::useGrenade(const TurnData& turnData)
{
	// TODO проверять чтобы ни меня ни моих типов не задело
	// range на самом деле 6 т.к. можно по меньше дамага нанести но всетаки это 50 вроде, что круто
	// в теории нужно бы походить 1 раз и потом киунть но это сильные тактики уже =)

	if(turnData.self.isHoldingGrenade()){
		if (turnData.self.getActionPoints() >= turnData.game.getGrenadeThrowCost()) {  
		
			// if enemy if throwing range
			vector<Trooper> troopers = turnData.world.getTroopers();

			for (size_t i = 0; i < troopers.size(); ++i) {                  
				Trooper trooper = troopers.at(i);
			
				const bool canThrow = Point(trooper).inRadius(Point(turnData.self), turnData.game.getGrenadeThrowRange());
			
				// TODO и кидать чтобы своих не задело!
				if(canThrow && !trooper.isTeammate()){
				    turnData.move.setAction(THROW_GRENADE);
					turnData.move.setX(trooper.getX());
					turnData.move.setY(trooper.getY());
					return true;					
				}
			}
		}
	}
	return false;
}

