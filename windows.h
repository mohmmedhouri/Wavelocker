#ifndef WINDOWS_H
#define WINDOWS_H


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include <string.h>



#include "redpitaya/rp.h"
#include "globals.h"





/*		
Move average windows
input: sampled reference signal 
input: sampled etalon singal 
input: total number of sample_number
input: number of samples to be averaged

*/
void REC_AVR_WIN(float* ADC1_out ,float* ADC2_out, uint32_t  sample_number,int avr)
{

	float  medium1=0;
	float  medium2=0;
	int i;
	int j;
	
	/* avrage is an odd number*/
	float *ADC1_out_cpy = (float *)malloc(sample_number * sizeof(float));
	
	float *ADC2_out_cpy = (float *)malloc(sample_number * sizeof(float));
	
	memcpy( ADC1_out_cpy, ADC1_out,  sample_number);
	
	memcpy(ADC2_out_cpy, ADC2_out,  sample_number);
	
	/*     */
	for(i=0;i< avr;i++)
	{
		medium1=0;
		medium2=0;
		
		for(j=i;j>i-avr;j--)
		{
			if(j<0)
				continue;
			medium1=medium1+ADC1_out[j];
			medium2=medium2+ADC2_out[j];
			
		}
		 ADC1_out_cpy[i] = medium1/avr;
		 ADC2_out_cpy[i] = medium2/avr;	
	}
		
		
	for(i=avr;i< sample_number;i++)
	{
		medium1=medium1+ADC1_out[i]-ADC1_out[i-avr];
		medium2=medium2+ADC2_out[i]-ADC2_out[i-avr];
		
		ADC1_out_cpy[i] = medium1/avr;
		ADC2_out_cpy[i] = medium2/avr;
	}
	
	memcpy(ADC1_out ,ADC1_out_cpy ,  sample_number);
	
	memcpy(ADC2_out, ADC2_out_cpy,  sample_number);
	 
}


/*		
hann windows
input: sampled reference signal 
input: sampled etalon singal 
input: total number of sample_number
*/
void hanning_win(float* ADC1_out ,float* ADC2_out, uint32_t  sample_number)
{
	float  multiplier=0;
	int i;
	
	for (i = 0; i < sample_number; i++) 
	{
		multiplier = 0.5 * (1 - cos(2*PI*i/(sample_number-1)));
		ADC1_out[i] = multiplier * ADC1_out[i];
		ADC2_out[i] = multiplier * ADC2_out[i];
	}
	
	
}

/*		
nutall3a windows
input: sampled reference signal 
input: sampled etalon singal 
input: total number of sample_number
*/

void nutall3a(float* ADC1_out ,float* ADC2_out, uint32_t  sample_number)
{
	float  multiplier=0;
	int i;
	
	for (i = 0; i < sample_number; i++) 
	{
		multiplier =0.40897-0.5*cos(2*PI*i/sample_number)+0.09103*cos(4*PI*i/sample_number);
		ADC1_out[i] = multiplier * ADC1_out[i];
		ADC2_out[i] = multiplier * ADC2_out[i];
	}
	
	
}


/*		
welch windows
input: sampled reference signal 
input: sampled etalon singal 
input: total number of sample_number
*/

void welch(float* ADC1_out ,float* ADC2_out, uint32_t  sample_number)
{
	float  multiplier=0;
	int i;
	
	for (i = 0; i < sample_number; i++) 
	{
		multiplier =1-pow(((2*i/sample_number)-1),2);
		ADC1_out[i] = multiplier * ADC1_out[i];
		ADC2_out[i] = multiplier * ADC2_out[i];
	}
	
	
}







#endif
