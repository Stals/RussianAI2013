#include "MyStrategy.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include <iostream>

#include "HealHelper.h"
#include "ShootHealper.h"
#include "MovementHelper.h"



using namespace model;
using namespace std;


bool MyStrategy::firstMove = true;

MyStrategy::MyStrategy() { 
	// TODO ����� � ������ ������� ������ ���� - ����� ���� �� � ���������������

}

void MyStrategy::doFirstMoveStuff(const TurnData& turnData)
{
	MovementHelper::chooseFirstTarget(turnData);
}
/*
	TODO �� �������� ������ �������� ���� ��������� ����� ������� ��������
	������ ��� ��� ������� ����� ��� �������

	������ �� ������ �������� ���� ��������� � ����������� �� ������� ������ - ���� ����� �� - ����� ���� � ������� ��������
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

	if(ShootHelper::shoot(turnData)) {
		// ���� �� ���������� ������ �� �������� ���������
		// TODO ���� ������ ���� ������ 1 ����������, ���� �� 1 ��� �� ������ ��������� ���� ��� ����
		useRation(turnData);
		return;
	}

	if(HealHelper::useHeal(turnData, SELF)) return;
	// TODO - check for bonuses

	if(self.getType() == FIELD_MEDIC){
		if(MovementHelper::follow(SOLDIER, turnData)) return;
		else if(MovementHelper::follow(COMMANDER, turnData)) return;
	}else if(self.getType() == COMMANDER){
		if(MovementHelper::follow(SOLDIER, turnData)) return;
	}

	if(MovementHelper::simpleMove(turnData)) return;	
}


bool MyStrategy::useRation(const TurnData& turnData)
{
	if(!turnData.self.isHoldingFieldRation()) return false;
	if(!(turnData.self.getActionPoints() >= turnData.game.getFieldRationEatCost())) return false;

	const int additionalPoints = turnData.game.getFieldRationBonusActionPoints() - turnData.game.getFieldRationEatCost();

	// ������ ���� ������ �� ����� wasted
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
	// TODO ��������� ����� �� ���� �� ���� ����� �� ������
	// range �� ����� ���� 6 �.�. ����� �� ������ ������ ������� �� ������� ��� 50 �����, ��� �����
	// � ������ ����� �� �������� 1 ��� � ����� ������ �� ��� ������� ������� ��� =)

	if(turnData.self.isHoldingGrenade()){
		if (turnData.self.getActionPoints() >= turnData.game.getGrenadeThrowCost()) {  
		
			// if enemy if throwing range
			vector<Trooper> troopers = turnData.world.getTroopers();

			for (size_t i = 0; i < troopers.size(); ++i) {                  
				Trooper trooper = troopers.at(i);
			
				const bool canThrow = Point(trooper).inRadius(Point(turnData.self), turnData.game.getGrenadeThrowRange());
			
				// TODO � ������ ����� ����� �� ������!
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

