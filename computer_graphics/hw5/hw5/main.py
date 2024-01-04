# Import necessary libraries
import pygame as pg
from OpenGL.GL import *
import numpy as np
import pyrr
from guiV1 import SimpleGUI
import pyrr.matrix44 as m4
from objLoaderV3 import ObjLoader
import shaderLoader

# Initialize pygame
pg.init()

# Set up OpenGL context version
pg.display.gl_set_attribute(pg.GL_CONTEXT_MAJOR_VERSION, 3)
pg.display.gl_set_attribute(pg.GL_CONTEXT_MINOR_VERSION, 3)

# Create a window for graphics using OpenGL
width = 640
height = 480
pg.display.set_mode((width, height), pg.OPENGL | pg.DOUBLEBUF)

glClearColor(0.3, 0.4, 0.5, 1.0)
# Todo: Enable depth testing here using glEnable()
glEnable(GL_DEPTH_TEST)


# Todo: Part 3: Write shaders (vertex and fragment shaders) and compile them here
shader = shaderLoader.compile_shader("shaders/vert.glsl", "shaders/frag.glsl")
glUseProgram(shader)

# Todo: Part 1: Read the 3D model
# Lets setup our scene geometry.
obj = ObjLoader("objects/deadpool.obj")
vertices = np.array(obj.vertices, dtype="float32")
center = obj.center
dia = obj.dia

size_position = 3
size_texture = 2
size_normal = 3
stride = 32
offset_position = 0
offset_texture = 3 * 4
offset_normal = 5 * 4
n_vertices = len(vertices)
scale = 2.0 / dia
aspect = width / height
# Todo: Part 2: Upload the model data to the GPU. Create a VAO and VBO for the model data.

# Create a Vertex Buffer Object (VBO) to store the vertex data
vao = glGenVertexArrays(1) # generate a VAO
glBindVertexArray(vao) # Bind the VAO. That is, make it the active one

vbo = glGenBuffers(1)  # Generate one buffer and store its ID.
glBindBuffer(GL_ARRAY_BUFFER, vbo)  # Bind the buffer. That is, make it the active one.
glBufferData(GL_ARRAY_BUFFER, size=vertices.nbytes,  # size of entire vertex data in bytes
             data=vertices,
             usage=GL_STATIC_DRAW)  # Upload the data to the GPU

# Todo: Part 4: Configure vertex attributes using the variables defined in Part 1
position_loc = glGetAttribLocation(shader, "position")

glVertexAttribPointer(index=position_loc,           # Now we specify how the data is stored in the VBO for the position attribute
                      size=size_position,           # Specify the number of components per attribute: 3 for position (x, y, z)
                      type=GL_FLOAT,                # Specify the type of the components
                      normalized=GL_FALSE,          # Specify if we want the data to be normalized
                      stride=stride,                # Specify the stride (number of bytes between each vertex)
                      pointer=ctypes.c_void_p(offset_position))   # Specify the starting point (in bytes) for the position data


glEnableVertexAttribArray(position_loc)


normal_loc = glGetAttribLocation(shader, "normal")    # Get the index of the normal attribute in the shader
glVertexAttribPointer(normal_loc,                    # Now we specify how the data is stored in the VBO for the normal attribute
                      size=size_normal,
                      type=GL_FLOAT,
                      normalized=GL_FALSE,
                      stride=stride,
                      pointer=ctypes.c_void_p(offset_normal))   # The starting point for the normal data (in bytes)
# Enable the vertex attribute (normal) using its index
glEnableVertexAttribArray(normal_loc)

# Todo: Part 5: Configure uniform variables.
scale_loc = glGetUniformLocation(shader, "scale")   # Get the location of the uniform variable "scale" in the shader
glUniform1f(scale_loc, scale)                   # Set the value of the uniform variable "scale" in the shader


center_loc = glGetUniformLocation(shader, "center")   # Get the location of the uniform variable "center" in the shader
glUniform3fv(center_loc, 1, center)    # Set the value of the uniform variable "center" in the shader


aspect_loc = glGetUniformLocation(shader, "aspect")   # Get the location of the uniform variable "aspect" in the shader
glUniform1f(aspect_loc, aspect)                   # Set the value of the uniform variable "aspect" in the shader

# hw5
gui = SimpleGUI("cameraMovement")
angleY_slider = gui.add_slider("camera Y angle", min_value=-90, max_value=90, initial_value=0)

angleX_slider = gui.add_slider("camera X angle", min_value=-90, max_value=90, initial_value=0)

fov_slider = gui.add_slider("fov", min_value=20, max_value=120, initial_value=45)


model_matrix = m4.create_identity()
eye = [0, 0, center[2] + dia]
view_matrix = m4.create_look_at(eye, center, [0, 1, 0])
projection_matrix = m4.create_perspective_projection_matrix(fov_slider.get_value(), width / height, 0.1, 1000)


model_mat_location = glGetUniformLocation(shader, "model_mat")
view_mat_location = glGetUniformLocation(shader, "view_mat")
projection_mat_location = glGetUniformLocation(shader, "projection_mat")



# Todo: Part 6: Do the final rendering. In the rendering loop, do the following:
# - Clear the color buffer and depth buffer before drawing each frame using glClear()
# - Use the shader program using glUseProgram()
# - Bind the VAO using glBindVertexArray()
# - Draw the triangle using glDrawArrays()


# Run a loop to keep the program running
draw = True
while draw:
    for event in pg.event.get():
        if event.type == pg.QUIT:
            draw = False

    transformed_eye = [0, 0, 0]
    transformed_eye[0] = eye[0] - obj.center[0]  # Initialize with the initial dia value
    transformed_eye[1] = eye[1] - obj.center[1]
    transformed_eye[2] = eye[2] - obj.center[2]

    # Rotate the camera position using the rotation matrix
    rotation_mat_x = pyrr.Matrix44.from_x_rotation(angleX_slider.get_value())
    rotation_mat_y = pyrr.Matrix44.from_y_rotation(angleY_slider.get_value())
    combined_rotation = pyrr.matrix44.multiply(rotation_mat_x, rotation_mat_y)

    rotated_eye = pyrr.matrix44.apply_to_vector(combined_rotation, transformed_eye)

    # Translate the camera position back to its original position
    camera_position = [rotated_eye[0] + center[0], rotated_eye[1] + center[1], rotated_eye[2] + center[2]]

    # Update the view matrix with the new camera position
    view_matrix = pyrr.matrix44.create_look_at(camera_position, center, [0, 1, 0])
    projection_matrix = m4.create_perspective_projection_matrix(fov_slider.get_value(), width / height, 0.1, 1000)

    # Translate the camera position by the negative of the center of the scene
    glUniformMatrix4fv(model_mat_location, 1, GL_FALSE, model_matrix)
    glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view_matrix)
    glUniformMatrix4fv(projection_mat_location, 1, GL_FALSE, projection_matrix)

    # Clear color buffer and depth buffer before drawing each frame
    glClear(GL_COLOR_BUFFER_BIT)

    glUseProgram(shader)   # Use the shader program using glUseProgram()

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) # Clear the color buffer and depth buffer before drawing each frame using glClear()

    glDrawArrays(GL_TRIANGLES,
                 0,
                 n_vertices)  # Draw the triangle

    # Refresh the display to show what's been drawn
    pg.display.flip()

# Cleanup
glDeleteVertexArrays(1, [vao])
glDeleteBuffers(1, [vbo])
glDeleteProgram(shader)

pg.quit()  # Close the graphics window
quit()  # Exit the program
