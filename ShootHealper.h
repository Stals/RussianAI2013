#pragma once

#include "TurnData.h"

class ShootHelper{
public:
	// �������� ���� ������
	static bool shoot(TurnData& turnData);

	// �������� ������ ����� �������
	static bool shoot(TurnData& turnData, const model::Trooper& target);

	static bool canShoot(const TurnData& turnData, const model::Trooper& target);

	// ���������� ���� �� ������� ����� ����������
	// TODO ���� ��� � ���� ���������� ������� ���� ����� ������?
	static bool getTarget(const TurnData& turnData, model::Trooper& target);

private:
	static bool isVisible(const model::World& world, const model::Trooper& self, const model::Trooper& target);
};