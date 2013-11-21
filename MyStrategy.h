#pragma once

#ifndef _MY_STRATEGY_H_
#define _MY_STRATEGY_H_

#include "Strategy.h"
#include "TurnData.h"
#include "Point.h"

enum Corner{
	TopLeft,
	TopRight,
	BottomLeft,
	BottomRight
};


class MyStrategy : public Strategy {
public:
    MyStrategy();
    void move(const model::Trooper& self, const model::World& world, const model::Game& game, model::Move& move);


	bool useGrenade(const TurnData& turnData);
	
	bool simpleMove(const model::Trooper& self, const model::World& world, const model::Game& game, model::Move& move);
	Point chooseTarget(const model::Trooper& self, const model::World& world);

	Point targetToPoint(const model::World& world, Corner target);

private:
	static Corner currentTarget;
};

#endif
