#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "param.h"

double awgn(int *idum){
    double ran1(int *);
    double x,y,z;
    double nvar;
    nvar=1.0;
    x=0.0;
    while (x==0.0) {
        x=(double)ran1(idum);
    }
    y=(double)ran1(idum);
    
    z=sqrt(-2.0*nvar*log(x))*cos(2.0*M_PI*y);
    return z;
}
