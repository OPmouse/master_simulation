#!/usr/bin/python
# coding: utf-8

import numpy,time,datetime,sys,math,struct,os,subprocess,csv,re,argparse
from signal import signal, SIGPIPE, SIG_DFL

signal(SIGPIPE,SIG_DFL)

class cdf:
    def __init__(self):
        pass
    
    def search_out_cdf(self,args):
        f=open(args,'r')
        powerlist=[]
        for line in f:
            powerlist.append(float(line))
        sumention = 0
        #print self.dict[args]
        histlist = numpy.histogram(powerlist,bins=500)
        for i in range(len(histlist[0])):
            prob=histlist[0][i]/float(len(powerlist))
            sumention=prob+sumention
            #print str(histlist[1][i])+','+str(prob)
            print str(histlist[1][i])+','+str(prob)+','+str(sumention)


if __name__ == "__main__":
    m = cdf()
    m.search_out_cdf(sys.argv[1])
