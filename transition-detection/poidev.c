#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "param.h"

/* ================================================
   Random Variable Generation
   ran1()

   ln(gamma function)
   gammln()

   Possion Random Variable Generation
   poidev()
   
   Hao Wang 2015.03.12
   from NUMERICAL RECIPES in C
   ================================================ */


#define IA 16807
#define IM 2147483647
#define AM (1.0/IM)
#define IQ 127773
#define IR 2836
#define NTAB 32
#define NDIV (1+(IM-1)/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)

/*ln(gamma function)*/
double gammln(double xx)
{
  double x,y,tmp,ser;
  static double cof [6]={76.18009172947146,-86.50532032941677,
                         24.01409824083091,-1.231739572450155,
                         0.1208650973866179e-2,-0.5395239384953e-5};
  int j;

  y=x=xx;
  tmp=x+5.5;
  tmp -= (x+0.5)*log(tmp);
  ser=1.000000000190015;
  for ( j=0;j<=5;j++) ser += cof[j]/++y;
  return -tmp+log(2.5066282746310005*ser/x);
}


#define PI 3.141592654
/*mean xm poisson random variable*/
double poidev(double ave,int *idum)
{
    double gammln(double xx);
    // float gammln(long *idum);
    double ran1(int *idum);         /* fix me*/

    static double sq, alxm, g, oldm;
    double em,t,y;

    oldm = (-1.0);
    if (ave < 12.0) {
        if(ave != oldm){
            oldm=ave;
            g=exp(-ave);
        }
        em = -1;
        t=1.0;
        do{
            ++em;
            t *= ran1(idum);
        }while (t > g);
    }else{
        if (ave != oldm) {
            oldm=ave;
            sq=sqrt(2.0*ave);
            alxm=log(ave);
            g=ave*alxm-gammln(ave+1.0);
        }
        do{
            do{
                y=tan(PI*ran1(idum));
                em=sq*y+ave;
            } while (em < 0.0);
            em=floor(em);
            t=0.9*(1.0+y*y)*exp(em*alxm-gammln(em+1.0)-g);
        }while (ran1(idum) > t);
    }
    return em;
}
