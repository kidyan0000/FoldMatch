#!/home/kidyan0000/anaconda2/bin/python
import fileinput
import re
import os
import re
import numpy as np
import pymesh
import vispy.scene
from vispy.scene import visuals
from plyfile import PlyData, PlyElement, PlyProperty, PlyListProperty
from math import ceil
import sympy
import matplotlib.pyplot as plt
import matplotlib.colors as colors
from matplotlib.mlab import bivariate_normal
from mpl_toolkits.mplot3d import Axes3D
from rbf.basis import RBF, get_r, get_eps


class MidpointNormalize(colors.Normalize):
    def __init__(self, vmin=None, vmax=None, midpoint=None, clip=False):
        self.midpoint = midpoint
        colors.Normalize.__init__(self, vmin, vmax, clip)

    def __call__(self, value, clip=None):
        # I'm ignoring masked values and all kinds of edge cases to make a
        # simple example...
        x, y = [self.vmin, self.midpoint, self.vmax], [0.0, 0.12, 0.6]
        return np.ma.masked_array(np.interp(value, x, y))


def shiftedColorMap(cmap, start=0, midpoint=0.05, stop=1.0, name='shiftedcmap'):
    '''
    Function to offset the "center" of a colormap. Useful for
    data with a negative min and positive max and you want the
    middle of the colormap's dynamic range to be at zero
    
    Input
    -----
      cmap : The matplotlib colormap to be altered
      start : Offset from lowest point in the colormap's range.
          Defaults to 0.0 (no lower ofset). Should be between
          0.0 and 1.0.
      midpoint : The new center of the colormap. Defaults to 
          0.5 (no shift). Should be between 0.0 and 1.0. In
          general, this should be  1 - vmax/(vmax + abs(vmin))
          For example if your data range from -15.0 to +5.0 and
          you want the center of the colormap at 0.0, `midpoint`
          should be set to  1 - 5/(5 + 15)) or 0.75
      stop : Offset from highets point in the colormap's range.
          Defaults to 1.0 (no upper ofset). Should be between
          0.0 and 1.0.
    '''
    cdict = {
        'red': [],
        'green': [],
        'blue': [],
        'alpha': []
    }
      
    # regular index to compute the colors
    reg_index = np.linspace(start, stop, 257)

    # shifted index to match the data
    shift_index = np.hstack([
        np.linspace(0.0, midpoint, 128, endpoint=False), 
        np.linspace(midpoint, 1.0, 129, endpoint=True)
    ])
    
    for ri, si in zip(reg_index, shift_index):
        r, g, b, a = cmap(ri)

        cdict['red'].append((si, r, r))
        cdict['green'].append((si, g, g))
        cdict['blue'].append((si, b, b))
        cdict['alpha'].append((si, a, a))
        
    newcmap = colors.LinearSegmentedColormap(name, cdict)
    plt.register_cmap(cmap=newcmap)

    return newcmap


def loadCorrespondence(fileCorrespPath):
    with open(fileCorrespPath) as file:
	next(file)
	next(file)
        array2d = [[int(digit) for digit in line.split()] for line in file]
    return np.asarray(array2d)


def getNormalizePointCloud(pts):
    max_ = np.amax(np.absolute(pts), axis = 0)        
    factor = np.amax(max_)   
    pts = np.divide(pts , factor)
    return pts   

def getVolumericFrequencyNormalization(bbMaxR, bbMinR, bbMaxT, bbMinT, ptsT, ptsR):
    # prepare the grid
    if bbMinR[0] != bbMaxR[0]:
    	x = np.linspace(bbMinR[0] , bbMaxR[0], len(ptsR))
    if bbMinR[1] != bbMaxR[1]:
	y = np.linspace(bbMinR[1] , bbMaxR[1], len(ptsR))
    if bbMinR[1] != bbMaxR[1]:
	z = np.linspace(bbMinR[2] , bbMaxR[2], len(ptsR))

# read the ply file The vertices will
currentpath = os.getcwd()
os.chdir(currentpath)
 
for i in range(1, 72):
    plyFileName = 'lambda1_' + str(i) + '.ply'
    plydata = PlyData.read(plyFileName)

    x = plydata['vertex']['x']
    y = plydata['vertex']['y']
    z = plydata['vertex']['z']
    vertices = getNormalizePointCloud(np.column_stack((x, y, z)))

    q = plydata['vertex']['quality']

    fig = plt.figure( 1, figsize = (10,10))
    ax = fig.add_subplot(111, projection='3d')

    orig_Jetcmap = plt.cm.jet
    shiftedJet_cmap = shiftedColorMap(orig_Jetcmap, start=0.0, midpoint=0.05, stop=1.0, name='shifted')

    clrs = shiftedJet_cmap(q)
    clrs_jet =  plt.cm.jet(q)

    print clrs 
    #q = matplotlib.colors.SymLogNorm(1,vmin=0.0,vmax=0.6)
    #clrs = plt.cm.jet(norms(q))
    color_255 = np.ceil(clrs * 255)
    
    with open('lambdaQuality1_' + str(i) + '.ply', 'w') as fp:
	    fp.write('ply\n')
	    fp.write('format ascii 1.0\n')
	    fp.write('comment VCGLIB generated\n')
	    fp.write('element vertex')
	    fp.write(' ' + str(len(vertices)) + '\n')
	    fp.write('property float x\n')
	    fp.write('property float y\n')
	    fp.write('property float z\n')
	    fp.write('property uchar red\n')
	    fp.write('property uchar green\n')
	    fp.write('property uchar blue\n')
            fp.write('property float quality\n')
	    fp.write('end_header\n')
	    for v in range(0,len(vertices)):
		fp.write('%f %f %f %d %d %d %f\n' %(vertices[v][0], vertices[v][1], vertices[v][2], color_255[v][0], color_255[v][1], color_255[v][2], q[v]))  

    #surf = ax.scatter(vertices[:,0],vertices[:,1], vertices[:,2], c=colors)
    #sm = plt.cm.ScalarMappable(cmap=plt.cm.plasma, norm=norm)
    #sm.set_array(q)
    #fig.colorbar(sm)
    #ax.set_xlabel("x")
    #ax.set_ylabel("y")
    #ax.set_zlabel("z")
    #ax.view_init(elev=0., azim=180.0)

    #p = ax.tripcolor(pointsT[:,0], pointsT[:,1], valuesDummyT, shading = 'gouraud', cmap= 'viridis')
    #plt.scatter(vertices[:,0],vertices[:,1], vertices[:,2], s=25, cmap='plasma', marker = 'o') #, edgecolor='k'
    #plt.colorbar(p,ax=ax)
    #plt.grid(True)
    #plt.xticks(fontsize = 25) # work on current fig
    #plt.yticks(fontsize = 25) # work on current fig
    #plt.zticks(fontsize = 25) # work on current fig
    #plt.tight_layout()
    #plt.savefig('/home/ali/lambda1/lambda1_' + str(i) + '.png')
    #plt.show()


