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
#define transition_point1 500

//#define P 10
#define sigma_2 1.0

//common paramter
#define SNR_STEP 30
#define RANGE 10.0
#define MIN_SNR  20
#define iteration 10000
#define T0 10
#define MODE 1
#define MODE_NORM 1
#define MODE_EXP 0

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

//normal distribution
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

//exponential distribution
double _randExp(double rambda,int *idum){
  return -log(ran1(idum))/rambda;
}

//data generation
void _DataGeneration(double *data,double var,int start,int end,int *idum,int mode){
  unsigned int i;
  if(mode==MODE_EXP){
    for (i=start; i<end; i++) {
      data[i] = _randExp(var, idum);
    }
  }
  if(mode==MODE_NORM){
    for (i=start; i<end; i++) {
      data[i] = _randG(var, idum);
    }
  }
  
}

//loglikihood function for ln(f1(t)/f0(t))
void _loglikelihood_OFF2ON(double *l,double *data,double P,int mode){
  unsigned int i;
  for (i=0; i<FFTPOINT; i++) {
    //case 1 : normal distribution
    if(mode==MODE_NORM){
      l[i] = P*data[i]*data[i]/(2*(P+sigma_2)*sigma_2)+0.5*log(sigma_2/(P+sigma_2));
    }
    //case 2 : exponential distribution
    if(mode==MODE_EXP){
      l[i] = 0.5*log(2*M_PI*sigma_2*sigma_2*P*P)-P*data[i]+data[i]*data[i]/(2*sigma_2*sigma_2);
    }
  }
}

//loglikihood function for ln(f0(t)/f1(t))
void _loglikelihood_ON2OFF(double *l,double *data,double P,int mode){
  unsigned int i;
  //case 1 : normal distribution
  if(mode==MODE_NORM){
    for (i=0; i<FFTPOINT; i++) {
      l[i] = -P*data[i]*data[i]/(2*(P+sigma_2)*sigma_2)+0.5*log((P+sigma_2)/sigma_2);
    }
  }
  //case 2 : exponential distribution
  if(mode==MODE_EXP){
    for (i=0; i<FFTPOINT; i++) {
      l[i] = -0.5*log(2*M_PI*sigma_2*P*P)+P*data[i]-data[i]*data[i]/(2*sigma_2);
    }
  }
}

double _MAX(double x,double y){
  if(x>y){
    return x;
  }else{
    return y;
  }
}
//CUSUM algorithm
void _CUSUM(double *g,double *l){
  unsigned int i;
  g[0] = 0.0;
  for (i=1; i<FFTPOINT; i++) {
    g[i]=_MAX(g[i-1]+l[i], 0.0);
    //printf("g[%d]=%lf\n",i,g[i]);
  }
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

double SecondMax(double *num, int n)
{
  int i;
  double second,max;
  second=max=num[0];
  for (i=1; i<n; i++) {
    max=num[i];
  }
  for (i=0; i<n; i++) {
    if (num[i]>max) {
      second=max;
      max=num[i];
    }else if ((max>num[i])&&(num[i]>second)){
      second=num[i];
    }
  }
  return second;
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

int find_id(double *data,double max,int len){
  unsigned int i;
  int transition_point ;
  for (i=0; i<len; i++) {
    if(data[i]==max){
      transition_point = i;
    }
  }
  return transition_point;
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
    int candidate = 0;
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


int main(int argc,char *argv[]) {
  int transition_point2;
  transition_point2 = atoi(argv[1]);

  unsigned int i,iter,sn;
  double y[FFTPOINT];//observed sample
  double l_OFF2ON[FFTPOINT],l_ON2OFF[FFTPOINT];//log likelyhood ratio
  double g_cusum_OFF2ON[FFTPOINT],g_cusum_ON2OFF[FFTPOINT];//decision value for quickest detection using CUSUM algorithm
  //double g_glr_OFF2ON[FFTPOINT],g_glr_ON2OFF[FFTPOINT];
  int Decision_cusum_OFF2ON[FFTPOINT];//,Decision_glr_OFF2ON[FFTPOINT];
  int Decision_cusum_ON2OFF[FFTPOINT];//,Decision_glr_ON2OFF[FFTPOINT];
  
  double SNRdB[SNR_STEP];
  double P[SNR_STEP];
  //double P_max[SNR_STEP],P_min[SNR_STEP];
  //double P_heart[SNR_STEP];
  int Transition_point_OFF2ON_cusum[SNR_STEP];//,Transition_point_OFF2ON_glr[SNR_STEP];
  int Transition_point_ON2OFF_cusum[SNR_STEP];//,Transition_point_ON2OFF_glr[SNR_STEP];
  
  
  //double F_P_OFF2ON[FFTPOINT],F_P_ON2OFF[FFTPOINT];
  
  double Power = 0.0;
  double Power_transition_cusum = 0.0;//,Power_transition_glr=0.0;
  double Power_2 = 0.0;
  double Power_transition_2_cusum =0.0;//,Power_transition_2_glr =0.0;
  double Var;
  double Var_transition_cusum;//,Var_transition_glr;
  double diff = 0.0;
  double diff_transition_cusum=0.0;//,diff_transition_glr=0.0;
  double ave_diff;
  double ave_diff_transition_cusum;//,ave_diff_transition_glr;
  //g[0]=0.0;
  
  double threshold_cusum = log((double)T0);
  //double threshold_glr[sn];
  
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
    SNRdB[sn] = (double)sn-RANGE;
    P[sn] = pow(10.0, SNRdB[sn]/10.0)*sigma_2;
    
    for (iter=0;iter<iteration;iter++){
      //observed sample generation OFF:[0,transition point1] ON:[transition point1 FFTPOINT]
      
      //N(0,sigma_2) range:[0,transition_point1]
      _DataGeneration(y, sigma_2, 0, transition_point1, &p,MODE_NORM);
      
      //N(0,P+sigma_2) range:[transition_point1,transition_point2]
      if(MODE==MODE_NORM){
	_DataGeneration(y, sigma_2+P[sn], transition_point1, transition_point2, &p,MODE_NORM);
      }
      if(MODE==MODE_EXP){
	_DataGeneration(y, P[sn], transition_point1, transition_point2, &p,MODE_EXP);
      }
      //N(0,sigma_2) range:[transition_point2,FFTPOINT]
      _DataGeneration(y, sigma_2, transition_point2, FFTPOINT, &p,MODE_NORM);
      
      //log likelihood calculation
      _loglikelihood_OFF2ON(l_OFF2ON, y, P[sn],MODE);
      _loglikelihood_ON2OFF(l_ON2OFF, y, P[sn],MODE);
      
      //decision value calculation for quickest detection
      _CUSUM(g_cusum_OFF2ON, l_OFF2ON);
      _CUSUM(g_cusum_ON2OFF, l_ON2OFF);
      
      for (i=0; i<FFTPOINT; i++) {
	//printf("g_ON2OFF\t%d\t%lf\tg_OFF2ON\t%d\t%lf\n",i,g_cusum_ON2OFF[i],i,g_cusum_OFF2ON[i]);
      }
      
      //decision for detecting OFF->ON
      //_decision(Decision_cusum_OFF2ON, g_cusum_OFF2ON,threshold_cusum);
      
      //decision for detecting ON->OFF
      //_decision(Decision_cusum_ON2OFF, g_cusum_ON2OFF, threshold_cusum);
      
      //transition point detection ON->OFF
      //Transition_point_ON2OFF_cusum[sn] = transition_point_detection_ON2OFF(Decision_cusum_ON2OFF, FFTPOINT, Transition_point_OFF2ON_cusum[sn]);
      Transition_point_ON2OFF_cusum[sn] = find_id(g_cusum_OFF2ON, max(g_cusum_OFF2ON, FFTPOINT), FFTPOINT);
      
      //transition point detection OFF->ON
      //Transition_point_OFF2ON_cusum[sn] = transition_point_detection_OFF2ON(Decision_cusum_OFF2ON, FFTPOINT);
      //fprintf(stdout,"max id: %d, second max id:%d\n",find_id(g_cusum_ON2OFF, max(g_cusum_ON2OFF, FFTPOINT), FFTPOINT),find_id(g_cusum_ON2OFF, SecondMax(g_cusum_ON2OFF, FFTPOINT), FFTPOINT));
      if (find_id(g_cusum_ON2OFF, SecondMax(g_cusum_ON2OFF, FFTPOINT), FFTPOINT)>Transition_point_ON2OFF_cusum[sn] || find_id(g_cusum_ON2OFF, max(g_cusum_ON2OFF, FFTPOINT), FFTPOINT)>Transition_point_ON2OFF_cusum[sn]) {
	Transition_point_OFF2ON_cusum[sn] = find_id(g_cusum_ON2OFF, max(g_cusum_ON2OFF, Transition_point_ON2OFF_cusum[sn]), Transition_point_ON2OFF_cusum[sn]);
      }
      //printf("CUSUM: OFF2ON:%d  ON2OFF:%d\n",Transition_point_OFF2ON_cusum[sn],Transition_point_ON2OFF_cusum[sn]);
      if (Transition_point_OFF2ON_cusum[sn]<Transition_point_ON2OFF_cusum[sn]) {
	//fprintf(stdout,"%lf,%d,%d\n",SNRdB[sn],Transition_point_OFF2ON_cusum[sn],Transition_point_ON2OFF_cusum[sn]);
	for (i=0; i<FFTPOINT; i++) {
	  Power += y[i];
	  Power_2 += y[i]*y[i];
	  if (i>Transition_point_OFF2ON_cusum[sn] && i < Transition_point_ON2OFF_cusum[sn]) {
	  Power_transition_cusum += y[i];
	  Power_transition_2_cusum += y[i]*y[i];
	  }
	}
      }
      //Var =E(X^2)-E(X)^2
      Power /= (double)FFTPOINT;
      Power_2 /= (double)FFTPOINT;
      //printf("Power=%lf,power_2=%lf\n",Power,Power_2);
      //CUSUM
      Power_transition_cusum /=((double)FFTPOINT-(double)Transition_point_OFF2ON_cusum[sn]);
      Power_transition_2_cusum /=((double)FFTPOINT-(double)Transition_point_OFF2ON_cusum[sn]);
        
      Var = Power_2 - Power*Power;
      
      //CUSUM
      Var_transition_cusum = Power_transition_2_cusum - Power_transition_cusum*Power_transition_cusum;
      
      //printf("sn=%d,Power=%lf,Power_transition=%lf\n",sn,Var,Var_transition);
      if(Var!=0.0) {
	diff += (double)P[sn]/(double)Var;
	diff_transition_cusum += (double)P[sn]/(double)Var_transition_cusum;
      }
      //printf("%lf,%lf,%lf,diff=%lf\n",P[sn],Var,(double)P[sn]/(double)Var,(double)diff);
      //CUSUM
      //GLR
      //diff_transition_glr += P[sn]/Var_transition_glr;
      
      Power = 0.0;Power_2 = 0.0;
      Power_transition_cusum = 0.0;Power_transition_2_cusum = 0.0;
    }
    ave_diff = 10*log10(diff/(double)iteration);
    //CUSUM
    ave_diff_transition_cusum =10*log10(diff_transition_cusum/(double)iteration);
    fprintf(stdout,"%lf\t%lf\t%lf\n",SNRdB[sn],ave_diff,ave_diff_transition_cusum);
    
    diff=0.0;
    diff_transition_cusum=0.0;
  }
  return 0;
}
