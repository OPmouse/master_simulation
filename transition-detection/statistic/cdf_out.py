#!/usr/bin/python
# coding: utf-8

import numpy,time,datetime,sys,math,struct,os,subprocess,csv,re,argparse,GPS_mesh
from signal import signal, SIGPIPE, SIG_DFL

signal(SIGPIPE,SIG_DFL)

class cdf:
    def __init__(self):
        pass
    def create_dict(self,args):
        f = open(args,'r')
        self.dict = {}
        for line in f:
           tmp = line.split(" ")
           x = tmp[0]
           y = tmp[1]
           meshcode = str(x) + str(y)
           #print meshcode
           if self.dict.has_key(meshcode):
               self.dict[meshcode].append(float(tmp[2]))
           else:
               self.dict[meshcode] = [float(tmp[2]),]
    def getdict(self):
        return self.dict

    def search_out_cdf(self,args):
        sumention = 0
        #print self.dict[args]
        histlist = numpy.histogram(self.dict[args],bins=500)
        for i in range(len(histlist[0])):
            prob=histlist[0][i]/float(len(self.dict[args]))
            sumention=prob+sumention
            print str(histlist[1][i])+','+str(prob)+','+str(sumention)
        #print args.start_time,args.duration


if __name__ == "__main__":
    m = cdf() 
    m.create_dict(sys.argv[1])
    mesh_code = GPS_mesh.Mesh()
    mesh_code.GIS_CreateMeshCode(sys.argv[2],sys.argv[3])
    xycode = str(mesh_code.getx()) + str(mesh_code.gety())
    m.search_out_cdf(xycode)
