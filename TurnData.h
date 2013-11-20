#pragma once

#include "Strategy.h"

using namespace model;
struct TurnData{
	TurnData(const model::Trooper& self, const model::World& world, const model::Game& game, model::Move& move):
self(self), world(world), game(game), move(move){}

	const model::Trooper& self;
	const model::World& world;
	const model::Game& game;
	model::Move& move;
};
