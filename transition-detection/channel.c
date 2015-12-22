#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "param.h"

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

/*fading coexistence generation based on jakes model*/
double fading(int time,double f_D,int *idum){
  double ran1(int *);
  int i;
  double rx_real[wave_number];
  double rx_imaginary[wave_number];
  double rx_angle[wave_number];
  double theta[wave_number];
  double g_real=0.0;
  double g_imaginary=0.0;
  double g_m_real;
  double g_m_imaginary;
  double h;
  /*到来角*/
  rx_angle[0]=2*ran1(idum)*M_PI;
  for (i=1; i<wave_number;i++) {
    rx_angle[i]=rx_angle[0]+2*M_PI*(double)i/(double)wave_number;
  }
  /*素波の位相*/
  for (i=0; i<wave_number; i++) {
    theta[i]=2*ran1(idum)*M_PI;
  }
  
  for (i=0; i<wave_number; i++) {
    rx_real[i]     =cos(2*M_PI*cos(rx_angle[i]*f_D*(double)time/kizami)+theta[i]);
    rx_imaginary[i]=sin(2*M_PI*cos(rx_angle[i]*f_D*(double)time/kizami)+theta[i]);
  }
  for (i=0; i<wave_number; i++) {
    g_real+=rx_real[i];
    g_imaginary+=rx_imaginary[i];
  }
  /*複素係数の正規化*/
  g_m_real=g_real/sqrt((double)wave_number);
  g_m_imaginary=g_imaginary/sqrt((double)wave_number);
  
  h=pow(g_m_real, 2.0)+pow(g_m_imaginary, 2.0);
  return h;
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
  return 10*log10(fg);
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

