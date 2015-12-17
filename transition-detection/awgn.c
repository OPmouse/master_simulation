/***********************************************
    AWGN.c
***********************************************/
#include <stdio.h>
#include <math.h>
#include"param.h"

#define PI 3.141592653
double ran1(int *);

/*AWGN amplitude generator*/
double awgn(double sn,int *idum)
{
  double x,y;
  double noise=0.0;
  double nvar;

  nvar = 0.5*pow(10.0,(-0.1*sn));
  //nvar = 1.0;
  y = (double)ran1(idum);
  x=0.0;
  while(x == 0.0){
    x = (double)ran1(idum);
  }
  y = (double)ran1(idum);
  noise = sqrt(-2.0*nvar*log(x))*cos(2.0*PI*y);
  return (noise);
}

/*AWGN power generator[mW]*/
double awgn_pow(int *idum){
  double awgn(double sn,int *idum);
  
  double noise_power;
  double I,Q;
  double NOISE_sinti = 0.0;
  I=sqrt(NOISE_sinti);//*awgn(idum);
  Q=sqrt(NOISE_sinti);//*awgn(idum);
  //printf("%e,%e\n",I,Q);
  noise_power = I*I +  Q*Q;
  return noise_power; 
}


