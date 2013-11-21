#include "ShootHealper.h"


bool ShootHelper::shoot(TurnData& turnData)
{
		if (turnData.self.getActionPoints() >= turnData.self.getShootCost()) {                 // ���� ���������� ����� ��������
        std::vector<Trooper> troopers = turnData.world.getTroopers();                 // �������� ������ ���� ������� ������ �� ���� ���

        for (size_t i = 0; i < troopers.size(); ++i) {                  // ���������� ���� ������� ������
            Trooper trooper = troopers.at(i);

			const bool canShoot = isVisible(turnData.world, turnData.self, trooper);

            if (canShoot && !trooper.isTeammate()) {                   
                turnData.move.setAction(SHOOT);                       
                turnData.move.setX(trooper.getX());                       
                turnData.move.setY(trooper.getY());                      
                return true;                                           
            }
        }
    }
	 return false;


}


bool ShootHelper::isVisible(const model::World& world, const model::Trooper& self, const model::Trooper& target)
{
	return world.isVisible(self.getShootingRange(),    
            self.getX(), self.getY(), self.getStance(),            
            target.getX(), target.getY(), target.getStance());
}