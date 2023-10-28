//Copyright (c) 2013, Dmitri V. Kalashnikov. All rights reserved.
//This copyright notice should remain at the top of this file.
//

#include <stdio.h>
#include "Point.hpp"  

REAL Point::eps;

//-------------------------------------------
Point::Point()  
{
    ;
	//relPoint = new DynArray<Point*>;
}   
 
//------------------------------------------- 
/*void Point::print()  
{  
	printf("[");

	for (int i = 0; i < NUM_DIM; i++)
		printf("%f, ", x[i]);

	printf("]");
} */

//-------------------------------------------
/*void Point::print2()  
{  
	print();

	printf(" -> ");

	for (int i = 0; i < relPoint->cur_sz; i++)
	{
		relPoint->arr[i]->print();
	}
} */

//-------------------------------------------
bool Point::operator<(Point const &p)
{
	for (int i = 0; i < NUM_DIM; i++)
	{
		if ( (int) (x[i] / eps) < (int)(p.x[i] / eps) ) return true;
		if ( (int) (x[i] / eps) > (int)(p.x[i] / eps) ) return false;
	}

	return false;
} 

//-------------------------------------------
bool Point::operator<=(Point const &p)
{
	for (int i = 0; i < NUM_DIM; i++)
	{
		if ( (int) (x[i] / eps) < (int)(p.x[i] / eps) ) return true;
		if ( (int) (x[i] / eps) > (int)(p.x[i] / eps) ) return false;
	}

	return true;
} 
