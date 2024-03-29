#pragma once

#include "TurnData.h"
#include "Point.h"


// -2/2 ����� ��� ����������� ��������������� �����
enum Corner{
	TopLeft = -2,
	BottomRight = 2,
	TopRight = -1,
	BottomLeft = 1
};


class MovementHelper{
public:
	static void chooseFirstTarget(const TurnData& turnData);
	static bool simpleMove(const TurnData& turnData);

	// ������� ��������� �� ��������� ����� ���� / ���� ������ ��� ��� �� ���������� ���������� false
	static bool follow(TrooperType type, const TurnData& turnData);

	// considerTeammates - ���� true �� ����� ������� ������� ������ ���
	static bool moveTo(const Point& target, const TurnData& turnData, bool considerTeammates);

private:
	MovementHelper();
	static Corner getRandomCorner();
	static Corner getCurrentCorner(const model::World& world, const model::Trooper& self);	
	static Point chooseTarget(const model::Trooper& self, const model::World& world);
	static Point targetToPoint(const model::World& world, Corner target);

	static Corner currentTarget;
};