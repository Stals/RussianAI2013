#include "MyStrategy.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include <iostream>

#include "HealHelper.h"
#include "ShootHealper.h"
#include "Astar.h"


using namespace model;
using namespace std;

Corner MyStrategy::currentTarget = TopRight;

MyStrategy::MyStrategy() { 
	// TODO ����� � ������ ������� ������ ���� - ����� ���� �� � ���������������
}


/*
	TODO �� �������� ������ �������� ���� ��������� ����� ������� ��������
	������ ��� ��� ������� ����� ��� �������

	������ �� ������ �������� ���� ��������� � ����������� �� ������� ������ - ���� ����� �� - ����� ���� � ������� ��������
*/
void MyStrategy::move(const Trooper& self, const World& world, const Game& game, Move& move) {
	TurnData turnData(self, world, game, move);

	//!!!!!// TODO �������� heal others � heal self ��� ����� �� ����������� ����� ���� �������� 
	
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
	if(simpleMove(self, world, game, move)) return;
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



bool MyStrategy::simpleMove(const Trooper& self, const World& world, const Game& game, Move& move){
	// TODO ���������� ���� �� ���� �����, ��� ������ ������� ���������� ������ ���� � ���� ����

	if (self.getActionPoints() >= game.getStandingMoveCost()) {         // ���� ���������� ����� ��������
        vector<vector<CellType> > cells = world.getCells();             // �������� ����� ����������� �� ������� ����

		Point targetPoint = chooseTarget(self, world);
		
		// fill map
		for(int y = 0; y < m; y++){
			for(int x = 0; x < n; x++){
				if(cells[x][y] == FREE){
					map[x][y] = 0;
				}else{
					map[x][y] = 1;
				}
			}
		}

		// print
		/*for(int y=0;y<m;y++)
		{
			for(int x=0;x<n;x++)
				if(map[x][y]==0)
					cout<<".";
				else if(map[x][y]==1)
					cout<<"O"; //obstacle
				else if(map[x][y]==2)
					cout<<"S"; //start
				else if(map[x][y]==3)
					cout<<"R"; //route
				else if(map[x][y]==4)
					cout<<"F"; //finish
			cout<<endl;
		}*/



		// find route
		std::string route=pathFind(self.getX(), self.getY(), targetPoint.x, targetPoint.y);

		if(route.length()>0)
    {       char c = route.at(0);
            int j = atoi(&c); 
            int x = self.getX() + dx[j];
            int y = self.getY() + dy[j];
			move.setAction(MOVE);
			move.setX(x);                                 
            move.setY(y);  
			return true;
		}	

    }
    
	return false;
}

Point MyStrategy::chooseTarget(const Trooper& self, const model::World& world){
	// TODO ����� � ������ ������� ������ ���� - ����� ���� �� � ���������������
	
	const int closeEnough = 3;

	Point selfPoint(self.getX(), self.getY());
	Point targetPoint = targetToPoint(world, currentTarget);

	if(selfPoint.inRadius(targetPoint, closeEnough)){
		// choose new target
		Corner newTarget;
		//do{
			newTarget = static_cast<Corner>(rand() % 4);
		//}while(newTarget != currentTarget);
		// TODO � �� ��������������� ����, ������ ��� ����� ����� �� ���

		currentTarget = newTarget;
	}

	return targetToPoint(world, currentTarget);
}

Point MyStrategy::targetToPoint(const model::World& world, Corner target)
{
	/*const static Point TopLeft(offset, offset);
	const static Point TopRight(world.getWidth() - offset, offset);
	const static Point BottomLeft(offset, world.getHeight() - offset);
	const static Point BottomRight(world.getWidth() - offset, world.getHeight() - offset);*/
	
	const int offset = 1;
	static const Point corners[] = {
		Point(offset, offset),										// TopLeft
		Point(world.getWidth() - offset, offset),					// TopRight
		Point(offset, world.getHeight() - offset),					// BottomLeft
		Point(world.getWidth() - offset, world.getHeight() - offset)// BottomRight
	};

	return corners[target];
}