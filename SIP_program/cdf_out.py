#!/usr/bin/python
# coding: utf-8

import numpy,time,datetime,sys,math,struct,os,subprocess,csv,re,argparse
from signal import signal, SIGPIPE, SIG_DFL

signal(SIGPIPE,SIG_DFL)

class cdf:
    def __init__(self):
        pass
    def run(self,args):
        f = open(args.target,'r')
        self.reader = csv.reader(filter(lambda row: row[0]!='#', f))
        inputtime = a.Tconv(args.start_time)
        if args.m:
            endtime = inputtime + a.Tsec(60*args.duration) 
        else :
            endtime = inputtime + a.Tsec(args.duration) 
        #print inputtime,endtime
        lists = [line for line in self.reader if len(line)==10 and inputtime < a.Tconv(line[0]) < endtime]
        #print len(lists)
        powerlist=[]
        for line in lists:
            powerlist.append(10*math.log10(float(line[8])))
            #print a.Get_jst(line[0])
        #print powerlist
        sumention = 0
        histlist = numpy.histogram(powerlist,bins=200)
        for i in range(len(histlist[0])):
            prob=histlist[0][i]/float(len(lists))
            sumention=prob+sumention
            print str(histlist[1][i])+','+str(prob)+','+str(sumention)
        #print args.start_time,args.duration

if __name__ == "__main__":
    m=
    m.run(args)
   
