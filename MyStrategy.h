#pragma once

#ifndef _MY_STRATEGY_H_
#define _MY_STRATEGY_H_

#include "Strategy.h"
#include "TurnData.h"
#include "Point.h"
#include "TeamHelper.h"


class MyStrategy : public Strategy {
public:
    MyStrategy();
    void move(const model::Trooper& self, const model::World& world, const model::Game& game, model::Move& move);

	bool useRation(const TurnData& turnData);
	bool useGrenade(const TurnData& turnData);

private:
	static TrooperIndex currentEnemyID;
	static bool firstMove;

	void doFirstMoveStuff(const TurnData& turnData);

};

#endif
