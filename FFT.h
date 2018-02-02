#ifndef _FFT_
#define _FFT_

#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>



#include "redpitaya/rp.h"
#include "globals.h"



 /* Recursive Algorithim of FFT */


typedef double complex cplx;
/* start FFT algorithm*/ 
void _fft(cplx buf[], cplx out[], uint32_t n, uint32_t step)
{
	if (step < n) {
		_fft(out, buf, n, step * 2);
		_fft(out + step, buf + step, n, step * 2);
        uint32_t i;
		for (i = 0; i < n; i += 2 * step) {
			cplx t = cexp(-I * PI * i / n) * out[i + step];
			buf[i / 2]     = out[i] + t;
			buf[(i + n)/2] = out[i] - t;
		}
	}
}
 
void fft(cplx buf[], uint32_t n)
{
	cplx out[n];
	
	uint32_t i ;
	
	for (i = 0; i < n; i++) out[i] = buf[i];
 
	_fft(buf, out, n, 1);
}

/* end FFT algorithm*/ 


/* Convert the data from float to complex float*/ 
void filler(cplx *buf,float* windADC, uint32_t samples_number)
{
   uint32_t i=0;
   
   for(i=0;i<samples_number;i++)
	   
   	buf[i]=windADC[i];
} 

/* print out the sampled and windowed singal signal*/ 
void sampler_print(float* Reff,float* etalon,char* Reffile,char* Etafile,uint32_t sample_number )
{
        FILE* eta;
        FILE* ref;
        eta=fopen(Reffile,"w");
        ref=fopen(Etafile,"w");
        uint32_t i;
        for (i=0;i<sample_number;i++)
        {
                fprintf(eta,"%f\n",Reff[i]);
                fprintf(ref,"%f\n",etalon[i]);
        }

        fclose(eta);
        fclose(ref);
}



/* print out the FFT  signal*/ 
void Result_output(cplx reff[],cplx etalon[],uint32_t samples_number)
{
	/* Channel 1 reff; Channel 2 for etalon		*/
	
	FILE* ref;
	
	FILE* eta;
	
	
	ref=fopen("Ref_FFT.txt","w");
	
	eta=fopen("Etalon_FFT.txt","w");
	
	uint32_t i;
	
	for (i=0;i<samples_number;i++)
	{
		fprintf(ref,"%f\n",cabsf(reff[i]));
		
		fprintf(eta,"%f\n",cabsf(etalon[i]));
	}

	fclose(ref);
	
	fclose(eta);
	
}
#endif

