#include "MovementHelper.h"
#include "Astar.h"
#include "TeamHelper.h"
#include <cmath>
#include <cstdlib>

Corner MovementHelper::currentTarget = TopLeft;

void MovementHelper::chooseFirstTarget(const TurnData& turnData)
{
	currentTarget = getCurrentCorner(turnData.world, turnData.self);
	chooseTarget(turnData.self, turnData.world);
}

Corner MovementHelper::getRandomCorner()
{
	// выбирам 1 или 2
	int rnd = ((rand() % 2) + 1);
	bool isMinus = rand() % 2;
	if(isMinus) rnd *= -1;

	return static_cast<Corner>(rnd);
}

Corner MovementHelper::getCurrentCorner(const model::World& world, const model::Trooper& self)
{
	const double radius = 5.0;
	if( Point(self).inRadius( targetToPoint(world, TopLeft), radius)) return TopLeft;
	if( Point(self).inRadius( targetToPoint(world, TopRight), radius)) return TopRight;
	if( Point(self).inRadius( targetToPoint(world, BottomLeft), radius)) return BottomLeft;
	if( Point(self).inRadius( targetToPoint(world, BottomRight), radius)) return BottomRight;
	return TopLeft;
}

bool MovementHelper::simpleMove(const TurnData& turnData){
	Point targetPoint = chooseTarget(turnData.self, turnData.world);		
	return MovementHelper::moveTo(targetPoint, turnData);
}

Point MovementHelper::chooseTarget(const Trooper& self, const model::World& world){
	// TODO нужно в начале выбрать первую цель - чтобы идти не в противоположный
	const int closeEnough = 3;

	Point selfPoint(self.getX(), self.getY());
	Point targetPoint = targetToPoint(world, currentTarget);

	if(selfPoint.inRadius(targetPoint, closeEnough)){
		// choose new target
		Corner newTarget;
		do{
			newTarget = getRandomCorner();
		}while((newTarget == currentTarget) || ((abs(newTarget) == abs(currentTarget))));
		// TODO и не противоположный угол, потому что через центр не айс

		currentTarget = newTarget;
	}

	return targetToPoint(world, currentTarget);
}

Point MovementHelper::targetToPoint(const model::World& world, Corner target)
{
	const int offset = 1;
	switch(target){
	case TopLeft: return Point(offset, offset);
	case TopRight: return Point(world.getWidth() - offset, offset);
	case BottomLeft: return Point(offset, world.getHeight() - offset);
	case BottomRight: return Point(world.getWidth() - offset, world.getHeight() - offset);
	}
	return Point(0,0);
}


bool MovementHelper::moveTo(const Point& targetPoint, const TurnData& turnData)
{
	if (!(turnData.self.getActionPoints() >= turnData.game.getStandingMoveCost())) return false;
	 // Если достаточно очков действия

	vector<vector<CellType> > cells = turnData.world.getCells();             // Получаем карту препятствий на игровом поле
		// fill map
		for(int y = 0; y < m; y++){
			for(int x = 0; x < n; x++){
				if(cells[x][y] == FREE){
					map[x][y] = 0;
				}else{
					map[x][y] = 1;
				}
			}
		}

		// print
		/*for(int y=0;y<m;y++)
		{
			for(int x=0;x<n;x++)
				if(map[x][y]==0)
					cout<<".";
				else if(map[x][y]==1)
					cout<<"O"; //obstacle
				else if(map[x][y]==2)
					cout<<"S"; //start
				else if(map[x][y]==3)
					cout<<"R"; //route
				else if(map[x][y]==4)
					cout<<"F"; //finish
			cout<<endl;
		}*/



		// find route
		std::string route=pathFind(turnData.self.getX(), turnData.self.getY(), targetPoint.x, targetPoint.y);

		if(route.length()>0)
    {       char c = route.at(0);
            int j = atoi(&c); 
            int x = turnData.self.getX() + dx[j];
            int y = turnData.self.getY() + dy[j];
			turnData.move.setAction(MOVE);
			turnData.move.setX(x);                                 
            turnData.move.setY(y);  
			return true;
		}	
	return false;

}

bool MovementHelper::follow(TrooperType type, const TurnData& turnData)
{
	// get teammates
	// get teammate with this type
	// go to this point as target

	std::vector<model::Trooper> teammates = TeamHelper::getTeammates(turnData.world);

	for(size_t i = 0; i < teammates.size(); ++i){
		Trooper trooper = teammates[i];

		if(trooper.getType() == type){
			return moveTo(Point(trooper), turnData);
		}
	}


	return false;
}