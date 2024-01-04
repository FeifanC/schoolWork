# Import necessary libraries
import pygame as pg
from OpenGL.GL import *
import numpy as np
import pyrr
from guiV1 import SimpleGUI
import pyrr.matrix44 as m4
from objLoaderV4 import ObjLoader
import shaderLoader

# Initialize pygame
pg.init()

# Set up OpenGL context version
pg.display.gl_set_attribute(pg.GL_CONTEXT_MAJOR_VERSION, 3)
pg.display.gl_set_attribute(pg.GL_CONTEXT_MINOR_VERSION, 3)

# Create a window for graphics using OpenGL
width = 640
height = 480
pg.display.set_mode((width * 2, height), pg.OPENGL | pg.DOUBLEBUF)

# glClearColor(0.3, 0.4, 0.5, 1.0)

glEnable(GL_DEPTH_TEST)
glEnable(GL_SCISSOR_TEST)

# shader1 for rabbit
shader1 = shaderLoader.compile_shader("shaders/vert.glsl", "shaders/frag1.glsl")
glUseProgram(shader1)

# Todo: Part 1: Read the 3D model
# Lets setup our scene geometry.
obj1 = ObjLoader("objects/raymanModel.obj")
obj2 = ObjLoader("objects/dragon.obj")

vertices1 = np.array(obj1.vertices, dtype="float32")
center1 = obj1.center
dia1 = obj1.dia

vertices2 = np.array(obj2.vertices, dtype="float32")
center2 = obj2.center
dia2 = obj2.dia

size_position = 3
stride = 32
offset_position = 0
size_normal = 3
offset_normal = 20

# Create a Vertex Buffer Object (VBO) to store the vertex data for the obj1
vao1 = glGenVertexArrays(1)
glBindVertexArray(vao1)                 # Bind the VAO. That is, make it the active one.
vbo1 = glGenBuffers(1)                  # Generate one buffer and store its ID.
glBindBuffer(GL_ARRAY_BUFFER, vbo1)     # Bind the buffer. That is, make it the active one.
glBufferData(GL_ARRAY_BUFFER, size=vertices1.nbytes,  # size of entire vertex data in bytes
             data=vertices1,
             usage=GL_STATIC_DRAW)  # Upload the data to the GPU
pos_loc1 = glGetAttribLocation(shader1, "position")
glVertexAttribPointer(index=pos_loc1,           # Now we specify how the data is stored in the VBO for the position attribute
                      size=size_position,           # Specify the number of components per attribute: 3 for position (x, y, z)
                      type=GL_FLOAT,                # Specify the type of the components
                      normalized=GL_FALSE,          # Specify if we want the data to be normalized
                      stride=stride,                # Specify the stride (number of bytes between each vertex)
                      pointer=ctypes.c_void_p(offset_position))   # Specify the starting point (in bytes) for the position data
normal_loc1 = glGetAttribLocation(shader1, "normal")

glVertexAttribPointer(normal_loc1,
                      size=size_normal,
                      type=GL_FLOAT,
                      normalized=GL_FALSE,
                      stride=stride,
                      pointer=ctypes.c_void_p(offset_normal))   # The starting point for the normal data (in bytes)
# Enable the vertex attribute (normal) using its index glEnableVertexAttribArray(pos_loc1)

glEnableVertexAttribArray(normal_loc1)
glEnableVertexAttribArray(pos_loc1)


# shader2 for dragon
shader2 = shaderLoader.compile_shader("shaders/vert.glsl", "shaders/frag2.glsl")
glUseProgram(shader2)

# Create a Vertex Buffer Object (VBO) to store the vertex data for the obj1
vao2 = glGenVertexArrays(1)
glBindVertexArray(vao2)                 # Bind the VAO. That is, make it the active one.
vbo2 = glGenBuffers(1)                  # Generate one buffer and store its ID.
glBindBuffer(GL_ARRAY_BUFFER, vbo2)     # Bind the buffer. That is, make it the active one.
glBufferData(GL_ARRAY_BUFFER, size=vertices2.nbytes,  # size of entire vertex data in bytes
             data=vertices2,
             usage=GL_STATIC_DRAW)  # Upload the data to the GPU

pos_loc2 = glGetAttribLocation(shader2, "position")
normal_loc2 = glGetAttribLocation(shader2, "normal")
glVertexAttribPointer(index=pos_loc2,           # Now we specify how the data is stored in the VBO for the position attribute
                      size=size_position,           # Specify the number of components per attribute: 3 for position (x, y, z)
                      type=GL_FLOAT,                # Specify the type of the components
                      normalized=GL_FALSE,          # Specify if we want the data to be normalized
                      stride=stride,                # Specify the stride (number of bytes between each vertex)
                      pointer=ctypes.c_void_p(offset_position))   # Specify the starting point (in bytes) for the position data

glVertexAttribPointer(normal_loc2,                    # Now we specify how the data is stored in the VBO for the normal attribute
                      size=size_normal,
                      type=GL_FLOAT,
                      normalized=GL_FALSE,
                      stride=stride,
                      pointer=ctypes.c_void_p(offset_normal))   # The starting point for the normal data (in bytes)
# Enable the vertex attribute (normal) using its index glEnableVertexAttribArray(pos_loc1)
glEnableVertexAttribArray(pos_loc2)
glEnableVertexAttribArray(normal_loc2)


# hw6
#Create the 4x4 translation matrix to translate the object to the origin
translation_matrix1 = m4.create_from_translation(pyrr.Vector3(-obj1.center))
translation_matrix2 = m4.create_from_translation(pyrr.Vector3(-obj2.center))

# Create the scaling matrix
scale_factor1 = 2.0 / obj1.dia
scale_factor2 = 2.0 / obj2.dia

scaling_matrix1 = m4.create_from_scale(pyrr.Vector3([scale_factor1] * 3))
scaling_matrix2 = m4.create_from_scale(pyrr.Vector3([scale_factor2] * 3))

gui = SimpleGUI("cameraMovement")
angleY_slider = gui.add_slider("camera Y angle", min_value=-180, max_value=180, initial_value=0)
angleX_slider = gui.add_slider("camera X angle", min_value=-90, max_value=90, initial_value=0)
fov_slider = gui.add_slider("fov", min_value=25, max_value=120, initial_value=45)


# Create a view matrix
eye = [0,0,2]
view_matrix1 = m4.create_look_at(eye, [0,0,0], [0, 1, 0])
view_matrix2 = m4.create_look_at(eye, [0,0,0], [0, 1, 0])

projection_matrix = m4.create_perspective_projection_matrix(45, width / height, 0.1, 10)

model_mat_location1 = glGetUniformLocation(shader1, "model_mat")
view_mat_location1 = glGetUniformLocation(shader1, "view_mat")
projection_mat_location1 = glGetUniformLocation(shader1, "projection_mat")

model_mat_location2 = glGetUniformLocation(shader2, "model_mat")
view_mat_location2 = glGetUniformLocation(shader2, "view_mat")
projection_mat_location2 = glGetUniformLocation(shader2, "projection_mat")

# Run a loop to keep the program running
draw = True
while draw:
    for event in pg.event.get():
        if event.type == pg.QUIT:
            draw = False

    # Clear color buffer and depth buffer before drawing each frame
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)  # Clear the color buffer and depth buffer before drawing each frame using glClear()

    # Rotate the camera position using the rotation matrix
    rotation_mat_x = pyrr.Matrix44.from_x_rotation(np.deg2rad(angleX_slider.get_value()))
    rotation_mat_y = pyrr.Matrix44.from_y_rotation(np.deg2rad(angleY_slider.get_value()))

    projection_matrix = m4.create_perspective_projection_matrix(fov_slider.get_value(), width / height, 0.1, 10)

    # camera_position1 = [rotated_eye[0] + center1[0], rotated_eye[1] + center1[1], rotated_eye[2] + center1[2]]
    view_matrix1 = pyrr.matrix44.create_look_at(eye, [0, 0, 0], [0, 1, 0])
    view_matrix2 = pyrr.matrix44.create_look_at(eye, [0, 0, 0], [0, 1, 0])

    model_matrix1 = pyrr.matrix44.multiply(translation_matrix1, rotation_mat_x)
    model_matrix1 = pyrr.matrix44.multiply(model_matrix1, rotation_mat_y)
    model_matrix1 = pyrr.matrix44.multiply(model_matrix1, scaling_matrix1)

    model_matrix2 = pyrr.matrix44.multiply(translation_matrix2, rotation_mat_x)
    model_matrix2 = pyrr.matrix44.multiply(model_matrix2, rotation_mat_y)
    model_matrix2 = pyrr.matrix44.multiply(model_matrix2, scaling_matrix1)

    # for the rabbit
    glViewport(0, 0, width, height)
    glScissor(0, 0, width, height)
    glClearColor(0.3, 0.4, 0.5, 1.0)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

    glUseProgram(shader1)  # Use the shader program using glUseProgram()
    glUniformMatrix4fv(model_mat_location1, 1, GL_FALSE, model_matrix1)
    glUniformMatrix4fv(view_mat_location1, 1, GL_FALSE, view_matrix1)
    glUniformMatrix4fv(projection_mat_location1, 1, GL_FALSE, projection_matrix)
    glBindVertexArray(vao1)
    glDrawArrays(GL_TRIANGLES, 0, obj1.n_vertices)  # Draw the triangle

    # for the dragon
    # camera_position2 = [rotated_eye[0] + center2[0], rotated_eye[1] + center2[1], rotated_eye[2] + center2[2]]

    # view_matrix2 = pyrr.matrix44.create_look_at(camera_position2, center2, [0, 1, 0])

    glViewport(width, 0, width, height)
    glScissor(width, 0, width, height)
    glClearColor(0.2, 0.3, 0.4, 1.0)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

    glUseProgram(shader2)   # Use the shader program using glUseProgram()
    glUniformMatrix4fv(model_mat_location2, 1, GL_FALSE, model_matrix2)
    glUniformMatrix4fv(view_mat_location2, 1, GL_FALSE, view_matrix2)
    glUniformMatrix4fv(projection_mat_location2, 1, GL_FALSE, projection_matrix)
    glBindVertexArray(vao2)
    glDrawArrays(GL_TRIANGLES,0, obj2.n_vertices)  # Draw the triangle
    # Refresh the display to show what's been drawn
    pg.display.flip()

# Cleanup
glDeleteVertexArrays(1, [vao1])
glDeleteBuffers(1, [vbo1])

glDeleteVertexArrays(1, [vao2])
glDeleteBuffers(1, [vbo2])

glDeleteProgram(shader1)
glDeleteProgram(shader2)


pg.quit()  # Close the graphics window
quit()  # Exit the program
