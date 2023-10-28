//Copyright (c) 2013, Dmitri V. Kalashnikov. All rights reserved.
//This copyright notice should remain at the top of this file.
//

#ifndef CONST_H
#define CONST_H 


#define REAL float

#ifndef NULL
const int NULL = 0; 
#endif

const REAL CLUSTER_STDDEV	= 0.05f;
const REAL ONE_MINUS_EPS	= 0.99999f; //0.999999999f;

const int NUM_DIM	= 32;
const int Z_DIM		= NUM_DIM - 2;
const int MINLEN 	= 24;//32;


//MNIDT     : 60K 784D
//Aerial40  : 275K 60D
//ColorHist : 68K 32D
//ColorMom  : 68K 9D
//CoocText  : 68K 16D
//LayHist   : 66K 32D
//MG        : 27K 2D
//LB        : 36K 2D


//#define DATA_FILE	"uni6d.txt"
//#define DATA_FILE	"../../f784.txt"
//#define DATA_FILE	"../../Aerial40.txt"
#define DATA_FILE	"../../ColorHist.txt"
//#define DATA_FILE	"../../ColorMom.txt"
//#define DATA_FILE	"../../CoocTexture.csv"
//#define DATA_FILE	"../../LayHist.csv"
//#define DATA_FILE	"../../MG.csv"
//#define DATA_FILE	"../../LB.csv"


#ifndef __max
#define __max(a,b) ( ((a) > (b))? (a) : (b) )
#endif


#ifndef __min
#define __min(a,b) ( ((a) < (b))? (a) : (b) )
#endif


#endif
