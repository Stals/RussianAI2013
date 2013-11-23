#pragma once

#include "Point.h"
#include "TurnData.h"

class BonusHelper{
public:
	// ��������� ��������� � ������ �����(�������� � ���� ���) � ������� range
	static bool pickupBonus(const TurnData& turnData, double range);
	// ���������� ������ ������ ������ � ������� range
	static std::vector<Bonus> getBonusesInRange(const TurnData& turnData, double range);
	// ���������� true ���� � ������ ���� ����� ���� type
	static bool haveBonus(const model::Trooper& self, BonusType type);

private:
	BonusHelper();
};
