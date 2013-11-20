#include "MyStrategy.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include <iostream>

#include "HealHelper.h"


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
	
	// TODO use �������
	if(useGrenade(turnData)) return;

	//if(useMedkit(turnData)) return;

	if(HealHelper::useMedkit(turnData, OTHERS)) return;
	if(HealHelper::useMedkit(turnData, SELF)) return;

	if(HealHelper::useHeal(turnData, OTHERS)) return;


	if(simpleShoot(self, world, game, move)) return;

	if(HealHelper::useHeal(turnData, SELF)) return;
	// TODO - check for bonuses
	if(simpleMove(self, world, game, move)) return;
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

bool MyStrategy::useMedkit(const TurnData& turnData){
	//TODO 1) medkit other - else try self

	if(turnData.self.isHoldingMedikit()){
		// if can use
		if (turnData.self.getActionPoints() >= turnData.game.getMedikitUseCost()) {   
			// if need to use
			const int missingHP = (turnData.self.getMaximalHitpoints() - turnData.self.getHitpoints());
			if( missingHP >= turnData.game.getMedikitHealSelfBonusHitpoints()){
				
				turnData.move.setAction(USE_MEDIKIT); 
				turnData.move.setX(turnData.self.getX());
				turnData.move.setY(turnData.self.getY());

				return true;   
			}
		}
	}
	return false;
}

bool MyStrategy::simpleShoot(const Trooper& self, const World& world, const Game& game, Move& move){
	 if (self.getActionPoints() >= self.getShootCost()) {                 // ���� ���������� ����� ��������
        vector<Trooper> troopers = world.getTroopers();                 // �������� ������ ���� ������� ������ �� ���� ���

        for (size_t i = 0; i < troopers.size(); ++i) {                  // ���������� ���� ������� ������
            Trooper trooper = troopers.at(i);

            bool canShoot = world.isVisible(self.getShootingRange(),    // ���������, ��� ���� �� ������ �� �����
                self.getX(), self.getY(), self.getStance(),             // �����-���� ������������, � ����� ���������
                trooper.getX(), trooper.getY(), trooper.getStance());   // � �������� ��� ��������� ��������

            if (canShoot && !trooper.isTeammate()) {                    // ���� ����� �������� � ��� �� �������
                move.setAction(SHOOT);                                  // ������������� �������� "�������"
                move.setX(trooper.getX());                              // ������������� ���������� ���� X...
                move.setY(trooper.getY());                              // ...� Y
                return true;                                            // ��������� ���
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
		int targetX = targetPoint.x;
		int targetY = targetPoint.y;


        int offsetX = self.getX() > targetX ? -1                        // ���������� �������� ��� �������� �� ��� X
            : self.getX() < targetX ? 1
            : 0;

        int offsetY = self.getY() > targetY ? -1                        // ���������� �������� ��� �������� �� ��� Y
            : self.getY() < targetY ? 1
            : 0;

        bool canMoveX = offsetX != 0                                    // ���������� ������������� � ����������� ��������
            && cells[self.getX() + offsetX][self.getY()] == FREE;       // �� ��� X

        bool canMoveY = offsetY != 0                                    // ���������� ������������� � ����������� ��������
            && cells[self.getX()][self.getY() + offsetY] == FREE;       // �� ��� Y

        if (canMoveX || canMoveY) {                                     // ���� �� ����� ��������� ���� �� � ����� �����������
            move.setAction(MOVE);                                       // ������������� �������� "�����������"

            if (canMoveX && canMoveY) {                                 // ���� ����� ��������� � ���� ������������
                if (rand() % 2 == 0) {                                  // �������� ����������� ��������
                    move.setX(self.getX() + offsetX);                   // ������������� �������� �� �����������
                    move.setY(self.getY());                             // ��������� ������� ���������� Y
                } else {
                    move.setX(self.getX());                             // ��������� ������� ���������� X
                    move.setY(self.getY() + offsetY);                   // ������������� �������� �� ���������
                }
            } else if (canMoveX) {                                      // ���� ����� ��������� ������ �� �����������
                move.setX(self.getX() + offsetX);                       // ������������� �������� �� �����������
                move.setY(self.getY());                                 // ��������� ������� ���������� Y
            } else {                                                    // ������� ������ ���� �������: �������� �� ���������
                move.setX(self.getX());                                 // ��������� ������� ���������� X
                move.setY(self.getY() + offsetY);                       // ������������� �������� �� ���������
            }

            return true;                                                     // ��������� ���
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