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

void print_array(double *data,int num) {
  int i;
  for(i=0;i<num;i++) {
    printf("%lf\n",10*log10(data[i]));
  }
}

int main() {
    double ran1(int *);/*random value generator [0,1)*/
    double dierfc(double );/* inverse of error function in double precision */
    double awgn(int *);/*normal distribution random value generator*/
    double awgn_power(int *);/*noise power*/
    double dB2real(double dBm);
    double pathloss(double);
    double poidev(double ave,int *idum);
    double fading_dB(int *idum);/*fading function*/
    double shadowing(int *idum);/*log-normal shadowing function*/
    void channel(double *,int,int,int*);

    int count_ON(int *,int );
    int count_OFF2OFF(int *,int );
    int count_ON2ON(int *,int );
    int count_ON2OFF(int *,int );
    int count_OFF2ON(int *,int );

    int i,j;
    int sn;
    int L;
    
    double Rx_Power;
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
    int error = 0;

    int correct_transition = 0;
    int tmp1=0,tmp2;
    int bool=0;
    double gamma,myu;
    int FFTPOINT;
    int iter;
    double ave_upload=0.0,ave_upload_transition=0.0;
    double var_upload=0.0,var_upload_transition=0.0;

    int s_data[BIT];
    double s_sig[BIT];
    double r_sig[BIT];
    int r_data[BIT];

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
    
    for (L=MIN_DIST;L<=MAX_DIST;L=L+10) {
      //channel(Statistic,L,AWGN,&p);
      //channel(Statistic,L,FADING,&p);
      channel(Statistic,L,AWGN_FADING,&p);
      print_array(Statistic,SAMPLE);
      //printf("Distance: %d [m],Rx_Power=%lf \n",L,Rx_Power);
    }



    return 0;
}
