#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include <string.h>



#include "commands.h"
#include "FFT.h"
#include "windows.h"
#include "redpitaya/rp.h"


uint32_t Num_SAMPLES= 8192;
int AVR_windows= 101; 











int main(int argc, char **argv){

        /* Print error, if rp_Init() function failed */
        if(rp_Init() != RP_OK)
		{
			fprintf(stderr, "Rp api init failed!\n");
        }
		
		rp_DpinSetDirection (RP_DIO4_N, RP_OUT);
		rp_DpinSetState(RP_DIO4_N,RP_LOW);
	  	struct channel_info CH1;
		memcpy(CH1.selCH, setch28, sizeof (setch28));
		memcpy(CH1.PTON, set_durr_on, sizeof(set_durr_on));
		memcpy(CH1.PTDur, set_period_on, sizeof(set_period_on));
		CH1.CurStat='0';
		int c; // couter for loops 
		float avrRef=0;
		float avrEta=0;
		float avr=0;

	float Demoarray [5];

	for (c=0;c<255;c++)
	CH1.obj[c].power=0;			

		
        float *buff  = (float *)malloc(Num_SAMPLES * sizeof(float)); // Allocate space for for 1st ADC 1 for reference 

        float *buff2 = (float *)malloc(Num_SAMPLES * sizeof(float)); // Allocate space for for 2nd ADC 2 for Etalon
	
		cplx *buf = (cplx *)malloc(Num_SAMPLES * sizeof(cplx));

		cplx *buf2 = (cplx *)malloc(Num_SAMPLES* sizeof(cplx));

	while(1)
	{
		
		 switch(CH1.CurStat) {
      
	  case '0' :  //  not channel found
        
       		rp_AcqReset();
		
		rp_AcqSetDecimation(RP_DEC_1024);
	  
		rp_AcqSetSamplingRate(RP_SMP_122_070K);
		
		
                seq_gen_mc(CH1.selCH,810,1);

                usleep(1000);

                seq_gen_mc(CH1.PTON,810,1);

                usleep(1000);
		
		seq_gen_mc(PtFrq505,810,1);
		
		usleep(1000);
		
                seq_gen_mc(CH1.PTDur,810,1);

                usleep(1000);

                seq_gen_mc(ptmd32,810,1);

		usleep(1000);
	
		rp_AcqStart();
        
		
        /* After acquisition is started some time delay is needed in order to acquire fresh samples in to buffer*/

	
		usleep(68800);
	 
		rp_AcqStop();

        	
		rp_AcqGetOldestDataV(RP_CH_1, &Num_SAMPLES, buff);
	
		
		rp_AcqGetOldestDataV(RP_CH_2, &Num_SAMPLES, buff2);

	
		filler(buf,buff,Num_SAMPLES);
		   
		fft(buf,Num_SAMPLES);
		
       		filler(buf2,buff2,Num_SAMPLES);

       		fft(buf2,Num_SAMPLES);
		
		int j=3389;
		int i;

		float AVR_ref=maxia(cabsf(buf[j]),cabsf(buf[j-1]),cabsf(buf[j+1]));
		if(AVR_ref > 2 )
		{
		  printf("New channel was found\n");
		  CH1.CurStat='1';
		}
		else 
		{
		  printf("%f  No channel was found we resend the data again\n",AVR_ref);

		}

		break ;

	case '1' : // tunning
		
		for (i=128;i<168;i++)
		{	
			avrRef=0;
			avrEta=0;
			avr=0;
			printf("Postive shift Stage \n ");
			for (c=0;c<50;c++)
			{
			
			rp_AcqReset();

               		 rp_AcqSetDecimation(RP_DEC_1024);

                	rp_AcqSetSamplingRate(RP_SMP_122_070K);

		
              	  	rp_AcqStart();


                	usleep(68800);

                	rp_AcqStop();


                	rp_AcqGetOldestDataV(RP_CH_1, &Num_SAMPLES, buff);


                	rp_AcqGetOldestDataV(RP_CH_2, &Num_SAMPLES, buff2);


                	filler(buf,buff,Num_SAMPLES);

                	fft(buf,Num_SAMPLES);

                	filler(buf2,buff2,Num_SAMPLES);

                	fft(buf2,Num_SAMPLES);
					
					avrRef = avrRef+ maxia(cabsf(buf[j]),cabsf(buf[j-1]),cabsf(buf[j+1]));


					avrEta = avrEta + maxia(cabsf(buf2[j]),cabsf(buf2[j-1]),cabsf(buf2[j+1]));
					
					avr= avr + (avrEta/avrRef);
			
			
			}
			
			CH1.obj[i].ref =    avrRef/50;

			CH1.obj[i].etalon = avrEta/50;

			CH1.obj[i].power =  avr/50;
			
			seq_gen_mc(PosShi20,810,1);
			
			printf("%f  %f  %f \n",CH1.obj[i].ref,CH1.obj[i].etalon,CH1.obj[i].power);

		} // end of for or positive tune

		for (i=0;i<40;i++)
		{
		 	seq_gen_mc(Negshi20,810,1);
			usleep(1000000);
		} 

		 CH1.CurStat='2';

		break; // end of positve tune stage
	
	case '2' :

                usleep(100);


		
		 for (i=127;i>87;i--)
         {
			 avrRef=0;
			 avrEta=0;
			 avr=0;
			 
			printf("negative shift stage\n");
			for (c=0;c<50;c++)
		{
                        rp_AcqReset();

                         rp_AcqSetDecimation(RP_DEC_1024);

                        rp_AcqSetSamplingRate(RP_SMP_122_070K);


                        rp_AcqStart();


                        usleep(68800);

                        rp_AcqStop();


                        rp_AcqGetOldestDataV(RP_CH_1, &Num_SAMPLES, buff);


                        rp_AcqGetOldestDataV(RP_CH_2, &Num_SAMPLES, buff2);


                        filler(buf,buff,Num_SAMPLES);

                        fft(buf,Num_SAMPLES);

                        filler(buf2,buff2,Num_SAMPLES);

                        fft(buf2,Num_SAMPLES);
						
					avrRef = avrRef+ maxia(cabsf(buf[j]),cabsf(buf[j-1]),cabsf(buf[j+1]));

					avrEta = avrEta + maxia(cabsf(buf2[j]),cabsf(buf2[j-1]),cabsf(buf2[j+1]));
					
					avr= avr + (avrEta/avrRef);
			
			
			}
			
			CH1.obj[i].ref =    avrRef/50;

			CH1.obj[i].etalon = avrEta/50;

			CH1.obj[i].power =  avr/50;
			
			seq_gen_mc(Negshi20,810,1);
			printf("%f  %f  %f \n",CH1.obj[i].ref,CH1.obj[i].etalon,CH1.obj[i].power);
	
                       

            } // end of for or negative tune
			
			for (i=0;i<40;i++)
			{
				seq_gen_mc(PosShi20,810,1);
				usleep(1000000);

			}

	 CH1.CurStat='3';

         break; // end of positve tune stage

	case '3':
	
	printf("now we are in last stage\n");
			
	int data =OptiPoint(CH1.obj,255);
	printf("location of the maximum %d \n",data);

	for (int i=0;i<255;i++)
	{
		printf("location %d, %f,%f,%f \n" ,i,CH1.obj[i].ref,CH1.obj[i].etalon,CH1.obj[i].power);
	}
	int differ=-128+data;
	if (differ<0)
	{
			
	do 
	{
			seq_gen_mc(Negshi20,810,1);
			avr=0;
		for (i=0 ;i<50;i++)
		{
		  rp_AcqReset();

					   rp_AcqSetDecimation(RP_DEC_1024);

					   rp_AcqSetSamplingRate(RP_SMP_122_070K);


					   rp_AcqStart();


					   usleep(68800);

					  rp_AcqStop();


					  rp_AcqGetOldestDataV(RP_CH_1, &Num_SAMPLES, buff);


					 rp_AcqGetOldestDataV(RP_CH_2, &Num_SAMPLES, buff2);

					 filler(buf,buff,Num_SAMPLES);

					fft(buf,Num_SAMPLES);

					filler(buf2,buff2,Num_SAMPLES);

					fft(buf2,Num_SAMPLES);
				
			float REF=maxia(cabsf(buf[j]),cabsf(buf[j-1]),cabsf(buf[j+1]));


			float ETA=maxia(cabsf(buf2[j]),cabsf(buf2[j-1]),cabsf(buf2[j+1]));
			
			avr=avr+ETA/REF;
		}
		printf("%f \n",avr/50);
	}while((avr/50)<2.9);//modified
	// }while((avr/50)<50*CH1.obj[data+1].power/100);// end of do while dynamic
		
			
			
		
		/*
		for (i=0;i<abs(differ)*3/4;i++)
		{
			seq_gen_mc(Negshi20,810,1);
			usleep(68800);
		}
		*/
		
	} // end of if
	else
	{
		
		
		do 
	{
			seq_gen_mc(PosShi20,810,1);
			avr=0;
		for (i=0 ;i<50;i++)
		{
		  rp_AcqReset();

					   rp_AcqSetDecimation(RP_DEC_1024);

					   rp_AcqSetSamplingRate(RP_SMP_122_070K);


					   rp_AcqStart();


					   usleep(68800);

					  rp_AcqStop();


					  rp_AcqGetOldestDataV(RP_CH_1, &Num_SAMPLES, buff);


					 rp_AcqGetOldestDataV(RP_CH_2, &Num_SAMPLES, buff2);

					 filler(buf,buff,Num_SAMPLES);

					fft(buf,Num_SAMPLES);

					filler(buf2,buff2,Num_SAMPLES);

					fft(buf2,Num_SAMPLES);
				
			float REF=maxia(cabsf(buf[j]),cabsf(buf[j-1]),cabsf(buf[j+1]));


			float ETA=maxia(cabsf(buf2[j]),cabsf(buf2[j-1]),cabsf(buf2[j+1]));
			
			avr=avr+ETA/REF;
		}
		printf("%f \n",avr/50);
	}while(avr/50<2.9);//modified
	// }while((avr/50)<50*CH1.obj[data+1].power/100);// end of do while dynamic
		/*
		for (i=0;i<abs(differ)*3/4;i++)
		{
			seq_gen_mc(PosShi20,810,1);
			usleep(68800);
		}
		*/
	} // end of else
	
	
	
	
	
	for (c=0;c<5;c++)
	{
		avr=0;
		if(c==1 || c==2)
		{
			seq_gen_mc(PosShi20,810,1);
			usleep(68800);
		}
		else if(c==3)
		{
			seq_gen_mc(Negshi20,810,1);
			usleep(68800);
			seq_gen_mc(Negshi20,810,1);
			usleep(68800);
			seq_gen_mc(Negshi20,810,1);
			usleep(68800);
			
		}
		else if(c==4)
		{
			seq_gen_mc(Negshi20,810,1);
			usleep(68800);
		}
		
			 
        for (i=0 ;i<50;i++)
        {
         	 rp_AcqReset();

             rp_AcqSetDecimation(RP_DEC_1024);

             rp_AcqSetSamplingRate(RP_SMP_122_070K);


             rp_AcqStart();


             usleep(68800);

             rp_AcqStop();


             rp_AcqGetOldestDataV(RP_CH_1, &Num_SAMPLES, buff);


			 rp_AcqGetOldestDataV(RP_CH_2, &Num_SAMPLES, buff2);

   			 filler(buf,buff,Num_SAMPLES);

             fft(buf,Num_SAMPLES);

			 filler(buf2,buff2,Num_SAMPLES);
			 
			 fft(buf2,Num_SAMPLES);

             float REF=maxia(cabsf(buf[j]),cabsf(buf[j-1]),cabsf(buf[j+1]));


             float ETA=maxia(cabsf(buf2[j]),cabsf(buf2[j-1]),cabsf(buf2[j+1]));

             avr=avr+ETA/REF;
        }
			Demoarray[c]=avr/50;
	
	}// end of the for loop

	seq_gen_mc(PosShi20,810,1);
	usleep(68800);
	seq_gen_mc(PosShi20,810,1);
	usleep(68800);
		
	CH1.CurStat='4';

	break ;
	
	case '4':
	

	printf("tracking stage \n");

	avr=0;
	for (i=0 ;i<50;i++)
	{
	  rp_AcqReset();

                   rp_AcqSetDecimation(RP_DEC_1024);

                   rp_AcqSetSamplingRate(RP_SMP_122_070K);


                   rp_AcqStart();


                   usleep(68800);

                  rp_AcqStop();


                  rp_AcqGetOldestDataV(RP_CH_1, &Num_SAMPLES, buff);


                 rp_AcqGetOldestDataV(RP_CH_2, &Num_SAMPLES, buff2);

                 filler(buf,buff,Num_SAMPLES);

                fft(buf,Num_SAMPLES);

                filler(buf2,buff2,Num_SAMPLES);

                fft(buf2,Num_SAMPLES);
			
		float REF=maxia(cabsf(buf[j]),cabsf(buf[j-1]),cabsf(buf[j+1]));


		float ETA=maxia(cabsf(buf2[j]),cabsf(buf2[j-1]),cabsf(buf2[j+1]));
		
		avr=avr+ETA/REF;
	}
	
	for (c=0;c<5;c++)
		printf ("%d   %f   \n",c,Demoarray[c]);

		if((avr/50)>Demoarray[4])
		{
			printf("shift to positive\n");
			seq_gen_mc(PosShi20,810,1);
		}

		else if((avr/50)<Demoarray[2])
		{
			printf("shift to negative \n");
			seq_gen_mc(Negshi20,810,1);
		}
		else 
		{
			printf("still in locked stage \n");
		}
		printf("Demoarray %f \n",Demoarray[0]);
		printf("Current value %f \n",avr/50);

	 break ;
			
			
		
	}// end of switch

	
	}//end of while	
	
	
		
		printf("done data sent \n");
		free(buf);
		free(buf2);
		free(buff);
		free(buff2);

        rp_Release();
        return 0;
}
