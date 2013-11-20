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

	// TODO 1 ����� ������� ������ ����� ������ ��� ������� - ������ ���������� ������ Trooper
	// � �� ��� �������� � ���� ������ �� ���� � ������
	// ���� ������ ����� ���������� ����� �������� ����� ������

	//todo 2 ����� ����������� ������� ����� �� ��

	//��������� ������� � �������� ������ ���� ����� ���� - ����� return false
	static bool getMostDamagedTeammate(TurnData &turnData, model::Trooper& teammate);

	// healAmount - ������� ���� ��� ���������������
	// healToWaste - ������� ���� ����� ����������
	static bool healingNeeded(const model::Trooper& trooper, int healAmount, int healToWaste = 0);
};

