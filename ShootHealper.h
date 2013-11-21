#pragma once

#include "TurnData.h"

class ShootHelper{
public:
	static bool shoot(TurnData& turnData);

private:
	static bool isVisible(const model::World& world, const model::Trooper& self, const model::Trooper& target);
};