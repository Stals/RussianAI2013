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
	// TODO нужно в начале выбрать первую цель - чтобы идти не в противоположный
}


/*
	TODO за движение должен отвечать прям отдельный класс который движется
	Потому что это большой кусок мне кажется

	причем он должен выбирать куда двигаться в зависимости от текущей задачи - типо нужно хп - нужно идти к медпаку например
*/
void MyStrategy::move(const Trooper& self, const World& world, const Game& game, Move& move) {
	TurnData turnData(self, world, game, move);

	//!!!!!// TODO разнести heal others и heal self так чтобы их очередность можно было выбирать 
	
	// TODO use сухпаек
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
	 if (self.getActionPoints() >= self.getShootCost()) {                 // Если достаточно очков действия
        vector<Trooper> troopers = world.getTroopers();                 // Получаем список всех видимых бойцов на поле боя

        for (size_t i = 0; i < troopers.size(); ++i) {                  // Перебираем всех видимых бойцов
            Trooper trooper = troopers.at(i);

            bool canShoot = world.isVisible(self.getShootingRange(),    // Проверяем, что цель не скрыта от бойца
                self.getX(), self.getY(), self.getStance(),             // каким-либо препятствием, а также находится
                trooper.getX(), trooper.getY(), trooper.getStance());   // в пределах его дальности стрельбы

            if (canShoot && !trooper.isTeammate()) {                    // Если можем стрелять и это не союзник
                move.setAction(SHOOT);                                  // Устанавливаем действие "выстрел"
                move.setX(trooper.getX());                              // Устанавливаем координаты цели X...
                move.setY(trooper.getY());                              // ...и Y
                return true;                                            // Завершаем ход
            }
        }
    }
	 return false;

}


bool MyStrategy::simpleMove(const Trooper& self, const World& world, const Game& game, Move& move){
	// TODO поочередно идем по всем углам, как только доходим достаточно близко идем в след угол

	if (self.getActionPoints() >= game.getStandingMoveCost()) {         // Если достаточно очков действия
        vector<vector<CellType> > cells = world.getCells();             // Получаем карту препятствий на игровом поле

		Point targetPoint = chooseTarget(self, world);
		int targetX = targetPoint.x;
		int targetY = targetPoint.y;


        int offsetX = self.getX() > targetX ? -1                        // Определяем смещение при движении по оси X
            : self.getX() < targetX ? 1
            : 0;

        int offsetY = self.getY() > targetY ? -1                        // Определяем смещение при движении по оси Y
            : self.getY() < targetY ? 1
            : 0;

        bool canMoveX = offsetX != 0                                    // Определяем необходимость и возможность движения
            && cells[self.getX() + offsetX][self.getY()] == FREE;       // по оси X

        bool canMoveY = offsetY != 0                                    // Определяем необходимость и возможность движения
            && cells[self.getX()][self.getY() + offsetY] == FREE;       // по оси Y

        if (canMoveX || canMoveY) {                                     // Если мы можем двигаться хотя бы в одном направлении
            move.setAction(MOVE);                                       // Устанавливаем действие "перемещение"

            if (canMoveX && canMoveY) {                                 // Если можем двигаться в двух направлениях
                if (rand() % 2 == 0) {                                  // Выбираем направление случайно
                    move.setX(self.getX() + offsetX);                   // Устанавливаем смещение по горизонтали
                    move.setY(self.getY());                             // Оставляем текущую координату Y
                } else {
                    move.setX(self.getX());                             // Оставляем текущую координату X
                    move.setY(self.getY() + offsetY);                   // Устанавливаем смещение по вертикали
                }
            } else if (canMoveX) {                                      // Если можем двигаться только по горизонтали
                move.setX(self.getX() + offsetX);                       // Устанавливаем смещение по горизонтали
                move.setY(self.getY());                                 // Оставляем текущую координату Y
            } else {                                                    // Остался только один вариант: движение по вертикали
                move.setX(self.getX());                                 // Оставляем текущую координату X
                move.setY(self.getY() + offsetY);                       // Устанавливаем смещение по вертикали
            }

            return true;                                                     // Завершаем ход
        }
    }
	return false;
}

Point MyStrategy::chooseTarget(const Trooper& self, const model::World& world){
	// TODO нужно в начале выбрать первую цель - чтобы идти не в противоположный
	
	const int closeEnough = 3;

	Point selfPoint(self.getX(), self.getY());
	Point targetPoint = targetToPoint(world, currentTarget);

	if(selfPoint.inRadius(targetPoint, closeEnough)){
		// choose new target
		Corner newTarget;
		//do{
			newTarget = static_cast<Corner>(rand() % 4);
		//}while(newTarget != currentTarget);
		// TODO и не противоположный угол, потому что через центр не айс

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