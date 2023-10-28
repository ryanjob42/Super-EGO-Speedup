//Copyright (c) 2013, Dmitri V. Kalashnikov. All rights reserved.
//This copyright notice should remain at the top of this file.
//

#ifndef JOB_HPP 
#define JOB_HPP 



class Job
{
public:
	int frA;
	int toA;
	int frB;
	int toB;
	int start_dim;	
};
typedef Job *pJob;


class Jobs
{
public:
	Job jobs[10000];
 	int num_jobs;
	
	Jobs();
	
	void add(int frA, int toA, int frB, int toB, int start_dim);
	void get(int* frA, int* toA, int* frB, int* toB, int* start_dim);
	
};
typedef Jobs *pJobs;


#endif