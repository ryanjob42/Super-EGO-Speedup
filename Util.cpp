//Copyright (c) 2013, Dmitri V. Kalashnikov. All rights reserved.
//This copyright notice should remain at the top of this file.
//

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "Point.hpp"  
//#include "CellRect.hpp"
  
#include "Util.hpp"  

#include <list>
using namespace std;


REAL Util::eps;
REAL Util::eps2; //eps squared

int Util::r1[NUM_DIM+1][2]; //ranges for SimpleJoin
int Util::r2[NUM_DIM+1][2];
int Util::r3[NUM_DIM+1][2];

int  Util::num_heur_calls = 0;
int  Util::num_heur_fails = 0;

pthread_mutex_t Util::need_job = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t Util::gdata	   = PTHREAD_MUTEX_INITIALIZER;



//---------------------------------------------------
Point* Util::loadPoint(char* filename, int num_point)
{
	FILE *f;
	int ch;

	f = fopen(filename, "r");

	if (f == NULL)
	{
		printf("Util::loadPoint(): cannot open %s", filename);
		exit(-1);
	}

	Point *pointArr = new Point[num_point + 1];  
  
	for (int i = 0; i < num_point; i++)  
	{  
		pPoint p = & pointArr[i];

		fscanf(f, "%d", &p->id);

		for (int k = 0; k < NUM_DIM; k++)
		{
			fscanf(f, "%f", &p->x[k]);
		}

		//-- skip till the end of line --
		
		do
		{
			ch = fgetc(f);
		} while (ch != '\n');

	}  
  
    fclose(f);
    
	return pointArr;  
}



//---------------------------------------------------
// Convert files to LSS format
Point* Util::toLSS(char* filename, int num_point)
{
    printf("\n\nConverting file '%s' to LSS format...", filename);
    
	FILE *f;
    FILE *f2;
	int ch;
    
	f = fopen(filename, "r");
    
	if (f == NULL)
	{
		printf("\nUtil::toLSS(): cannot open '%s'", filename);
		exit(-1);
	}
    
    f2 = fopen("./data_lss.bin", "w");
    
    if (f2 == NULL)
	{
		printf("\nUtil::toLSS(): cannot open './data_lss.bin'");
		exit(-1);
	}
    
    
    
    // Write in header: number of points, dimensions.
    if (fwrite(&num_point, sizeof(num_point), 1, f2) != 1) {
        printf("\nUtil::toLSS(): cannot write number of points");
    }
    
    int n_dim = NUM_DIM;
    
    if (fwrite(&n_dim, sizeof(n_dim), 1, f2) != 1) {
        printf("\nUtil::toLSS(): cannot write number of dimensions");
    }
    
    
    
    
    
	
	for (int i = 0; i < num_point; i++)
	{
		int id;
        
		fscanf(f, "%d", &id);
        
		for (int k = 0; k < NUM_DIM; k++)
		{
            float val;
			fscanf(f, "%f", &val);
            
            if (fwrite(&val, sizeof(val), 1, f2) != 1) {
                printf("\nUtil::toLSS(): error writing point data\n");
            }
 		}
        
		//-- skip till the end of line --
		
		do
		{
			ch = fgetc(f);
		} while (ch != '\n');
        
	}
    
    fclose(f);
    fclose(f2);
    
    printf("done!\n\n");
    
    exit(-1);
    
    
	return NULL;
}




//---------------------------------------------------
//IDEA try to use MBR instead of inactive dimensions...
void Util::egoJoin(pPoint A, int frA, int toA, pPoint B, int frB, int toB, int start_dim, pThreadParam param)
{
	num_heur_calls ++;
	
	//-- Determine if BR1 and BR2++ intersect -
	pPoint fst_A = &A[frA];
	pPoint lst_A = &A[toA];
	pPoint fst_B = &B[frB];
	pPoint lst_B = &B[toB];
	
	/*if (start_dim >= NUM_DIM) {
		printf("\negoJoin(): ***q1: Error, start_dim = %d***", start_dim);
		exit(-1);
	}*/
	
    int i;
	
	for (i = start_dim; i < NUM_DIM; i++)
	{
		int lo_A = (int) (fst_A->x[i] / eps);
		int hi_B = (int) (lst_B->x[i] / eps);
		
		if (lo_A > hi_B + 1) // lo_B--------hi_B  lo_A--------hi_A
			return;
		
		int lo_B = (int) (fst_B->x[i] / eps);
		int hi_A = (int) (lst_A->x[i] / eps);
		
		if (lo_B > hi_A + 1) // lo_A--------hi_A  lo_B---------hi_B
			return;

		if ( (lo_A < hi_A) || (lo_B < hi_B) ) // the remaining dimensions will always intersect
		{
			start_dim = i;
			break;
		}
	}
	
	/*if (start_dim >= NUM_DIM) {
		printf("\negoJoin(): ***q2: Error, start_dim = %d***", start_dim);
		exit(-1);
	}*/
	

	num_heur_fails ++;
	
    int A_sz = toA - frA + 1;
	int B_sz = toB - frB + 1;


	//-- case1 --
	if (A_sz < MINLEN && B_sz < MINLEN)
	{
        /*printf("\n i=%d, d_str = %d", i, start_dim);
        
        for (int i = 0; i < NUM_DIM; i++)
        {
            int lo_A = (int) (fst_A->x[i] / eps);
            int hi_A = (int) (lst_A->x[i] / eps);
      
            int lo_B = (int) (fst_B->x[i] / eps);
            int hi_B = (int) (lst_B->x[i] / eps);
            
            printf("\n d=%d A[%d,%d] B[%d,%d]",i, lo_A, hi_A, lo_B, hi_B);
            
        }
        
        getchar();*/
        
        //simpleJoin (A, frA, toA, B, frB, toB, param->result);
		simpleJoin2(A, frA, toA, B, frB, toB, param->result, i);
        
		return;
	}
	
		
	//-- share if there are jobless threads --
	//if (*(param->num_jobless) > 0)// || A_sz < 10000 || B_sz < 10000) // notice - a check without a mutex, so it should be fast
	//if (rnd() > 0.9)
	if (A_sz > 5000 || B_sz > 5000)
	{
		pJobs GQ = param->pGQ;
		
		lock(&gdata);
		//if (*(param->num_jobless) == 0 || GQ->num_jobs > 0) //do not share
        if ( GQ->num_jobs >= param->num_threads) //do not share
		{
			unlock(&gdata);	
		    goto dont_share;
		}
		
		
		//printf("\nsimpleJoin(): Sharing");
		
		
		//-- case 2 --
		if (A_sz >= MINLEN && B_sz >= MINLEN)
		{
			//printf("\nsimpleJoin(): case 2");
			
			GQ->add(frA             , frA + A_sz / 2, frB             , frB + B_sz/2, start_dim); // f f
			GQ->add(frA             , frA + A_sz / 2, frB + B_sz/2 + 1, toB         , start_dim); // f s
			GQ->add(frA + A_sz/2 + 1, toA           , frB             , frB + B_sz/2, start_dim); // s f
			GQ->add(frA + A_sz/2 + 1, toA           , frB + B_sz/2 + 1, toB         , start_dim); // f s
		}
		
		//-- case 3--
		else if (A_sz >= MINLEN && B_sz < MINLEN)
		{
			GQ->add(frA             , frA + A_sz / 2, frB, toB, start_dim); // f full
			GQ->add(frA + A_sz/2 + 1, toA           , frB, toB, start_dim); // s full
		}
		
		//-- case 4 --
		else if (A_sz < MINLEN && B_sz >= MINLEN)
		{
			GQ->add(frA, toA, frB             , frB + B_sz/2, start_dim); // f f
			GQ->add(frA, toA, frB + B_sz/2 + 1, toB         , start_dim); // f s
		}
		
		unlock(&gdata);
		trylock(&need_job);
		unlock(&need_job);							
		
		return;
	}
	
dont_share:

	//-- no threads are jobless, so do the job yourself --

	//printf(".");
	//-- case 2 --
	if (A_sz >= MINLEN && B_sz >= MINLEN)
	{
		egoJoin(A, frA             , frA + A_sz / 2, B, frB             , frB + B_sz/2, start_dim, param); // f f
		egoJoin(A, frA             , frA + A_sz / 2, B, frB + B_sz/2 + 1, toB         , start_dim, param); // f s
		egoJoin(A, frA + A_sz/2 + 1, toA           , B, frB             , frB + B_sz/2, start_dim, param); // s f
		egoJoin(A, frA + A_sz/2 + 1, toA           , B, frB + B_sz/2 + 1, toB         , start_dim, param); // f s
		return;
	}

	//-- case 3--
	if (A_sz >= MINLEN && B_sz < MINLEN)
	{
		egoJoin(A, frA             , frA + A_sz / 2, B, frB, toB, start_dim, param); // f full
		egoJoin(A, frA + A_sz/2 + 1, toA           , B, frB, toB, start_dim, param); // s full
		return;
	}

	//-- case 4 --
	if (A_sz < MINLEN && B_sz >= MINLEN)
	{
		egoJoin(A, frA, toA, B, frB             , frB + B_sz/2, start_dim, param); // f f
		egoJoin(A, frA, toA, B, frB + B_sz/2 + 1, toB         , start_dim, param); // f s
		return;
	}
}



//-----------------------------------------
void Util::reorderDim(pPoint A, int A_sz, pPoint B, int B_sz)
{
    //-- define stat vars --
    int num_buck = ceil(1 / eps) + 1;   //number of buckets
    pPoint hA = new Point[num_buck];    //histogram for A
    pPoint hB = new Point[num_buck];    //histogram for B
    double d[NUM_DIM];                  //avg distance in each dim
    
    
    //-- init stats to zeroes --
    for(int i = 0; i < NUM_DIM; i++)
        d[i] = 0;
    
    for (int i = 0; i < num_buck; i++)
    {
        for (int j = 0; j < NUM_DIM; j++)
        {
            hA[i].x[j] = 0;
            hB[i].x[j] = 0;
        }
    }
        
    
    //-- sample points --
    int sample_sz = 2000;
    
    
    for (int i = 0; i < sample_sz; i++)
    {
        //-- get rnd point from A --
        int rnd_i = (A_sz - 1) * rnd();
        pPoint rnd_pA = &A[rnd_i];
        
        //-- get rnd point from B --
        rnd_i = (B_sz - 1) * rnd();
        pPoint rnd_pB = &B[rnd_i];        

        //-- update stats --
        for (int j = 0; j < NUM_DIM; j++)
        {
            d[j] += fabs(rnd_pA->x[j] - rnd_pB->x[j]);
            
            int buck_i = (int) (rnd_pA->x[j] / eps); 
            hA[buck_i].x[j] += 1;
            
            buck_i = (int) (rnd_pB->x[j] / eps); 
            hB[buck_i].x[j] += 1;
        }
    }
    
        
    //-- compute fail factor f in each dimension --
    double f[NUM_DIM];
    double g[NUM_DIM];
    
    for (int i = 0; i < NUM_DIM; i++)
    {
        f[i] = 0;
        
        for (int j = 0; j < num_buck; j++)
        {
            if (j == 0) 
            {
                f[i] += hA[j].x[i] * (hB[j].x[i] + hB[j+1].x[i]);
                continue;
            }
            else if (j == (num_buck - 1) )
            {
                f[i] += hA[j].x[i] * (hB[j].x[i] + hB[j-1].x[i]);
                continue;
            }
            
            f[i] += hA[j].x[i] * (hB[j].x[i] + hB[j-1].x[i] + hB[j+1].x[i]);
        }
    
    }
    
    
    //-- normalizing f --
    for (int i = 0; i < NUM_DIM; i++)
    {
        d[i] = d[i] / sample_sz;
        f[i] = f[i] / (sample_sz * sample_sz);
        g[i] = -d[i]; //f[i];
        //printf("\n f[%d] = %f", i, f[i]);
    }
    
    
    //-- constracting map of remapping (not efficient) --
    int map[NUM_DIM];
    
    for (int i = 0; i < NUM_DIM; i++)
    {
        double min = 1000;
        int min_j = -1;
        
        for (int j = 0; j < NUM_DIM; j++)
        {
            if (g[j] < min)
            {
                min = g[j];
                min_j = j;
            }
        }
        
        map[i] = min_j;
        g[min_j] = 2.0; // so that it is not min next time
    }
    
    
    printf("\n\n -- map --");
    for (int i = 0; i < NUM_DIM; i++)
    {
        printf("\n s[%2d] = %.3f, d[%2d] = %.3f, map[%2d] = %2d", i, 1 - f[i], i, d[i], i, map[i]);
    }
    printf("\n-- --- --\n\n");
    
    
    //-- reoder dimension in A --
    for (int i = 0; i < A_sz; i++)
    {
        REAL x[NUM_DIM];
        
        
        for (int j = 0; j < NUM_DIM; j++)
            x[j] = A[i].x[j];
        
        for (int j = 0; j < NUM_DIM; j++)
            A[i].x[j] = x[map[j]];
    }
    
    
    //-- reoder dimension in B, if not self-join --
    if(B != A)
    {
        for (int i = 0; i < B_sz; i++)
        {
            REAL x[NUM_DIM];
            
            
            for (int j = 0; j < NUM_DIM; j++)
                x[j] = B[i].x[j];
            
            for (int j = 0; j < NUM_DIM; j++)
                B[i].x[j] = x[map[j]];
        }
    }
    
    
    //-- reorder stats accrodingly as well --
    double *rs = new double[NUM_DIM];
    double *rd = new double[NUM_DIM];
    
    for (int j = 0; j < NUM_DIM; j++)
    {
        rs[j] = 1 - f[map[j]];
        rd[j] = d[map[j]];
    }
    
    
    printf("\n\n -- remap --");
    for (int i = 0; i < NUM_DIM; i++)
    {
        printf("\n rs[%2d] = %.3f, rd[%2d] = %.3f", i, rs[i], i, rd[i]);
    }
    printf("\n-- --- --\n\n");
    
    
    
    
    //-- Case 1: zero inactive dimensions --
    int sml_seq_sz = __min(1, NUM_DIM);
    
    for (int i = 0; i < sml_seq_sz; i++)
    {
        r1[i][0] = 0;
        r1[i][1] = NUM_DIM-1;
        
        r2[i][0] = 0;
        r2[i][1] = -1;
        
        r3[i][0] = 0;
        r3[i][1] = -1;
    }
    
    
    //-- Case 2: all dims are inactive --
    r1[NUM_DIM][0] = 0;
    r1[NUM_DIM][1] = NUM_DIM-1;
    
    r2[NUM_DIM][0] = 0;
    r2[NUM_DIM][1] = -1;
    
    r3[NUM_DIM][0] = 0;
    r3[NUM_DIM][1] = -1;


   
    
    //-- Case 3: remaining cases --
    
    
    //-- find first k s.t. rd[k] < eps/2 --
    int k = NUM_DIM;
    
    for (int i = 0; i < NUM_DIM; i++)
    {
        if (rd[i] < eps/2)
        {
            k = i;
            break;
        }
    }
    
      
    
    //--
    for (int i = sml_seq_sz; i < NUM_DIM; i++)
    {
        //-- Case I: 1-interval --
        if (rd[i] < eps/2) 
        {
            r1[i][0] = 0;
            r1[i][1] = NUM_DIM-1;
            
            r2[i][0] = 0;
            r2[i][1] = -1;
            
            r3[i][0] = 0;
            r3[i][1] = -1;
            
            continue;
        }
        
        //-- Case II: 3-intervals --
        if (k < NUM_DIM)
        {
            r1[i][0] = i; 
            r1[i][1] = k - 1;
            
            r2[i][0] = 0;
            r2[i][1] = i - 1;
            
            r3[i][0] = k;
            r3[i][1] = NUM_DIM-1;
            
            continue;
        }
        
        //-- Case III: 2-interval --
        r1[i][0] = i; //in simj, start by scanning first unproc dim        
        r1[i][1] = NUM_DIM-1;
        
        r2[i][0] = 0;
        r2[i][1] = i - 1;
        
        r3[i][0] = 0;
        r3[i][1] = -1;
    }
    
    //-- print scan table --
    printf("\n--- scan table --");
    for (int i = 0; i <= NUM_DIM; i++)
    {
        printf("\n  d=%2d: r1=[%2d,%2d] r2=[%2d,%2d] r3=[%2d,%2d]", i, r1[i][0], r1[i][1], r2[i][0], r2[i][1], r3[i][0], r3[i][1]);
    }
    printf("\n--- ------ --\n\n");
    
   
    //getchar(); 
}



//-----------------------------------------
void Util::simpleJoin(pPoint A, int frA, int toA, pPoint B, int frB, int toB, resCont *result)
{
	for (int i = frA; i <= toA; i++)
	{
		pPoint p = &A[i];

		for (int j = frB; j <= toB; j++)
		{
			pPoint q = &B[j];

			REAL sum = 0;
            
            
            

			/*for (int k = NUM_DIM/2; k < NUM_DIM; k++) // NUM_DIM/2
			{
				REAL dx = (p->x[k] - q->x[k]);
				dx = dx * dx;
				sum += dx;
				
				if (sum > eps2)
					goto stop1;
			}
			
			for (int k = 0; k < NUM_DIM/2; k++) // NUM_DIM/2
			{
				REAL dx = (p->x[k] - q->x[k]);
				dx = dx * dx;
				sum += dx;

				if (sum > eps2)
					goto stop1;
			}*/
			
			for (int k = 0; k < NUM_DIM; k++)
			{
				REAL dx = (p->x[k] - q->x[k]);
				dx = dx * dx;
				sum += dx;
				
				if (sum > eps2)
					goto stop1;
			}

			//printf("\nUtil::simpleJoin() q1");
			//getchar();
			//p->relPoint->insert(q);
			result->push_back(p->id);
			result->push_back(q->id);

			stop1: ;
		}
	}

	return;
}


//-----------------------------------------
void Util::simpleJoin2(pPoint A, int frA, int toA, pPoint B, int frB, int toB, resCont *result, int m)
{
    int r1_beg = r1[m][0];
    int r1_end = r1[m][1];
    
//    int r2_beg = r2[m][0];
    int r2_end = r2[m][1];
    
    int r3_beg = r3[m][0];
    int r3_end = r3[m][1];
    
    
    if (r3_end == -1) // 1 or 2 intervals
    {
        if (r2_end == -1) // 1 interval
        {
            simpleJoin(A, frA, toA, B, frB, toB, result);

            return;
        }
        
        //-- 2 intervals --
        for (int i = frA; i <= toA; i++)
        {
            pPoint p = &A[i];
            
            for (int j = frB; j <= toB; j++)
            {
                pPoint q = &B[j];
                
                REAL sum = 0;
                
                //-- scan over range 1 --
                for (int k = r1_beg; k <= NUM_DIM-1; k++)
                {
                    REAL dx = (p->x[k] - q->x[k]);
                    dx = dx * dx;
                    sum += dx;
                    
                    if (sum > eps2)
                        goto stop_2_int;
                }
                
                //-- scan over range 2 --
                for (int k = 0; k <= r2_end; k++)
                {
                    REAL dx = (p->x[k] - q->x[k]);
                    dx = dx * dx;
                    sum += dx;
                    
                    if (sum > eps2)
                        goto stop_2_int;
                }
                
                
                
                
                result->push_back(p->id);
                result->push_back(q->id);
                
            stop_2_int: ;
                
            }
        }
        
        return;
    }
    
    
    //-- 3 intervals --
	for (int i = frA; i <= toA; i++)
	{
		pPoint p = &A[i];
        
		for (int j = frB; j <= toB; j++)
		{
			pPoint q = &B[j];
            
			REAL sum = 0;
            
            //-- scan over range 1 --
            for (int k = r1_beg; k <= r1_end; k++)
            {
                REAL dx = (p->x[k] - q->x[k]);
                dx = dx * dx;
                sum += dx;
                 
                if (sum > eps2)
                    goto stop2;
            }
            
            //-- scan over range 2 --
            for (int k = 0; k <= r2_end; k++)
            {
                REAL dx = (p->x[k] - q->x[k]);
                dx = dx * dx;
                sum += dx;
                
                if (sum > eps2)
                    goto stop2;
            }
			
            //-- scan over range 3 --
            for (int k = r3_beg; k <= NUM_DIM-1; k++)
            {
                REAL dx = (p->x[k] - q->x[k]);
                dx = dx * dx;
                sum += dx;
                
                if (sum > eps2)
                    goto stop2;
            }
            
			//printf("\nUtil::simpleJoin() q1");
			//getchar();
			//p->relPoint->insert(q);
            
                    
			result->push_back(p->id);
			result->push_back(q->id);
            
        stop2: ;
            
		}
	}
    
	return;
}


//-----------------------------------------
void Util::simpleBlockJoin(pPoint A, int A_sz, pPoint B, int B_sz, resCont *result, int block_sz)
{
    int num_block = ceil (float(B_sz) / block_sz);
    
    for(int k = 0; k < num_block; k++)
    {
        int fr_j = k * block_sz;
        int to_j = __min(fr_j + block_sz, B_sz);
        
        for (int i = 0; i < A_sz; i++)
        {
            pPoint p = &A[i];
        
            
            for (int j = fr_j; j < to_j; j++)
            {
                //if (rnd() > 0.5) continue;
                
                pPoint q = &B[j];
                
                REAL sum = 0;
                
                
                for (int k = 0; k < NUM_DIM; k++)
                {
                    REAL dx = (p->x[k] - q->x[k]);
                    dx = dx * dx;
                    sum += dx;
                    
                    if (sum > eps2)
                        goto stop1;
                }
                
                result->push_back(p->id);
                result->push_back(q->id);
                
            stop1: ;
            }
        }
    }
    
	return;
}

//-----------------------------------------
void Util::simpleBlockJoin2(pPoint A, int A_sz, pPoint B, int B_sz, resCont *result, int block_sz)
{
    int num_blk_A = A_sz / block_sz;
    int num_blk_B = B_sz / block_sz;
    
    for (int m = 0; m < num_blk_A; m++)
    {
        int fr_i = m * block_sz;
        int to_i = __min(fr_i + block_sz, A_sz);
        
        for(int k = 0; k < num_blk_B; k++)
        {
            int fr_j = k * block_sz;
            int to_j = __min(fr_j + block_sz, B_sz);
            
            for (int i = fr_i; i < to_i; i++)
            {
                pPoint p = &A[i];
                
                
                for (int j = fr_j; j < to_j; j++)
                {
                    //if (rnd() > 0.5) continue;
                    
                    pPoint q = &B[j];
                    
                    REAL sum = 0;
                    
                    
                    for (int k = 0; k < NUM_DIM; k++)
                    {
                        REAL dx = (p->x[k] - q->x[k]);
                        dx = dx * dx;
                        sum += dx;
                        
                        if (sum > eps2)
                            goto stop1;
                    }
                    
                    result->push_back(p->id);
                    result->push_back(q->id);
                    
                stop1: ;
                }
            }
        }
    }
    
	return;
}



int pcmp(const void *v1, const void *v2)
{
	pPoint p1 = (pPoint)v1;
	pPoint p2 = (pPoint)v2;

	for (int i = 0; i < NUM_DIM; i++)
	{
		int d = ((int) (p1->x[i]/Util::eps)) - ((int) (p2->x[i]/Util::eps));

		if (d != 0)
			return d;
	}

	return 0;
}


//--------------------------------------------------------------------------------
void Util::add_job(pJobs pGQ, int frA, int toA, int frB, int toB, int start_dim)
{
	
    //lock(&lock);
	
	//pGQ->add(frA, toA, frB, toB, start_dim);
	
	//if (frA > toA) { printf("\n add_job: q1"); exit(-1); }
	//if (frB > toB) { printf("\n add_job: q2"); exit(-1); }
	//if (start_dim > 8) { printf("\n add_job: q3"); exit(-1); }
	//if (Q->size() % 5 != 0) { printf("\n add_job: q4"); exit(-1); }
	
	//unlock(&lock);
}


//--------------------------------------------------------------------------------
int Util::get_job(pJobs GQ, int &frA, int &toA, int &frB, int &toB, int &start_dim, ThreadParam *param)
{
	//printf("\n -- Thread %d: [gj] wait(S_data) -- ", param->tid);
    
	lock(&gdata);
    
    //printf("\n -- Thread %d: [gj] got(S_data) -- ", param->tid);
    
    
	int *num_jobless = param->num_jobless;
	
	(*num_jobless) ++;
    
    //printf("\n -- Thread %d: [gj] N_jbls =%d -- ", param->tid, *num_jobless);

	
	goto next_step;
	
beginning:
    
    //printf("\n -- Thread %d: [gj] wait(S_data) p2 -- ", param->tid); //getchar();

	
	lock(&gdata);
    
    //printf("\n -- Thread %d: [gj] got(S_data) p2 -- ", param->tid); //getchar();
	
next_step:		
	
	if (GQ->num_jobs == 0) //Any jobs left?
	{	
		//printf("\n -- Thread %d: [gj] NO_JOBS, N_jbls =%d -- ", param->tid, *num_jobless);
		
		if ((*num_jobless) >= param->num_threads) //Is any thread still working? 
		{
            //-- no jobs & no one is working, so prepare to quit --
            //printf("\n -- Thread %d: [gj] ALL_JOBLESS, N_jbls =%d -- ", param->tid, *num_jobless);
        
            trylock(&need_job); //release other threads who might wait on need_job 
            unlock(&need_job); 
            
            unlock(&gdata);
			
            //printf("\n -- Thread %d: [gj] unlocked S_data, QUITTING!!! -- ", param->tid);
			
			return U_QUIT;
		}
		
		//-- At least one more thread is still working --
		unlock(&gdata);
        
        //printf("\n -- Thread %d: [gj] wait(S_need_job) -- ", param->tid);
        
        //-- time-limited wait for jobs to appear --
        for (int i = 0; i < 100; i++)
        {
            if (trylock(&need_job) == 0)
                break; // got lock!!!
            
            usleep(1000); // 1 ms
        }
        //lock(&need_job);
        
        //-- a new job *might* have appeared, recheck --
        
        //printf("\n -- Thread %d: [gj] got(S_need_job) -- ", param->tid);
        
		goto beginning;
	}
	
	/*if (GQ->num_jobs <= 0)
	{
		printf("\n --Util::get_job-- njbs = %d:", GQ->num_jobs);
		exit(-1);
	}*/
	
	//-- Q is not empty --
	GQ->get(&frA, &toA, &frB, &toB, &start_dim);
	
	//if (frA > toA) { printf("\n get_job: q1"); exit(-1); }
	//if (frB > toB) { printf("\n get_job: q2"); exit(-1); }
	//if (start_dim > 8) { printf("\n get_job: q3"); exit(-1); } 
    //if (Q->size() % 5 != 0) { printf("\n get_job: q4"); exit(-1); }
	
	(*num_jobless) --;
	
	if (GQ->num_jobs > 0 && num_jobless > 0)
	{
		trylock(&need_job);
		unlock(&need_job);
	}

	unlock(&gdata);
	
	return U_OK;
}

 


//-------------------------------------  
REAL Util::rnd()  
{  
	const double d2p31m = 2147483647;  
	const double d2p31  = 2147483711.0;  
  
	static double seed = 1234567.0; //init only one time  
  
	seed = 16807 * seed - floor(16807 * seed / d2p31m) * d2p31m;  
  
	return (REAL)(fabs(seed / d2p31));   
}   
  
//-------------------------------------   
REAL Util::rndNormal01()   
{  
	double v1, v2, s;  
  
	do  
	{  
		v1 = 2 * rnd() - 1;  
		v2 = 2 * rnd() - 1;  
  
		s = v1 * v1 + v2 * v2;  
	}  
	while (s >= 1);  
  
	return (REAL)(v1 * sqrt((- 2 * log(s)) / s));  
}  
  
//-------------------------------------   
REAL Util::rndNormal(REAL mean, REAL stddev)  
{  
	double r;  
  
	while (true)  
	{  
		r = stddev * rndNormal01() + mean;  
  
		if (r >= 0 && r <= 1)  
			return (REAL)r;  
	}  
}  
 
  
  
//-------------------------------------  
Point* Util::genPoint(int num_point)  
{  
	//printf(" uniform point "); 
 
	Point *pointArr = new Point[num_point + 1];  
  
	for (int i = 0; i < num_point; i++)  
	{  
		pPoint p = & pointArr[i];

		for (int i = 0; i < NUM_DIM; i++)
		{
			p->x[i] = rnd();
		}
	}  
  
	pointArr[num_point].x[0] = -1;  //absolete??
  
	return pointArr;  
}   
  
//-------------------------------------   
/*PList*  Util::genQList(int num_query)  
{  
	PList *qlistArr = new PList[num_query];  
	  
	return qlistArr;  
} */ 
  
//-------------------------------------    
Point* Util::genSkewedPoint(int num_point)   
{  
	printf(" skewed-point "); 
 
	int point_in_cluster = num_point / 5; 
  
	//-- create array for points -- 
	Point *pointArr = new Point[num_point + 1];   
  
	//-- init cluster centers -- 

	//-- Center 1 --
	Point *center1 = genCenter(0.2f, 0.2f);  
	Point *center2 = genCenter(0.8f, 0.2f);  
	Point *center3 = genCenter(0.2f, 0.8f);  
	Point *center4 = genCenter(0.8f, 0.8f);  
	Point *center5 = genCenter(0.5f, 0.5f);  
  
	//-- create clusters -- 
	genPointCluster(center1, CLUSTER_STDDEV, pointArr,                    0, point_in_cluster);  
	genPointCluster(center2, CLUSTER_STDDEV, pointArr,     point_in_cluster, point_in_cluster);  
	genPointCluster(center3, CLUSTER_STDDEV, pointArr, 2 * point_in_cluster, point_in_cluster);  
	genPointCluster(center4, CLUSTER_STDDEV, pointArr, 3 * point_in_cluster, point_in_cluster);  
	genPointCluster(center5, CLUSTER_STDDEV, pointArr, 4 * point_in_cluster, point_in_cluster);  
   
	pointArr[5 * point_in_cluster].x[0] = -1;   //absolete?
 
	delete center1; 
	delete center2; 
	delete center3; 
	delete center4; 
	delete center5; 
   
	return pointArr;   
}    
//-------------------------------------    
Point* Util::genCenter(REAL x, REAL y)   
{
	int i;

	pPoint p = new Point();

	for (i = 0; i < NUM_DIM; i+=2)
	{
		p->x[i] = x;
	}

	for (i = 1; i < NUM_DIM; i+=2)
	{
		p->x[i] = y;
	}

	return p;
}


  
//-------------------------------------    
void Util::genPointCluster(Point *center, REAL std_dev, Point *pointArr, int from, int size)  
{  
	int to = from + size;  
  
	for (int i = from; i < to; i++)  
	{  
		pPoint p = &pointArr[i];

		for (int i = 0; i < NUM_DIM; i++)
		{
			p->x[i] = rndNormal(center->x[i], std_dev);
		}
	}  
}  
  
