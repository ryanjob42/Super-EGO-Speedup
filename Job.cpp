//Copyright (c) 2013, Dmitri V. Kalashnikov. All rights reserved.
//This copyright notice should remain at the top of this file.
//

#include<stdio.h>

#include "Job.hpp"




//-------------------------------------------
Jobs::Jobs()  
{  
	num_jobs = 0;
}   


//-------------------------------------------
void Jobs::add(int frA, int toA, int frB, int toB, int start_dim)  
{ 
	//printf("\n --Jobs::add-- njobs = %d", num_jobs);
	jobs[num_jobs].frA = frA;
	jobs[num_jobs].toA = toA;
	jobs[num_jobs].frB = frB;
	jobs[num_jobs].toB = toB;
	jobs[num_jobs].start_dim = start_dim;
	
	num_jobs ++;
}   


//-------------------------------------------
void Jobs::get(int* frA, int* toA, int* frB, int* toB, int* start_dim)  
{
	//printf("\n --Jobs::get-- njobs = %d", num_jobs);
	
	num_jobs --;
	
	*frA = jobs[num_jobs].frA;
	*toA = jobs[num_jobs].toA;
	*frB = jobs[num_jobs].frB;
	*toB = jobs[num_jobs].toB;
	*start_dim = jobs[num_jobs].start_dim;
}   
