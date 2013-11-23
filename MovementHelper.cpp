#include "MovementHelper.h"
#include "Astar.h"
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
	if( Point(self).inRadius( targetToPoint(world, TopLeft), 5)) return TopLeft;
	if( Point(self).inRadius( targetToPoint(world, TopRight), 5)) return TopRight;
	if( Point(self).inRadius( targetToPoint(world, BottomLeft), 5)) return BottomLeft;
	if( Point(self).inRadius( targetToPoint(world, BottomRight), 5)) return BottomRight;
	return TopLeft;
}

bool MovementHelper::simpleMove(const Trooper& self, const World& world, const Game& game, Move& move){
	// TODO поочередно идем по всем углам, как только доходим достаточно близко идем в след угол

	if (self.getActionPoints() >= game.getStandingMoveCost()) {         // Если достаточно очков действия
        vector<vector<CellType> > cells = world.getCells();             // Получаем карту препятствий на игровом поле

		Point targetPoint = chooseTarget(self, world);
		
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
		std::string route=pathFind(self.getX(), self.getY(), targetPoint.x, targetPoint.y);

		if(route.length()>0)
    {       char c = route.at(0);
            int j = atoi(&c); 
            int x = self.getX() + dx[j];
            int y = self.getY() + dy[j];
			move.setAction(MOVE);
			move.setX(x);                                 
            move.setY(y);  
			return true;
		}	

    }
    
	return false;
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
