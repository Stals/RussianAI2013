#include "ShootHealper.h"


bool ShootHelper::shoot(TurnData& turnData)
{

	if (!(turnData.self.getActionPoints() >= turnData.self.getShootCost())) return false;
	std::vector<Trooper> troopers = turnData.world.getTroopers(); 


	// 1) ��������� ����� ����-�� ������ �����
	for (size_t i = 0; i < troopers.size(); ++i) {
        Trooper trooper = troopers.at(i);
		if(!trooper.isTeammate() && isVisible(turnData.world, turnData.self, trooper)){
			
			// ������� ��� ������ ����������
			const int possibleShots = turnData.self.getActionPoints() / turnData.self.getShootCost();
			const int possibleDMG = possibleShots * turnData.self.getDamage();

			if(possibleDMG >= trooper.getHitpoints()){
			    turnData.move.setAction(SHOOT);                       
                turnData.move.setX(trooper.getX());                       
                turnData.move.setY(trooper.getY()); 
			}
			
		}
	}



	// 2) ��������� ����� �� �������� � ������

	// 3) �������� � ���� � ���������� ���������







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