#pragma once

#include "Strategy.h"
#include "TurnData.h"
#include <vector>

class TeamHelper{
public:
	static std::vector<model::Trooper> getTeammates(const model::World& world);
	static std::vector<model::Trooper> getTeammatesAround(const model::Trooper& self,  const model::World& world);

	static bool isAround(const model::Trooper& self, const model::Trooper& other); 

	// ���������� true ���� ��� ����� ��������� � ������� radius
	static bool teammatesInRadius(double radius, const TurnData& turnData);
};
