//Copyright (c) 2013, Dmitri V. Kalashnikov. All rights reserved.
//This copyright notice should remain at the top of this file.
//

#ifndef POINT_HPP
#define POINT_HPP 

#include "const.h"
//#include "../DynArray.hpp"

class Point 
{ 
public: 
	static REAL eps;

	int id;

	REAL x[NUM_DIM];

	//DynArray<Point*> *relPoint;
 
	Point(); 
	//void print();
	//void print2();

	bool operator<(Point const &p);
	bool operator<=(Point const &p);
};  
 
typedef Point *pPoint;
 
#endif
