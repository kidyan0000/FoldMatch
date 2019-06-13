import numpy
import os
import numpy as np

currentpath = os.getcwd()
os.chdir(currentpath)

eigvalData = numpy.zeros((45138,1),dtype=float)
eigvecData = numpy.zeros((45138,3),dtype=float)

# read the information from test
for i in range(2, 10):
    eigvalName = 'eigval_' + str(i) + '.txt'
    eigval = open(eigvalName,'r')

    eigvallines = eigval.readlines()
    eigvalData_row = 0
    for eigvalline in eigvallines:
        # list = line.strip().split('        ')
        eigvallist = eigvalline
        eigvalData[eigvalData_row:] = eigvallist[0:]
        eigvalData_row+=1
        #print(eigvalData[eigvalData_row])

    eigvecName = 'eigvec_' + str(i) + '.txt'
    eigvec = open(eigvecName,'r')

    eigveclines = eigvec.readlines()
    eigvecData_row = 0
    for eigvecline in eigveclines:
        eigveclist = eigvecline.strip().split()
        #eigveclist = eigvecline
        eigvecData[eigvecData_row:] = eigveclist[0:]
        eigvecData_row+=1
        #print(eigvecData[eigvecData_row,0])
        
    
