#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "param.h"

double pathloss(double dist) {
  double pr;
  double lambda =(double) c / (double)Frequency;
  pr = 20.0*log10(lambda/(4*M_PI*ref_dis)) - 10.0*expon*log10((1+dist)/ref_dis);
  return pr;//[dB]
}

/*AWGNの電力　真値*/
double awgn_power(int *idum){
    double awgn(int *);
    double I,Q;
    double N_P;
    I=sqrt(NOISE_sinti)*awgn(idum);
    Q=sqrt(NOISE_sinti)*awgn(idum);
    //printf("%e,%e\n",I,Q);
    N_P=I*I+Q*Q;
    return N_P;
}

/*simplified fading function*/
double fading_dB(int *idum) {
  double ran1(int *idum);
  int i;
  double x = 0.0,y = 0.0;
  double rad;
  double fg;
  for (i=0;i<wave_number;i++) {
    rad = 2.0*M_PI*ran1(idum);
    x += cos(rad);
    y += sin(rad);
  }
  fg = (x*x + y*y)/wave_number;
  return fg;
}

/*log-normal shadowing function*/
double shadowing(int *idum) {
  double ran1(int *idum);
  double nvar = (double)SHADOWING;
  double x = 0.0,y;
  double shad_db = 0.0;
  double z;
  while (x == 0.0){
    x = ran1(idum);
  }
  y = ran1(idum);
  shad_db = sqrt(-2.0*nvar*log(x)*sin(2.0*M_PI*y));
  return shad_db;//[dB]
}

/*channel function including Pathloss,AWGN and Fading*/
void channel(double *Rx_P,int dist,int MODE,int *idum) {
  double dB2real(double dBm);
  double ran1(int *idum);
  int i;

  //only AWGN
  if(MODE == AWGN) {
    for(i=0;i<SAMPLE;i++) {
      Rx_P[i] = dB2real(Tx_Power + pathloss((double)dist))+awgn_power(idum);
      printf("%d,%lf\n",i,10*log10(Rx_P[i]));
    }//for
  }//if
  
  //only Fading
  if(MODE == FADING) {
    for(i=0;i<SAMPLE;i++) {
      Rx_P[i] = dB2real(Tx_Power + pathloss((double)dist) + fading_dB(idum));
    }//for
  }//if

  //AWGN+Fading
  if(MODE == AWGN_FADING) {
    for(i=0;i<SAMPLE;i++) {
      Rx_P[i] = dB2real(Tx_Power + pathloss((double)dist) + fading_dB(idum))+awgn_power(idum);
    }//for
  }//if
}
