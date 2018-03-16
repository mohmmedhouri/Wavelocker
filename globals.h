#ifndef GLOBAL_PARAMETER
#define GLOBAL_PARAMETER

#include "commands.h"
const float PI=3.141592653589793238460;
float RedMsg=0;
float BluMsg=0;

struct tracking_data {
  
float power;
float etalon;
float ref;



};

struct channel_info {
   char CurStat; // current status
   int selCH [58];
   int PTON  [58];
   int PTDur [58];
   int PTFrq [58];

   struct tracking_data obj [ 255 ];
   
 //  tracking_data  Tinfo [255];
   /* declare as many members as desired, but the entire structure size must be known to the compiler. */
};



int OptiPoint (struct tracking_data obj [ ],int size)

{
   int max=0;
	int i;
	for (i=1;i<size;i++){
	if(obj[max].power<obj[i].power)
	 {

	max=i;
	
	}

	}

return	max;


}

//tracking_data sampling_process ();

#endif
