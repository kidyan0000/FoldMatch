import numpy as np
from mayavi.mlab import *
import numpy
import os

import fileinput
import re
import pymesh
import vispy.scene
from vispy.scene import visuals
from plyfile import PlyData, PlyElement, PlyProperty, PlyListProperty
from math import ceil
import sympy
import matplotlib.pyplot as plt
import matplotlib.colors as colors
from matplotlib.mlab import bivariate_normal
from matplotlib.colors import PowerNorm
from mpl_toolkits.mplot3d import Axes3D
from rbf.basis import RBF, get_r, get_eps

def test_quiver3d():
    x, y, z = np.mgrid[-2:3, -2:3, -2:3]
    r = np.sqrt(x ** 2 + y ** 2 + z ** 4)
    u = y * np.sin(r) / (r + 0.001)
    v = -x * np.sin(r) / (r + 0.001)
    w = np.zeros_like(z)
    obj = quiver3d(x, y, z, u, v, w, line_width=3, scale_factor=1)
    return obj

# test_quiver3d()
# show()

currentpath = os.getcwd()
os.chdir(currentpath)

# eigvalData = numpy.zeros((45138,1),dtype=float)
ver_num = 45138
eigvecData = numpy.zeros((ver_num,3),dtype=float)

# read the information from test
for i in range(2, 3):
    
    eigvecName = 'eigvec_' + str(i) + '.txt'
    eigvec = open(eigvecName,'r')
    
    eigveclines = eigvec.readlines()
    eigvecData_row = 0
    for eigvecline in eigveclines:
        eigveclist = eigvecline.strip().split()
        #eigveclist = eigvecline
        eigvecData[eigvecData_row,:] = eigveclist[0:]
        eigvecData_row+=1   
    
    plyFileName = 'wrinkVecField_' + str(i) + '.ply'
    plydata = PlyData.read(plyFileName)

    x = plydata['vertex']['x']
    y = plydata['vertex']['y']
    z = plydata['vertex']['z']
    #vertices = getNormalizePointCloud(np.column_stack((x, y, z)))
    vertices = np.column_stack((x, y, z))
    
    q = plydata['vertex']['quality']
    print(q)
    
    norm = colors.PowerNorm(gamma=0.25)    
    clrs = plt.cm.jet(norm(q))
    #print clrs 
    #scale = np.ceil(q*100000, dtype=float)
    
    #q = matplotlib.colors.SymLogNorm(1,vmin=0.0,vmax=0.6)
    #clrs = plt.cm.jet(norms(q))
    color_255 = np.ceil(clrs * 255)
    #color_tri = np.concatenate((color_255[:,1],color_255[:,2],color_255[:,3]))
    #print(color_tri)
    
    pts = quiver3d(vertices[:,0],vertices[:,1], vertices[:,2], eigvecData[0:ver_num:3,1], eigvecData[1:ver_num:3,1], eigvecData[2:ver_num:3,1],scalars=q, line_width=2)
    pts.glyph.color_mode = 'color_by_scalar'
    # points3d(vertices[:,0],vertices[:,1], vertices[:,2])
    show()
    
    
    
    '''
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
    '''   
        #print(eigvecData[eigvecData_row,0])

    
