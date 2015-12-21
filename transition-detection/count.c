#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "param.h"


/*Function of Counting the number of ON*/
int count_ON(int *data,int sample){
  int i;
  int tmp=0;
  
  for(i=0;i<sample;i++){
    if(data[i]==1) tmp++;
  }
  return tmp;
}

/*Function of Counting the number of OFF->OFF*/
int count_OFF2OFF(int *data,int sample){
  int i;
  int tmp = 0;
  
  for(i=0;i<sample-1;i++){
    if((data[i]==0) && (data[i+1]==0)) tmp++;
  }
  return tmp;
}

/*Function of Counting the number of ON->ON*/
int count_ON2ON(int *data,int sample){
  int i;
  int tmp = 0;
  
  for(i=0;i<sample-1;i++){
    if((data[i]==1) && (data[i+1]==1)) tmp++;
  }
  return tmp;
}

/*Function of Counting the number of ON->OFF*/
int count_ON2OFF(int *data,int sample){
  int i;
  int tmp = 0;
  
  for(i=0;i<sample-1;i++){
    if((data[i]==1) && (data[i+1]==0)) tmp++;
  }
  return tmp;
}

/*Function of Counting the number of OFF->ON*/
int count_OFF2ON(int *data,int sample){
  int i;
  int tmp = 0;
  
  for(i=0;i<SAMPLE-1;i++){
    if((data[i]==0) && (data[i+1]==1)) tmp++;
  }
  return tmp;
}



/*Function of Counting the number of ON on PU*/
int count_ON_PU(int *data,int size){
  int i;
  int tmp=0;
  
  for(i=0;i<size;i++){
    if(data[i]!=0) tmp++;
  }
  return tmp;
}

/*Function of Counting the number of ON->OFF of PU*/
int count_ON2OFF_PU(int *data,int size){
  int i;
  int tmp = 0;
  
  for(i=0;i<size-1;i++){
    if((data[i]==1) && (data[i+1]==0)) tmp++;
  }
  return tmp;
}

/*Function of Counting the number of OFF->ON of PU*/
int count_OFF2ON_PU(int *data,int size){
  int i;
  int tmp = 0;
  
  for(i=0;i<size-1;i++){
    if((data[i]==0) && (data[i+1]==1)) tmp++;
  }
  return tmp;
}


/*Function of Counting the number of ON->ON of PU*/
int count_ON2ON_PU(int *data,int size){
  int i;
  int tmp = 0;
  
  for(i=0;i<size-1;i++){
    if((data[i]==1) && (data[i+1]==1)) tmp++;
  }
  return tmp;
}


/*Function of Counting the number of OFF->OFF of PU*/
int count_OFF2OFF_PU(int *data,int size){
  int i;
  int tmp = 0;
  
  for(i=0;i<size;i++){
    if((data[i]==0) && (data[i+1]==0)) tmp++;
  }
  return tmp;
}
