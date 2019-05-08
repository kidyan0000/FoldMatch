#!/home/kidyan0000/anaconda2/bin/python
import fileinput
import re
import os
import re
import numpy as np
import pymesh
#import vispy.scene
#from vispy.scene import visuals
from plyfile import PlyData, PlyElement, PlyProperty, PlyListProperty
from math import ceil
import sympy
import matplotlib.pyplot as plt
import matplotlib.colors as colors
from matplotlib.mlab import bivariate_normal
from matplotlib.colors import PowerNorm
from mpl_toolkits.mplot3d import Axes3D
from rbf.basis import RBF, get_r, get_eps
from vispy import app, scene, io, visuals
from vispy.color import get_colormaps, BaseColormap
from vispy.visuals.transforms import STTransform


class MidpointNormalize(colors.Normalize):
    def __init__(self, vmin=None, vmax=None, midpoint=None, clip=False):
        self.midpoint = midpoint
        colors.Normalize.__init__(self, vmin, vmax, clip)

    def __call__(self, value, clip=None):
        # I'm ignoring masked values and all kinds of edge cases to make a
        # simple example...
        x, y = [self.vmin, self.midpoint, self.vmax], [0.0, 0.12, 0.6]
        return np.ma.masked_array(np.interp(value, x, y))


def shiftedColorMap(cmap, start=0, midpoint=0.2, stop=1.0, name='shiftedcmap'):
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
 
for i in range(2, 75):
    plyFileName = 'wrinkVecField_' + str(i) + '.ply'
    plydata = PlyData.read(plyFileName)

    x = plydata['vertex']['x']
    y = plydata['vertex']['y']
    z = plydata['vertex']['z']
    #vertices = getNormalizePointCloud(np.column_stack((x, y, z)))
    vertices = np.column_stack((x, y, z))

    q = plydata['vertex']['quality']

    fig = plt.figure( 1, figsize = (10,10))
    ax = fig.add_subplot(111, projection='3d')
    
    #print ax.azim	
    #ax.view_init(-30, 90)
    
    #set the axis
    #plt.xlim(-1, 1)
    #plt.ylim(-1, 1)
    #plt.zlim(-1, 1)

    #orig_Jetcmap = plt.cm.jet
    #shiftedJet_cmap = shiftedColorMap(orig_Jetcmap, start=0.0, midpoint=0.2, stop=1.0, name='shifted')
    #clrs = shiftedJet_cmap(q)
    #clrs_jet = plt.cm.jet(q)
    
    norm = colors.PowerNorm(gamma=1./0.25) 
    rcmap = plt.cm.jet.reversed()   
    clrs = rcmap(norm(q))
    #clrs = plt.cm.jet(norm(q))
    print clrs 
    
    #q = matplotlib.colors.SymLogNorm(1,vmin=0.0,vmax=0.6)
    #clrs = plt.cm.jet(norms(q))
    color_255 = np.ceil(clrs * 255)
    
    with open('wrinkVecFieldQuality_' + str(i) + '.ply', 'w') as fp:
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
    

    # Prepare canvas
    canvas = scene.SceneCanvas(keys='interactive', size=(800, 600), show=True)
    canvas.measure_fps()
 
    # Set up a viewbox to display the image with interactive pan/zoom
    view = canvas.central_widget.add_view()
 
    # Set whether we are emulating a 3D texture
    emulate_texture = False
    
    # build your visuals
    Scatter3D = scene.visuals.create_visual_node(visuals.MarkersVisual)

    # Create the volume visuals, only one is visible
    # mesh1 = scene.visuals.Mesh(vertices = vertices,faces = None, color = (1,0,1), parent=view.scene)
    p1 = Scatter3D(parent=view.scene)
    p1.set_gl_state('translucent', blend=True, depth_test=True)
    p1.set_data(vertices, face_color=clrs,symbol='o', size=8.0)
    #p1.symbol = visuals.marker_types[0]
    
    # Create three cameras (Fly, Turntable and Arcball)
    fov = 60.
    cam1 = scene.cameras.FlyCamera(parent=view.scene, fov=fov, name='Fly')
    cam2 = scene.cameras.TurntableCamera(parent=view.scene, fov=fov,
                                     name='Turntable')
    cam3 = scene.cameras.ArcballCamera(parent=view.scene, fov=fov, name='Arcball')
    view.camera = cam2  # Select turntable at first
    
    # Create an XYZAxis visual
    axis = scene.visuals.XYZAxis(parent=view)
    s = STTransform(translate=(50, 50), scale=(50, 50, 50, 1))
    affine = s.as_matrix()
    axis.transform = affine
    
    # Use render to generate an image object
    img=canvas.render()

    # Use write_png to export your wonderful plot as png ! 
    io.write_png('wrinkVecField_pic_' + str(i) +'.png',img)
    
# Implement axis connection with cam2
@canvas.events.mouse_move.connect
def on_mouse_move(event):
    if event.button == 1 and event.is_dragging:
        axis.transform.reset()
 
        axis.transform.rotate(cam2.roll, (0, 0, 1))
        axis.transform.rotate(cam2.elevation, (1, 0, 0))
        axis.transform.rotate(cam2.azimuth, (0, 1, 0))
 
        axis.transform.scale((50, 50, 0.001))
        axis.transform.translate((50., 50.))
        axis.update()


#if __name__ == '__main__':
#    print(__doc__)
#    app.run()
    
    ###############################################################################
    #donnot use matplotlib
    ###############################################################################
    #plt.axis('off')	
    #ax.scatter(vertices[:,0],vertices[:,1], vertices[:,2], zdir='z', s=50, c=clrs)
    #plt.savefig('wrinkVecField_pic_' + str(i) +'.png') 

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
    #cbar = plt.colorbar(orientation='vertical')
    #cbar.ax.set_xticklabels(np.arange(np.amin(q), np.amax(q), 10))
    #plt.show()


