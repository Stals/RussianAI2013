#pragma once

#ifndef _MY_STRATEGY_H_
#define _MY_STRATEGY_H_

#include "Strategy.h"
#include "TurnData.h"
#include "Point.h"

// -2/2 нужны для определения противоположных углов
enum Corner{
	TopLeft = -2,
	BottomRight = 2,
	TopRight = -1,
	BottomLeft = 1
};


class MyStrategy : public Strategy {
public:
    MyStrategy();
    void move(const model::Trooper& self, const model::World& world, const model::Game& game, model::Move& move);

	bool useRation(const TurnData& turnData);
	bool useGrenade(const TurnData& turnData);
	
	bool simpleMove(const model::Trooper& self, const model::World& world, const model::Game& game, model::Move& move);
	Point chooseTarget(const model::Trooper& self, const model::World& world);

	Point targetToPoint(const model::World& world, Corner target);

private:
	static Corner currentTarget;

	Corner getRandomCorner();
};

#endif
