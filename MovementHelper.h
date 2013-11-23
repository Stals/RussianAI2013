#pragma once

#include "TurnData.h"
#include "Point.h"


// -2/2 нужны для определения противоположных углов
enum Corner{
	TopLeft = -2,
	BottomRight = 2,
	TopRight = -1,
	BottomLeft = 1
};


class MovementHelper{
public:
	static void chooseFirstTarget(const TurnData& turnData);
	static bool simpleMove(const TurnData& turnData);

	// пробует следовать за союзником этого типа / если такого нет или не получается возвращает false
	static bool follow(TrooperType type);

	static bool moveTo(const Point& target, const TurnData& turnData);

private:
	MovementHelper();
	static Corner getRandomCorner();
	static Corner getCurrentCorner(const model::World& world, const model::Trooper& self);	
	static Point chooseTarget(const model::Trooper& self, const model::World& world);
	static Point targetToPoint(const model::World& world, Corner target);

	static Corner currentTarget;
};