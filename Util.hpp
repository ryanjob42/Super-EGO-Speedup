//Copyright (c) 2013, Dmitri V. Kalashnikov. All rights reserved.
//This copyright notice should remain at the top of this file.
//

#ifndef UTIL_HPP
#define UTIL_HPP 

#include "Job.hpp"

#include <pthread.h>  
#include <time.h>

#include <vector>
#include <list>
using namespace std;

#define lock(a)         pthread_mutex_lock(a)
//#define timed_lock(a,b) pthread_mutex_timedlock(a,b)
#define unlock(a)       pthread_mutex_unlock(a)
#define trylock(a)      pthread_mutex_trylock(a)


#define U_QUIT  -1
#define U_OK     1


class CellRect;
int pcmp(const void *v1, const void *v2);


typedef vector<int> resCont;
//typedef list<int> resCont;

//typedef list<int> tyQ;
//typedef tyQ *pQ;

class ThreadParam
{
public:
	int tid;
	pPoint A;	
	pPoint B;
	pJobs pGQ;
	int *num_jobless;
	int num_threads;
	resCont *result;
};

typedef ThreadParam *pThreadParam;




class Util 
{ 
public:  
	//-- EGO --
	static REAL eps;
	static REAL eps2;
    
    
    //-- scanning ranges for SimpleJoin--
    static int r1[NUM_DIM+1][2];
    static int r2[NUM_DIM+1][2];
    static int r3[NUM_DIM+1][2];


	static int  num_heur_calls;
	static int  num_heur_fails;
	
	static pthread_mutex_t need_job;
	static pthread_mutex_t gdata;

	
	//static pPoint A;
	//static pPoint B;
	
	static resCont* multiThreadJoin(pPoint A, int A_sz, pPoint B, int B_sz, int num_threads);
	static void egoJoin(pPoint A, int frA, int toA, pPoint B, int frB, int toB, int start_dim, pThreadParam param);
	static void simpleJoin (pPoint A, int frA, int toA, pPoint B, int frB, int toB, resCont *result);
    static void simpleJoin2(pPoint A, int frA, int toA, pPoint B, int frB, int toB, resCont *result, int i);
    static void simpleBlockJoin(pPoint A, int A_sz, pPoint B, int B_sz, resCont *result, int block_sz);
    static void simpleBlockJoin2(pPoint A, int A_sz, pPoint B, int B_sz, resCont *result, int block_sz);
    static void reorderDim(pPoint A, int A_sz, pPoint B, int B_sz);
	
	
	//-- aux --
	static void add_job(pJobs pjobs, int frA, int toA, int frB, int toB, int start_dim);
	static int  get_job(pJobs pjobs, int &frA, int &toA, int &frB, int &toB, int &start_dim, ThreadParam *param);
	

	//-- random number generators --
	static REAL rnd();  
	static REAL rndNormal01(); 
	static REAL rndNormal(REAL mean, REAL stddev); 
 
	//-- data generators --
	static Point* loadPoint(char* filename, int num_point); 
	static Point* genPoint(int num_point);
    static Point* toLSS(char* filename, int num_point);
	
	//static PList* genQList(int num_query); 
 
	//-- non-uniform data generators -- 
	static Point* genSkewedPoint(int num_point);   
	static Point* genCenter(REAL x, REAL y);
 
	static void genPointCluster(Point *center, REAL std_dev, Point *pointArr, int from, int size); 

}; 

 
#endif
