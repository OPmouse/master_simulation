#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

//paramter for ran1()
#define IA 16807
#define IM 2147483647
#define AM (1.0/IM)
#define IQ 127773
#define IR 2836
#define NTAB 32
#define NDIV (1+(IM-1)/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)

//paramter for sensing
#define FFTPOINT 2048
//parameter for PU
#define transition_point1 512
#define transition_point2 1536

//#define P 10
#define sigma_2 1.0

//common paramter
#define SNR_STEP 1
#define MIN_SNR 10
#define iteration 1
#define T0 10

double ran1(int *idum)
{
    int j;
    int k;
    static int iy=0;
    static int iv[NTAB];
    double temp;
    if(*idum <= 0 || !iy) {
        if(-(*idum) < 1)
            *idum=1;
        else
            *idum = -(*idum);
        for(j=NTAB+7;j>=0;j--){
            k = (*idum) / IQ;
            *idum = IA * (*idum-k * IQ) - IR * k;
            if(*idum < 0)
                *idum += IM;
            if(j < NTAB)
                iv[j] = *idum;
        }
        iy=iv[0];
    }
    k = (*idum) / IQ;
    *idum = IA * (*idum - k * IQ) - IR * k;
    if(*idum < 0)
        *idum += IM;
    j = iy / NDIV;
    iy = iv[j];
    iv[j] = *idum;
    if ((temp = AM * iy) > RNMX)
        return RNMX;
    else
        return temp;
}
double _randG(double var,int *idum)
{
    double x,y;
    double noise=0.0;
    //nvar = 1.0;
    y = (double)ran1(idum);
    x=0.0;
    while(x == 0.0){
        x = (double)ran1(idum);
    }
    y = (double)ran1(idum);
    noise = sqrt(-2.0*var*log(x))*cos(2.0*M_PI*y);
    return (noise);
}

void _DataGeneration(double *data,double var,int start,int end,int *idum){
    unsigned int i;
    for (i=start; i<end; i++) {
        data[i] = _randG(var, idum);
    }
}

//loglikihood function for ln(f1(t)/f0(t))
void _loglikelihood_OFF2ON(double *l,double *data,double P){
    unsigned int i;
    for (i=0; i<FFTPOINT; i++) {
        l[i] = P*data[i]*data[i]/(2*(P+sigma_2)*sigma_2)+0.5*log(sigma_2/(P+sigma_2));
        
    }
}


double _MAX(double x,double y){
    if(x>y){
        return x;
    }else{
        return y;
    }
}

//Generatalized Likelihood Ratio algorithm
//g_t calculation for OFF->ON detection
double _GLR_OFF2ON(double y_heart,double P_op,int t,int k){
    return P_op*y_heart/(2.0*(P_op+sigma_2)*sigma_2) + 0.5*(double)(t-k)*log(sigma_2/(P_op+sigma_2));
}
//g_t calculation for ON->OFF detection
double _GLR_ON2OFF(double y_heart,double P_op,int t,int k){
    return (-1.0)*P_op*y_heart/(2.0*(P_op+sigma_2)*sigma_2) + 0.5*(double)(t-k)*log((P_op+sigma_2)/sigma_2);
}

void _decision(int *result,double *g_t,double threshold) {
    unsigned i;
    for (i=0; i<FFTPOINT-1; i++) {
        if(g_t[i]>threshold) {
            result[i] = 1;
        }
        else {
            result[i] = 0;
        }
    }
}


double _sum(double *data,int start,int end){
    unsigned int i;
    double sum=0.0;
    for (i=start; i<end; i++) {
        sum += data[i];
    }
    return sum;
}
double _sum_2(double *data,int start,int end){
    unsigned int i;
    double sum_2 = 0.0;
    for (i=start; i<end; i++) {
        sum_2 += data[i]*data[i];
    }
    return sum_2;
}

double max(double *data,int len){
    unsigned int i;
    double max=data[0];
    for(i=1;i<len;i++){
        if(max<data[i]){
            max=data[i];
        }
    }
    return max;
}

int transition_point_detection_OFF2ON(int *sample,int len){
    unsigned int i;
    int candidate = 0;
    //int len1,len2;
    for (i=0; i<len-1; i++) {
        if(sample[i]== 0 && sample[i+1]==1) {
            candidate = (int)i;
        }
    }
    return candidate;
}

int transition_point_detection_ON2OFF(int *sample,int len,int transition_point_OFF2ON){
    unsigned int i;
    int candidate;
    for (i=0; i<len-1; i++) {
        if(sample[i]==0 && sample[i+1]==1){
            if(i>transition_point_OFF2ON && i-transition_point_OFF2ON>0){
                candidate = (int) i;
            }
        }
    }
    return candidate;
}

//Kullback-Leibler divergance calculation D(f1||f0)
double _D(double P){
    return P/(2.0*(P+sigma_2))+0.5*log(sigma_2/(P+sigma_2));
}


int main() {
    unsigned int i,iter,sn,_t,k;
    double y[FFTPOINT];//observed sample
    double g_glr_OFF2ON[FFTPOINT],g_glr_ON2OFF[FFTPOINT];
    int Decision_glr_OFF2ON[FFTPOINT];
    int Decision_glr_ON2OFF[FFTPOINT];
    
    double SNRdB[SNR_STEP];
    double P[SNR_STEP];
    double P_max[SNR_STEP],P_min[SNR_STEP];
    double P_heart_OFF2ON[SNR_STEP],P_heart_ON2OFF[SNR_STEP];
    int Transition_point_OFF2ON_glr[SNR_STEP];
    int Transition_point_ON2OFF_glr[SNR_STEP];
    
    double tmp_upper,tmp_lower;
    double tmp_Pmin,tmp_Pmax;
    
    double F_P_OFF2ON[FFTPOINT],F_P_ON2OFF[FFTPOINT];
    
    double Power = 0.0;
    double Power_transition_glr=0.0;
    double Power_2 = 0.0;
    double Power_transition_2_glr =0.0;
    double Var;
    double Var_transition_glr;
    double diff = 0.0;
    double diff_transition_glr=0.0;
    double ave_diff;
    double ave_diff_transition_glr;
    
    double threshold_glr[sn];
    
    double a = 1.0/(double)(T0);
    double b[sn];
    
    /*ran1 initialization*/
    int p,n;
    double r;
    time_t t;
    
    
    n = -((unsigned int)time(&t));
    p = n;
    r = ran1(&p);
    
    srand((unsigned)time(NULL));
    /*ran1 initialization*/
    
    /*buffering output*/
    setvbuf(stdout, 0, _IONBF, 0);
    /*buffering output*/
    
    
    for (sn=0; sn<SNR_STEP;sn++) {
        SNRdB[sn] = (double)sn + (double)MIN_SNR;
        P[sn] = pow(10.0, SNRdB[sn]/10.0)*sigma_2;
        
        P_max[sn] = 2.0 * P[sn];
        P_min[sn] = 0.5 * P[sn];
        
        //threshold for grt algorithm
        b[sn]=3*log(pow(1+1.0/_D(P_min[sn]), 2.0)/a);
        threshold_glr[sn] = -log(-a/b[sn]);
        //printf("D=%lf\n",log(-a/(3*log(pow(1+1.0/_D(P_min[sn]),2.0)/a))));
        //printf("a=%lf,b=%lf\n",a,b[sn]);
        //printf("threshold---GLR:%lf\n",threshold_glr[sn]);
        
        for (iter=0;iter<iteration;iter++){
            //observed sample generation OFF:[0,transition point1] ON:[transition point1 FFTPOINT]
            
            //N(0,sigma_2) range:[0,transition_point1]
            _DataGeneration(y, sigma_2, 0, transition_point1, &p);
            
            //N(0,P+sigma_2) range:[transition_point1,transition_point2]
            _DataGeneration(y, sigma_2+P[sn], transition_point1, transition_point2, &p);
            
            //N(0,sigma_2) range:[transition_point2,FFTPOINT]
            _DataGeneration(y, sigma_2, transition_point2, FFTPOINT, &p);
            
            
            //GLT algorithm
            //find a optimal P -> P* for detecting OFF->ON
            for (_t=1; _t<FFTPOINT; _t++) {
                for (k=0; k<_t; k++) {
                    tmp_lower = (double)_sum_2(y, (int)k+1, (int)_t+1)/(double)(P_max[sn] + sigma_2);
                    tmp_upper = (double)_sum_2(y, (int)k+1, (int)_t+1)/(double)(P_min[sn] + sigma_2);
                    if ((double)(_t-k)<=tmp_lower) {
                        P_heart_OFF2ON[sn] = P_max[sn];
                        P_heart_ON2OFF[sn] = P_min[sn];
                    }
                    else if((double)(_t-k)>=tmp_lower && (double)(_t-k)<=tmp_upper){
                        P_heart_OFF2ON[sn] = (double)_sum_2(y, (int)k+1, (int)_t+1)/(double)(_t-k) - sigma_2;
                        
                        tmp_Pmax = _GLR_ON2OFF((double)_sum_2(y, (int)k+1, (int)_t+1), P_max[sn], (int)_t, (int)k);
                        tmp_Pmin = _GLR_ON2OFF((double)_sum_2(y, (int)k+1, (int)_t+1), P_min[sn], (int)_t, (int)k);
                        if(tmp_Pmax > tmp_Pmin){
                            P_heart_ON2OFF[sn] = P_max[sn];
                        }
                        else {
                            P_heart_ON2OFF[sn] = P_min[sn];
                        }
                    }
                    else if((double)(_t-k)>=tmp_upper){
                        P_heart_OFF2ON[sn] = P_min[sn];
                        P_heart_ON2OFF[sn] = P_max[sn];
                    }
                    F_P_OFF2ON[k] = _GLR_OFF2ON((double)_sum_2(y, (int)k+1, (int)_t+1), P_heart_OFF2ON[sn], (int)_t, (int)k);
                    F_P_ON2OFF[k] = _GLR_ON2OFF((double)_sum_2(y, (int)k+1, (int)_t+1), P_heart_ON2OFF[sn], (int)_t, (int)k);
                }
                g_glr_OFF2ON[_t] = max(F_P_OFF2ON, _t);
                g_glr_ON2OFF[_t] = max(F_P_ON2OFF, _t);
            }
            
            //for debug
            for (i=0; i<FFTPOINT; i++) {
                printf("%d,%lf\n",i,g_glr_ON2OFF[i]);
            }
            
            //decision for detecting OFF->ON
            _decision(Decision_glr_OFF2ON, g_glr_OFF2ON,threshold_glr[sn]);
            //decision for detecting ON->OFF
            _decision(Decision_glr_ON2OFF, g_glr_ON2OFF, threshold_glr[sn]);
            
            //transition point detection OFF->ON
            Transition_point_OFF2ON_glr[sn] = transition_point_detection_OFF2ON(Decision_glr_OFF2ON, FFTPOINT);
            //transition point detection ON->OFF
            Transition_point_ON2OFF_glr[sn] = transition_point_detection_ON2OFF(Decision_glr_ON2OFF, FFTPOINT, Transition_point_OFF2ON_glr[sn]);
            //printf("CUSUM: OFF2ON:%d  ON2OFF:%d\n",Transition_point_OFF2ON_cusum[sn],Transition_point_ON2OFF_cusum[sn]);
            
            //fprintf(stdout,"%lf,%d,%d\n",SNRdB[sn],Transition_point_OFF2ON_glr[sn],Transition_point_ON2OFF_glr[sn]);
            for (i=0; i<FFTPOINT; i++) {
                Power += y[i];
                Power_2 += y[i]*y[i];
                if (i>Transition_point_OFF2ON_glr[sn] && i<Transition_point_ON2OFF_glr[sn]) {
                    Power_transition_glr += y[i];
                    Power_transition_2_glr += y[i]*y[i];
                }
            }
            //Var =E(X^2)-E(X)^2
            Power /= (double)FFTPOINT;
            Power_2 /= (double)FFTPOINT;
            
            //GLR
            Power_transition_glr /=((double)FFTPOINT-(double)Transition_point_OFF2ON_glr[sn]);
            Power_transition_2_glr /=((double)FFTPOINT-(double)Transition_point_OFF2ON_glr[sn]);
            
            Var = Power_2 - Power*Power;
            
            //GLR
            Var_transition_glr = Power_transition_2_glr - Power_transition_glr*Power_transition_glr;
            
            //printf("sn=%d,Power=%lf,Power_transition=%lf\n",sn,Var,Var_transition);
            diff += P[sn]/Var;
            //GLR
            diff_transition_glr += P[sn]/Var_transition_glr;
            
            Power = 0.0;Power_2 = 0.0;
            Power_transition_glr = 0.0;Power_transition_2_glr = 0.0;
        }
        ave_diff = 10*log10(diff/(double)iteration);
        //GLR
        ave_diff_transition_glr =10*log10(diff_transition_glr/(double)iteration);
        //fprintf(stdout,"%lf,%lf,%lf\n",SNRdB[sn],ave_diff,ave_diff_transition_glr);
        
        
        diff=0.0;
        diff_transition_glr=0.0;
    }
    return 0;
}
