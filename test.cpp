//Copyright (c) 2013, Dmitri V. Kalashnikov. All rights reserved.
//This copyright notice should remain at the top of this file.
//

#include <stdio.h>  
#include <stdlib.h> 
#include <search.h>  
  
#if defined(VALIDATE)
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#endif

#include "Point.hpp"  
#include "Timer.hpp"   
#include "Util.hpp"  

//static vector<pPoint> ans;


//extern pthread_mutex_t Util::gdata;
 
int main(int argc, char* argv[])  
{
    
	printf("\nEGO e-join\n");

	//-- parse params --
	if (argc != 6) 
	{ 
		printf("USAGE: index eps A_sz B_sz skew num_thread\n");
		return -1; 
	} 

	// skew: {0 - uniform; 1 - skewed; 2 - from file; 3 uni self-join}
 
	REAL eps    = (REAL)atof(argv[1]); //10; 

	int A_sz   = atoi(argv[2]) * 1000; //250000;  
	int A_skew = atoi(argv[4]); 
 	int B_sz   = atoi(argv[3]) * 1000; //250000;  
	int B_skew = atoi(argv[4]);
	int nthr   = atoi(argv[5]);


	printf(" 0: num_dim : %d\n", NUM_DIM);
	printf(" 1: eps     : %f\n", eps);
	printf(" 2: A_sz    : %d\n", A_sz); 
	printf(" 3: A_skew  : %d\n", A_skew);
	printf(" 4: B_sz    : %d\n", B_sz); 
	printf(" 5: B_skew  : %d\n", B_skew);
	printf(" 6: nthr    : %d\n", nthr);
	printf("\n"); 


   
	//-- init --
	Timer *timer = new Timer();   
	Point::eps = eps;
	Util::eps  = eps;
	Util::eps2 = eps * eps;
	

		
	
  
	//--- generate points ---  
	//--- "A" set ---
	printf ("\nGenerating A set... ");  
	timer->start(); 
 
	Point *A; 
 
	if      (A_skew == 0 || A_skew == 3) A = Util::genPoint(A_sz);   
	else if (A_skew == 1) A = Util::genSkewedPoint(A_sz);  
	else                  
    {
        A = Util::loadPoint((char*)DATA_FILE, A_sz);
        //A = Util::toLSS((char*)DATA_FILE, A_sz); //Converting files to LSS format
        printf(" loading \"%s\") ", DATA_FILE);
    }
    
    float t_loadA = timer->getTime();
    float t_loadB = 0;
 
	printf("%.3f secs", t_loadA);  
 
 
	//--- "B" set ---
	printf ("\nGenerating B set... ");  
	timer->start(); 
 
	Point *B; 
 
	if      (B_skew == 0) B = Util::genPoint(B_sz);   
	else if	(B_skew == 1) B = Util::genSkewedPoint(B_sz);  
	else                  B = A; //self-join
 
    t_loadB = timer->getTime();
	printf("%.3f secs", t_loadB); 
    
    
    //-- Reorder Dimensions --
    printf ("\nReordering dimensions in A & B... ");  
	timer->start(); 
    
    Util::reorderDim(A, A_sz, B, B_sz);
  	
    //-- Test SimpleJoin as a baseline and/or "sanity check" -- 
    //resCont res;
    //Util::simpleBlockJoin(A, A_sz, B, B_sz, &res, 100); //--Util::simpleJoin(A, 0, A_sz-1, B, 0, B_sz-1, &res);
    //printf("\n|R| = %d, |R|-|A|=%d\n", (int) (res.size()/2), (int) (res.size()/2) - A_sz );
  
    float t_reord = timer->getTime();
	printf("%.3f secs", t_reord); 
    
    //exit(1);


	//-- EGO processing --  

	printf ("\nEGO Sorting A... ");  
	timer->start(); 
	//Util::egoSort(A, 0, A_sz-1);
	qsort(A, A_sz, sizeof(Point), pcmp);
	float t_srtA = timer->getTime();
	float t_srtB = 0;
	printf("%.3f ", t_srtA); 


	if (A_skew < 2) // not a self-join
	{
		printf ("\nEGO Sorting B... ");  
		timer->start(); 
		//Util::egoSort(B, 0, B_sz-1);
		qsort(B, B_sz, sizeof(Point), pcmp);
		t_srtB = timer->getTime(); 
	    printf("%.3f ", t_srtB);
	}
	 
  
	//-- EGO Join -- 
	printf ("\nDo join (secs) ... ");  
	timer->start();
	
	resCont *result = Util::multiThreadJoin(A, A_sz, B, B_sz, nthr);
	float t_join = timer->getTime();
	printf ("%.3f \n", t_join); 

	
    //-- check results --
	unsigned int sum = 0;

	for (int i = 0; i < nthr; i++)
		sum += result[i].size();

	printf("\n|R| = %u", sum / 2);
	printf("\n|A| = %d", A_sz);
    
    
    //-- Save results --
    float t_save = 0;
    
#if defined(VALIDATE)
    printf ("\nSaving resuls to disk (secs) ... ");  
	timer->start();
    
    // Use the name of the executable as the name of the file, plus ".out".
	// Need to replace the "./bin/" with "./outputs/".
	// Also need to remove the "-validate".
	std::string executableName = argv[0];
	executableName = std::regex_replace(executableName, std::regex("/bin/"), "/outputs/");
	executableName = std::regex_replace(executableName, std::regex("-validate"), "");
    ofstream out(executableName + ".out");
    for (int i = 0; i < nthr; i++)
    {
        resCont currResult = result[i];
        for (int j = 0; j < (int)(currResult.size()); j+=2) {
            std::string left = std::to_string(currResult[j]);
            std::string right = std::to_string(currResult[j+1]);
            out << left << "," << right << "\n";
        }
    }
	out.close();
#endif
    
  	t_save = timer->getTime();
	printf ("%.3f \n", t_save);
    
    
	//printf("\n\n *** uncomment Point.hpp ***\n\n");
    
    //-- Output stats --
	if (A_skew < 2)
	{
		printf("\n\n---J(A,B)-----");
		printf("\nS_A1 = %f\n\n", sum / 2.0 / A_sz);
		
		printf(" %f %f %f %f %f %u %f %f %f\n\n", eps, t_srtA, t_srtB, t_join, t_srtA + t_srtB + t_join, sum/2, (sum/2.0) / A_sz, t_loadA, t_loadB);
	}
	else 
	{
		printf("\n\n---J(A,A)-----");	
		printf("\n|R| - |A| = %d", sum/2 - A_sz);
		printf("\nS_A2 = %f\n\n", (sum/2.0 - A_sz) / A_sz);	
	
		printf(" %f %.3f %.3f %.3f %u %f %.3f %.3f %.3f %.3f\n\n", eps, t_srtA, t_join, t_srtA + t_join, sum/2 - A_sz, (sum/2.0 - A_sz) / A_sz, t_loadA, t_loadB, t_save, t_reord);
	}
	//printf("\nnum_heur_calls = %d\n\n", Util::num_heur_calls);
	//printf("\nnum_heur_succ = %d\n\n",  Util::num_heur_calls - Util::num_heur_fails);


	return 0;
}

