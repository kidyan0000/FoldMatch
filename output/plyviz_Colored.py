# -*- coding: utf-8 -*-
# -----------------------------------------------------------------------------
# Copyright (c) 2015, Vispy Development Team. All Rights Reserved.
# Distributed under the (new) BSD License. See LICENSE.txt for more info.
# -----------------------------------------------------------------------------
# vispy: gallery 2
 
"""
Example volume rendering
 
Controls:
 
* 1  - toggle camera between first person (fly), regular 3D (turntable) and
       arcball
* 2  - toggle between volume rendering methods
* 3  - toggle between stent-CT / brain-MRI image
* 4  - toggle between colormaps
* 0  - reset cameras
* [] - decrease/increase isosurface threshold
 
With fly camera:
 
* WASD or arrow keys - move around
* SPACE - brake
* FC - move up-down
* IJKL or mouse - look around
"""
 
from itertools import cycle
 
import numpy as np

from vispy import app, scene, io

from vispy.scene import visuals
from vispy.color import get_colormaps, BaseColormap
from vispy.visuals.transforms import STTransform

from locale import atoi, atof
from plyfile import PlyData, PlyElement

# Read volume

referenceply = PlyData.read("../results/reference.ply")
templateply = PlyData.read("../results/template.ply")
frames = 250
 
# Prepare canvas
canvas = scene.SceneCanvas(keys='interactive', size=(800, 600), show=True)
canvas.measure_fps()
 
# Set up a viewbox to display the image with interactive pan/zoom
view = canvas.central_widget.add_view()
 
# Set whether we are emulating a 3D texture
emulate_texture = True
 
# Create the volume visuals, only one is visible
reference = visuals.Markers()
ref = np.column_stack((referenceply['vertex']['x'],referenceply['vertex']['y']))
ref = np.column_stack((ref, referenceply['vertex']['z']))
refColors = np.column_stack( (referenceply['vertex']['red'], referenceply['vertex']['green'], referenceply['vertex']['blue']) )
refColors = refColors / 255.0
refColors = np.column_stack( (refColors , np.ones(len(ref), dtype=float)) )
reference.set_data(ref, edge_color=None, face_color=refColors , size=5)
view.add(reference)

template = visuals.Markers()
tem = np.column_stack((templateply['vertex']['x'],templateply['vertex']['y']))
tem = np.column_stack((tem, templateply['vertex']['z']))
temColors = np.column_stack((templateply['vertex']['red'], templateply['vertex']['green'], templateply['vertex']['blue']))
temColors = temColors / 255.0
temColors = np.column_stack((temColors , np.ones(len(tem), dtype=float)) )
template.set_data(tem, edge_color=None, face_color=temColors, size=5)
view.add(template)

# Create three cameras (Fly, Turntable and Arcball)
fov = 60.
cam1 = scene.cameras.FlyCamera(parent=view.scene, fov=fov, name='Fly')
cam2 = scene.cameras.TurntableCamera(parent=view.scene, fov=fov, name='Turntable')
cam3 = scene.cameras.ArcballCamera(parent=view.scene, fov=fov, name='Arcball')
view.camera = cam2  # Select turntable at first
 
# Create an XYZAxis visual
axis = scene.visuals.XYZAxis(parent=view)
s = STTransform(translate=(50, 50), scale=(50, 50, 50, 1))
affine = s.as_matrix()
axis.transform = affine

#update function

it=0
def update(ev):
	global it
	it +=1
	if it > frames:
		it=1
	s = str(it)
	if it < 10:
		s = '00' + s
	elif it >= 10 and it < 100:
		s = '0' + s
	templateply = PlyData.read("../results/template_" + str(it) +  ".ply")
	tem = np.column_stack((templateply['vertex']['x'],templateply['vertex']['y']))
	tem = np.column_stack((tem, templateply['vertex']['z'])) 
	temColors = np.column_stack((templateply['vertex']['red'], templateply['vertex']['green'], templateply['vertex']['blue']))
	temColors = temColors / 255.0
	temColors = np.column_stack((temColors , np.ones(len(tem), dtype=float)) )	  	
	template.set_data(tem, edge_color=None, face_color=temColors, size=5)
	view.add(template)

# setup timer

interv = 0.1 
timer = app.Timer(interval = interv)
timer.connect(update)
timer.start(interval = interv)

# create colormaps that work well for translucent and additive volume rendering
class TransFire(BaseColormap):
    glsl_map = """
    vec4 translucent_fire(float t) {
        return vec4(pow(t, 0.5), t, t*t, max(0, t*1.05 - 0.05));
    }
    """
 
 
class TransGrays(BaseColormap):
    glsl_map = """
    vec4 translucent_grays(float t) {
        return vec4(t, t, t, t*0.05);
    }
    """
 
# Setup colormap iterators
opaque_cmaps = cycle(get_colormaps())
translucent_cmaps = cycle([TransFire(), TransGrays()])
opaque_cmap = next(opaque_cmaps)
translucent_cmap = next(translucent_cmaps)
 
 
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
 

# Implement key presses
@canvas.events.key_press.connect
def on_key_press(event):	
	global opaque_cmap, translucent_cmap, it
	if event.text == 'n':
		it+=10
		if it > frames:
			it=1
		s = str(it)
		if it < 10:
			s = '00' + s
		elif it >= 10 and it < 100:
			s = '0' + s
		templateply = PlyData.read("../results/template_" + str(it) +  ".ply")
		tem = np.column_stack((templateply['vertex']['x'],templateply['vertex']['y']))
		tem = np.column_stack((tem, templateply['vertex']['z'])) 
		temColors = np.column_stack((templateply['vertex']['red'], templateply['vertex']['green'], templateply['vertex']['blue']))
		temColors = temColors / 255.0
		temColors = np.column_stack((temColors , np.ones(len(tem), dtype=float)) )  	
		template.set_data(tem, edge_color=None, face_color=temColors, size=5)
		view.add(template)

	elif event.text == '1':
		cam_toggle = {cam1: cam2, cam2: cam3, cam3: cam1}
		view.camera = cam_toggle.get(view.camera, cam2)
		print(view.camera.name + ' camera')
		if view.camera is cam2:
			axis.visible = True
		else:
			axis.visible = False
	elif event.text == '2':
		methods = ['mip', 'translucent', 'iso', 'additive']
		method = methods[(methods.index(volume1.method) + 1) % 4]
		print("Volume render method: %s" % method)
		cmap = opaque_cmap if method in ['mip', 'iso'] else translucent_cmap
		volume1.method = method
		volume1.cmap = cmap
		volume2.method = method
		volume2.cmap = cmap
	elif event.text == '3':
		volume1.visible = not volume1.visible
		volume2.visible = not volume1.visible
	elif event.text == '4':
		if volume1.method in ['mip', 'iso']:
			cmap = opaque_cmap = next(opaque_cmaps)
		else:
			cmap = translucent_cmap = next(translucent_cmaps)
		volume1.cmap = cmap
		volume2.cmap = cmap
	elif event.text == '0':
		cam1.set_range()
		cam3.set_range()
	elif event.text != '' and event.text in '[]':
		s = -0.025 if event.text == '[' else 0.025
		volume1.threshold += s
		volume2.threshold += s
		th = volume1.threshold if volume1.visible else volume2.threshold
		print("Isosurface threshold: %0.3f" % th)
		
 
# for testing performance
# @canvas.connect
# def on_draw(ev):
# canvas.update()
 
if __name__ == '__main__':
    print(__doc__)
    app.run()
