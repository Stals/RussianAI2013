#pragma once

#include "model/Move.h"
#include "TurnData.h"

enum HealTargetType{
	SELF,
	OTHERS
};


class HealHelper{
public:
	
	static bool useMedkit(TurnData &turnData, HealTargetType targetType);

	static bool useHeal(TurnData &turnData, HealTargetType targetType);

private:
	HealHelper();

	// TODO 1 метод котоырй найдет типов вокруг дл€ хилинга - должен возвращать массив Trooper
	// и по ним пройтись у кого меньше хп того и хилить
	// либо отсюда сразу возвращать типпа которого нужно хилить

	//todo 2 метод сортироваки массива типов по хп

	//¬озвращет типочка в соседней клатке если такой есть - иначе return false
	static bool getMostDamagedTeammate(TurnData &turnData, model::Trooper& teammate);

	// healAmount - сколько этот хил восстанавливает
	// healToWaste - сколько хила можно завейстить
	static bool healingNeeded(const model::Trooper& trooper, int healAmount, int healToWaste = 0);
};

