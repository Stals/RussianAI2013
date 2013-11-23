#pragma once

#include "TurnData.h"

class ShootHelper{
public:
	// стрелять кого нибудь
	static bool shoot(TurnData& turnData);

	// стрелять именно этого типочка
	static bool shoot(TurnData& turnData, const model::Trooper& target);

	static bool canShoot(const TurnData& turnData, const model::Trooper& target);

	// возвращает цель по которой можно выстрелить
	// TODO если нет в кого выстрелить вернуть кого видно хотябы?
	static bool getTarget(const TurnData& turnData, model::Trooper& target);

private:
	static bool isVisible(const model::World& world, const model::Trooper& self, const model::Trooper& target);
};