//Copyright (c) 2013, Dmitri V. Kalashnikov. All rights reserved.
//This copyright notice should remain at the top of this file.
//


#include <stdio.h> 
#include <math.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

#include <list>
#include <vector>
using namespace std;

#include "Timer.hpp"  
#include "Point.hpp" 
#include "Util.hpp"



//extern REAL Util::eps;




//-- threads --


void *thread_function( void *ptr );



//---------------------------------------------------
resCont* Util::multiThreadJoin(pPoint A, int A_sz, pPoint B, int B_sz, int num_threads)
{
	
	//-- start timer --
	Timer timer;
	timer.start();
	
	
	lock(&gdata);    //lock global vars
	lock(&need_job); //stop anyone who needs job
	
	
	//-- init global vars --
	Jobs GQ;
	GQ.add(0, A_sz - 1, 0, B_sz - 1, 0);
	
	int num_jobless = 0; //num_threads;
		
	
	//--starting threads --	
	printf("\n Starting %d thread(s) ... ", num_threads);
	

	pthread_t *thread = new pthread_t[num_threads];	
	ThreadParam *param = new ThreadParam[num_threads];
	resCont *result = new resCont[num_threads];
	
	
	
	//-- create threads --
	for (int i = 0; i < num_threads; i ++)
	{
		param[i].tid = i;
		param[i].A = A; 
		param[i].B = B;
		param[i].pGQ = &GQ;
		param[i].num_jobless = &num_jobless;
		param[i].num_threads = num_threads;
		param[i].result = &result[i];
		
		pthread_create( &thread[i], NULL, thread_function, (void*) &param[i]);		
	}
		
	unlock(&gdata);	

	
	//-- wait for threads to complete --
	for (int i = 0; i < num_threads; i ++)
		pthread_join( thread[i], NULL);
	
	delete thread;
	delete param;	

	printf("\n\n Done with threads in %.3f secs \n", timer.getTime());		
			
	return result;
}




//---------------------------------------------------------------
void *thread_function( void *ptr )
{
	Timer timer;
	timer.start();

	//-- Get global params --
	ThreadParam *param = (ThreadParam *) ptr;
	int tid  = param->tid;
	pPoint A = param->A; 
	pPoint B = param->B;
	pJobs GQ = param->pGQ;	
	
	
	//-- Local Params --
	int frA;
	int toA;
	int frB;
	int toB;
	int start_dim;
	
	//--
	printf("\n ** Thread %d: Started **", tid);
	
	//--
	while(1)
	{										
		if (Util::get_job(GQ, frA, toA, frB, toB, start_dim, param) == U_QUIT)
		{
			printf("\n ** Thread %d: Finished (%.3f secs) **", tid, timer.getTime()); //getchar();
			return NULL;
		}
		
		
		//printf("\n -- Thread %d: Got job  A[%d,%d] & B[%d, %d] sd=%d jbls=%d --", tid, frA, toA, frB, toB, start_dim, *(param->num_jobless));
		//getchar();		
		
		Util::egoJoin(A, frA, toA, B, frB, toB, start_dim, param);
	}
}