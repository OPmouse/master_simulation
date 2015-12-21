//
//  main.c
//  For_interim
//
//  Created by 王昊 on 2015/06/02.
//  Copyright (c) 2015年 王昊. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "param.h"

char TRAFFIC[204800000];

int main() {
    double ran1(int *);/*random value generator [0,1)*/
    double dierfc(double );/* inverse of error function in double precision */
    double awgn(int *);/*normal distribution random value generator*/
    double awgn_power(int *);/*noise power*/
    double dB2real(double dBm);
    double pathloss(double);
    double poidev(double ave,int *idum);

    int count_ON(int *,int );
    int count_OFF2OFF(int *,int );
    int count_ON2ON(int *,int );
    int count_ON2OFF(int *,int );
    int count_OFF2ON(int *,int );

    int i,j;
    int sn;
    double Statistic[SAMPLE];
    double Statistic_transition[SAMPLE];
    
    int Decision_data[iteration];
    int Decision_data_transition[iteration];
    
    double ReceivedPower;
    double RealReceivedPower;
    double Statistic_value = 0.0,StatisticTransition_value = 0.0;
    
    double Upload_value = 0.0,UploadTransition_value = 0.0;
    double Upload_value_2 = 0.0,UploadTransition_value_2 = 0.0;
    
    double threshold;
    int correct = 0;
    int correct_transition = 0;
    int tmp1=0,tmp2;
    int bool=0;
    double gamma,myu;
    int FFTPOINT;
    int iter;
    double ave_upload=0.0,ave_upload_transition=0.0;
    double var_upload=0.0,var_upload_transition=0.0;

    /*time initialization*/
    int p,n;
    double r;
    time_t t;
    n=-((unsigned int)time(&t));
    p=n;
    r=ran1(&p);

    FFTPOINT=2048;

    myu = 1.0/(double)AVERAGE_ON;
    gamma = 1.0/(double)AVERAGE_OFF;
  
    threshold = sqrt(2.0)*NOISE_SINTI*dierfc(2*P_FA)/sqrt((double)SAMPLE)+NOISE_SINTI;
    
    //sn=5;
    printf("sn = %d,usage = %lf\n",sn,(double)AVERAGE_ON/(double)(AVERAGE_ON+AVERAGE_OFF));
    
    for(sn=-30;sn<=50;sn=sn+2){
      //for(FFTPOINT=100;FFTPOINT<=2000;FFTPOINT=FFTPOINT+100){
      ReceivedPower = NOISE_POWER + (double)sn;
      RealReceivedPower = dB2real(ReceivedPower);
      
      for(iter=0;iter<trial;iter++){
	/****************Always ON**********************/
	for (i=0; i<iteration; i++) {
	  for (j=0; j<FFTPOINT; j++) {
	    Statistic[j] = RealReceivedPower + awgn_power(&p);
	    Statistic_value += Statistic[j];
	  }
	  Statistic_value /=(double)FFTPOINT;
	  //printf("Statistic=%lf\n",10*log10(Statistic_value));
	  if (Statistic_value > threshold) {
	    Upload_value += Statistic_value;
	    correct++;
	    Decision_data[i] = 1;
	  }else{
	    Decision_data[u] = 0;
	  }
	  Statistic_value=0.0;
	}
	ave_upload += Upload_value/(double)correct;
	
	/****************Always ON**********************/
	
	/************************************transition part********************************/
	for(i=0;i<iteration*FFTPOINT*Ts;i++){
	  /*traffic generation part*/
	  if(poidev(myu,&p)!= 0){
	    //printf("arrive=%d\n",i);
	    tmp2=i;
	    for(j=tmp1;j<tmp2;j++){
	      TRAFFIC[j] = 0;
	    }
	    tmp1=tmp2;
	    bool=1;
	  }
	  if(poidev(gamma,&p)!= 0){
	    //printf("depart=%d\n",i);
	    tmp2=i;
	    for(j=tmp1;j<tmp2;j++){
	      TRAFFIC[j] = 1;
	    }
	    tmp1=tmp2;
	    bool=0;
	  }
	}//iteration*MAX_SIZE
	if(bool){
	  for(i=tmp1;i<iteration*FFTPOINT*Ts;i++){
	    TRAFFIC[i] = 1;
	  }
	}else{
	  for(i=tmp1;i<iteration*FFTPOINT*Ts;i++){
	    TRAFFIC[i] = 0;
	  }
	}
	/*traffic generation part*/
	/************************************transition part********************************/
	
	/************************************sensing start**********************************/
	for(i=0;i<iteration;i++){
	  for(j=i*FFTPOINT*Ts;j<(i+1)*FFTPOINT*Ts;j=j+Ts){
	    if(TRAFFIC[j] == 1){
	      StatisticTransition_value += RealReceivedPower + awgn_power(&p);
	    }else{
	      StatisticTransition_value += awgn_power(&p);
	    }
	    //printf("Rx Pow = %lf\n",10*log10(Statistic_transition[j]));
	  } 
	  StatisticTransition_value /=(double)FFTPOINT;
	  //printf("%d,%d,%lf\n",sn,FFTPOINT,10*log10(StatisticTransition_value));
	  if (StatisticTransition_value > threshold) {
	    UploadTransition_value += StatisticTransition_value;
	    correct_transition++;
	    Decision_data_transition[i] = 1;
	  }else{
	    Decision_data_transition[i] = 0;
	  }
	  StatisticTransition_value = 0.0;
	}
	
	ave_upload_transition += UploadTransition_value/(double)correct_transition;
	
	
	//printf("ReceivedPower=%lf\n",ReceivedPower);
	//printf("%d,%lf\n",sn,(double)correct/(double)iteration);
	//printf("%d,%lf\n",FFTPOINT,10*log10(Upload_value)-ReceivedPower);
	
	UploadTransition_value = 0.0;
	Upload_value = 0.0;
	
	UploadTransition_value_2 = 0.0;
	Upload_value_2 = 0.0;
	correct_transition = 0;
	correct=0;
      }
      
      /************************************sensing end**********************************/
      //printf("%d,%lf,%lf\n",sn,var_upload_transition,ReceivedPower);
      
      //SNR estimation printf
      printf("%d,%lf,%lf\n",sn,10*log10(ave_upload/10.0)-NOISE_POWER,10*log10(ave_upload_transition/10.0)-NOISE_POWER);
      //printf("%d,%lf,%lf\n",FFTPOINT,10*log10(var_upload/1000.0),10*log10(var_upload_transition/1000.0));
      ave_upload = 0.0;ave_upload_transition = 0.0,var_upload=0.0,var_upload_transition=0.0;
    }
    return 0;
}
