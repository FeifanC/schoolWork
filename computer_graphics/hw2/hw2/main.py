# Import necessary libraries
import pygame as pg
from OpenGL.GL import *
import numpy as np
from objLoaderV1 import ObjLoader

# Initialize pygame
pg.init()

# Set up OpenGL context version
pg.display.gl_set_attribute(pg.GL_CONTEXT_MAJOR_VERSION, 3)
pg.display.gl_set_attribute(pg.GL_CONTEXT_MINOR_VERSION, 3)

# Create a window for graphics using OpenGL
width = 640
height = 480
pg.display.set_mode((width, height), pg.OPENGL | pg.DOUBLEBUF)

# Todo: Part 1
# Set the background color (clear color)
# glClearColor takes 4 arguments: red, green, blue, alpha. Each argument is a float between 0 and 1.
glClearColor(1.0, 0.0, 0.0, 1.0)


# Todo: Part 2
obj = ObjLoader("objects/raymanModel.obj")
positions = obj.v
print("Dimension of v: ", obj.v.shape)

texture_coordinates = obj.vt
print("Dimension of vt: ", obj.vt.shape)

normal_coordinates = obj.vn
print("Dimension of vn: ", obj.vn.shape)

vertices = obj.vertices         # 1D array of vertices (position, texture, normal)
print("Dimension of vertices: ", vertices.shape)

# Todo: Part 3
def compute_extent(v):
    xMin, yMin, zMin = v[0]
    xMax, yMax, zMax = v[0]
    
    for i in v:
        x, y, z = i
       
        xMin = min(xMin, x)
        yMin = min(yMin, y)
        zMin = min(zMin, z)
        xMax = max(xMax, x)
        yMax = max(yMax, y)
        zMax = max(zMax, z)

    xCenter = (xMax + xMin) / 2
    yCenter = (yMax + yMin) / 2
    zCenter = (zMax + zMin) / 2

    diameter = np.sqrt((xMax - xMin) ** 2 + (yMax - yMin) ** 2 + (zMax - zMin) ** 2)

    print("Min: [", xMin, yMin, zMin, " ]")
    print("Max: [", xMax, yMax, zMax, " ]")
    print("Center: [", xCenter, yCenter, zCenter, " ]")
    print("Diameter: [", diameter, " ]")
    
compute_extent(obj.v)

# Todo: Part 4
vbo = glGenBuffers(1)
glBindBuffer(GL_ARRAY_BUFFER, vbo)
glBufferData(GL_ARRAY_BUFFER,
size=vertices.nbytes,
data=vertices,
usage=GL_STATIC_DRAW)


# Run a loop to keep the program running
draw = True
while draw:
    for event in pg.event.get():
        if event.type == pg.QUIT:
            draw = False

    # Clear the screen (or clear the color buffer), and set it to the background color chosen earlier
    glClear(GL_COLOR_BUFFER_BIT)

    # Refresh the display to show what's been drawn
    pg.display.flip()


pg.quit()   # Close the graphics window
quit()      # Exit the program
