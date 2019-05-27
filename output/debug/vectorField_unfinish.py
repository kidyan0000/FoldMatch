# -*- coding: utf-8 -*-
# -----------------------------------------------------------------------------
# Copyright (c) Vispy Development Team. All Rights Reserved.
# Distributed under the (new) BSD License. See LICENSE.txt for more info.
# -----------------------------------------------------------------------------

"""
Show vector field flow
"""

from __future__ import division

from vispy import app, scene, visuals, gloo
from vispy.util import ptime
import numpy as np
import fileinput
import re
import os
import re
import numpy as np
import pymesh
import vispy.scene
from plyfile import PlyData, PlyElement, PlyProperty, PlyListProperty
from math import ceil
import sympy
import matplotlib.pyplot as plt
import matplotlib.colors as colors
from matplotlib.mlab import bivariate_normal
from matplotlib.colors import PowerNorm
from mpl_toolkits.mplot3d import Axes3D
from rbf.basis import RBF, get_r, get_eps

class VectorFieldVisual(visuals.Visual):
    vertex = """
    uniform sampler2D field;
    attribute vec2 index;
    uniform vec2 shape;
    uniform vec2 field_shape;
    uniform float spacing;
    varying float dist;  // distance along path for this vertex
    varying vec2 ij;
    uniform sampler2D offset;
    uniform float seg_len;
    uniform int n_iter;  // iterations to integrate along field per vertex
    uniform vec2 attractor;
    varying vec4 base_color;
    uniform sampler2D color;
    
    void main() {
        // distance along one line
        dist = index.y * seg_len;
        
        vec2 local;
        ij = vec2(mod(index.x, shape.x), floor(index.x / shape.x));
        // *off* is a random offset to the starting location, which prevents
        // the appearance of combs in the field 
        vec2 off = texture2D(offset, ij / shape).xy - 0.5;
        local = spacing * (ij + off);
        vec2 uv;
        vec2 dir;
        vec2 da;
        int index_y = int(index.y);
        for( int i=0; i<index.y; i+=1 ) {
            for ( int j=0; j<n_iter; j += 1 ) {
                uv = local / field_shape;
                dir = texture2D(field, uv).xy;
                
                // add influence of variable attractor (mouse)
                da = attractor - local;
                float al = 0.1 * length(da);
                da /= 0.5 * (1 + al*al);
                
                dir += da;
                
                // maybe pick a more accurate integration method?
                local += seg_len * dir / n_iter;
            }
        }
        base_color = texture2D(color, uv);
        
        gl_Position = $transform(vec4(local, 0, 1));
    }
    """
    
    fragment = """
    uniform float time;
    uniform float speed;
    varying float dist;
    varying vec2 ij;
    uniform sampler2D offset;
    uniform vec2 shape;
    uniform float nseg;
    uniform float seg_len;
    varying vec4 base_color;
    
    void main() {
        float totlen = nseg * seg_len;
        float phase = texture2D(offset, ij / shape).b;
        float alpha;
        
        // vary alpha along the length of the line to give the appearance of
        // motion
        alpha = mod((dist / totlen) + phase - time * speed, 1);
        
        // add a cosine envelope to fade in and out smoothly at the ends
        alpha *= (1 - cos(2 * 3.141592 * dist / totlen)) * 0.5;
        
        gl_FragColor = vec4(base_color.rgb, base_color.a * alpha);
    }
    """
    
    def __init__(self, field, spacing=10, segments=3, seg_len=0.5,
                 color=(1, 1, 1, 0.3)):
        self._time = 0.0
        self._last_time = ptime.time()
        rows = int(field.shape[0] / spacing)
        cols = int(field.shape[1] / spacing)
        index = np.empty((rows * cols, int(segments) * 2, 2), dtype=np.float32)
        
        # encodes starting position within vector field
        index[:, :, 0] = np.arange(rows * cols)[:, np.newaxis]
        # encodes distance along length of line
        index[:, ::2, 1] = np.arange(segments)[np.newaxis, :]
        index[:, 1::2, 1] = np.arange(segments)[np.newaxis, :] + 1
        self._index = gloo.VertexBuffer(index)
        if not isinstance(color, np.ndarray):
            color = np.array([[list(color)]], dtype='float32')
        self._color = gloo.Texture2D(color)
        
        offset = np.random.uniform(256, size=(rows, cols, 3)).astype(np.ubyte)
        self._offset = gloo.Texture2D(offset, format='rgb')
        self._field = gloo.Texture2D(field, format='rg',
                                     internalformat='rg32f',
                                     interpolation='linear')
        self._field_shape = field.shape[:2]
        
        visuals.Visual.__init__(self, vcode=self.vertex, fcode=self.fragment)
        self.timer = app.Timer(interval='auto', connect=self.update_time,
                               start=False)
        self.freeze()
        
        self.shared_program['field'] = self._field
        self.shared_program['field_shape'] = self._field.shape[:2]
        self.shared_program['shape'] = (rows, cols)
        self.shared_program['index'] = self._index
        self.shared_program['spacing'] = spacing
        self.shared_program['t'] = self._time
        self.shared_program['offset'] = self._offset
        self.shared_program['speed'] = 1
        self.shared_program['color'] = self._color
        self.shared_program['seg_len'] = seg_len
        self.shared_program['nseg'] = segments
        self.shared_program['n_iter'] = 1
        self.shared_program['attractor'] = (0, 0)
        self.shared_program['time'] = 0
        self._draw_mode = 'lines'
        self.set_gl_state('translucent', depth_test=False)
        
        self.timer.start()
        
    def _prepare_transforms(self, view):
        view.view_program.vert['transform'] = view.get_transform()
        
    def _prepare_draw(self, view):
        pass
    
    def _compute_bounds(self, axis, view):
        if axis > 1:
            return (0, 0)
        return (0, self._field_shape[axis])

    def update_time(self, ev):
        t = ptime.time()
        self._time += t - self._last_time
        self._last_time = t
        self.shared_program['time'] = self._time
        self.update()


VectorField = scene.visuals.create_visual_node(VectorFieldVisual)


def fn(y, x):
    dx = x-50
    dy = y-30
    hyp = (dx**2 + dy**2)**0.5 + 0.01
    return np.array([100 * dy / hyp**1.7, -100 * dx / hyp**1.8])

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

for i in range(2, 3):
    plyFileName = 'wrinkVecField_' + str(i) + '.ply'
    plydata = PlyData.read(plyFileName)

    x = plydata['vertex']['x']
    y = plydata['vertex']['y']
    z = plydata['vertex']['z']
    #vertices = getNormalizePointCloud(np.column_stack((x, y, z)))
    vertices = np.column_stack((x, y, z))

    q = plydata['vertex']['quality']

    #face_num = len(plydata['face']['vertex_indices'])
    face = plydata['face']['vertex_indices']
    
    #for f in range(0,len(face)):
    #    print face[f][0]    
    
    #faces = np.column_stack(face)
    #print face_num

    fig = plt.figure( 1, figsize = (10,10))
    ax = fig.add_subplot(111, projection='3d')

    #orig_Jetcmap = plt.cm.jet
    #shiftedJet_cmap = shiftedColorMap(orig_Jetcmap, start=0.0, midpoint=0.2, stop=1.0, name='shifted')
    #clrs = shiftedJet_cmap(q)
    #clrs_jet = plt.cm.jet(q)
    
    norm = colors.PowerNorm(gamma=0.25)    
    clrs = plt.cm.jet(norm(q))
    print clrs 
    
    #q = matplotlib.colors.SymLogNorm(1,vmin=0.0,vmax=0.6)
    #clrs = plt.cm.jet(norms(q))
    color_255 = np.ceil(clrs * 255)

#field = np.fromfunction(fn, (100, 100)).transpose(1, 2, 0).astype('float32')
#field[..., 0] += 10 * np.cos(np.linspace(0, 2 * 3.1415, 100))
#print field
field = vertices[1:101]
print len(field)
#field[..., 0] += 10 * np.cos(np.linspace(0, 2 * 3.1415, len(vertices)))

#color = np.zeros((100, 100, 4), dtype='float32')
#color[..., :2] = (field + 5) / 10.
#color[..., 2] = 0.5
#color[..., 3] = 0.5
color = color_255

canvas = scene.SceneCanvas(keys='interactive', show=True)
view = canvas.central_widget.add_view(camera='panzoom')

vfield = VectorField(field[..., :2], spacing=0.5, segments=30, seg_len=0.05,
                     parent=view.scene, color=color)
view.camera.set_range()


@canvas.connect
def on_mouse_move(event):
    if 3 in event.buttons:
        tr = canvas.scene.node_transform(vfield)
        vfield.shared_program['attractor'] = tr.map(event.pos)[:2]


if __name__ == '__main__':
    app.run()

