#pragma once

struct Point{
	Point(int x, int y):x(x), y(y){}
	Point(const model::Trooper& self):x(self.getX()), y(self.getY()){}

	int x;
	int y;

	// проверяет что эта точка находится на raius рядом с other
	bool inRadius(const Point& other, double radius) const{
		double dx = x - other.x;
		double dy = y - other.y;
		if (dx * dx + dy * dy < radius * radius) {
			return true;
		}
		return false;
	}

	double getDistanceTo(const Point& other){
		return std::fabs(sqrt((double)((other.x - x)*(other.x - x) + (other.y - y)*(other.y - y))));
	}
};
