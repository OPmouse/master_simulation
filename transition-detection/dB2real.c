#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

double dB2real(double dBm){
    double real;
    real=pow(10.0, dBm/10.0);
    return real;
}
