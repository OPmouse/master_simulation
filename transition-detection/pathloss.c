#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "param.h"

double pathloss(double distance){
    double d_0;
    double wavelength;
    double L_0;
    double P;
    /*wavelength(m)*/
    wavelength=3.0*pow(10.0, 8.0)/(double)Frequency;

    /*free distance(m)*/
    if(distance<ref_dis){
        d_0=distance;
    }else{
        d_0=ref_dis;
    }
    /*path-loss(dB)*/
    L_0=(10*log10(pow((4.0*M_PI*d_0/wavelength), 2.0)))+10*expon*log((distance/d_0));

    /*received power(dBm)*/
    P=GT+GR-L_0;
    return (P);
}
