//Copyright (c) 2013, Dmitri V. Kalashnikov. All rights reserved.
//This copyright notice should remain at the top of this file.
//

#include <stdio.h>
#include <sys/timeb.h>
#include <time.h>
#include "Timer.hpp"


Timer::Timer()
{
	start();
}

//-----------
void Timer::start()
{
	struct timeb tmp; 
	ftime(&tmp);

	start_time = tmp.time + (tmp.millitm / 1000.0);
}

//-----------
double Timer::getTime()
{
	struct timeb tmp;
    ftime(&tmp);
 
    double cur_time = tmp.time + (tmp.millitm / 1000.0);

	return cur_time - start_time;
}


//---------------------
void Timer::print()
{
   struct timeb timebuffer;
   char *timeline;

   ftime( &timebuffer );
   timeline = ctime( & ( timebuffer.time ) );

   printf("%.19s.%hu %s", timeline, timebuffer.millitm, &timeline[20]);
}

