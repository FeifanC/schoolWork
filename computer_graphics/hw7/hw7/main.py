# Import necessary libraries
import pygame as pg
from OpenGL.GL import *
import numpy as np
import pyrr
from guiV2 import SimpleGUI
import pyrr.matrix44 as m4
from objLoaderV4 import ObjLoader
import shaderLoaderV2 as shaderLoader

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
glClearColor(0.3, 0.4, 0.5, 1.0)

# shader1 for rabbit
shader = shaderLoader.compile_shader("shaders/vert.glsl", "shaders/frag.glsl")
glUseProgram(shader)

# Todo: Part 1: Read the 3D model
# Lets setup our scene geometry.
obj1 = ObjLoader("objects/dragon.obj")

vertices = np.array(obj1.vertices, dtype="float32")
center = obj1.center
dia = obj1.dia

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
glBufferData(GL_ARRAY_BUFFER, size=vertices.nbytes,  # size of entire vertex data in bytes
             data=vertices,
             usage=GL_STATIC_DRAW)  # Upload the data to the GPU
pos_loc1 = glGetAttribLocation(shader, "position")
glVertexAttribPointer(index=pos_loc1,           # Now we specify how the data is stored in the VBO for the position attribute
                      size=size_position,           # Specify the number of components per attribute: 3 for position (x, y, z)
                      type=GL_FLOAT,                # Specify the type of the components
                      normalized=GL_FALSE,          # Specify if we want the data to be normalized
                      stride=stride,                # Specify the stride (number of bytes between each vertex)
                      pointer=ctypes.c_void_p(offset_position))   # Specify the starting point (in bytes) for the position data
normal_loc1 = glGetAttribLocation(shader, "normal")

glVertexAttribPointer(normal_loc1,
                      size=size_normal,
                      type=GL_FLOAT,
                      normalized=GL_FALSE,
                      stride=stride,
                      pointer=ctypes.c_void_p(offset_normal))   # The starting point for the normal data (in bytes)
# Enable the vertex attribute (normal) using its index glEnableVertexAttribArray(pos_loc1)
glEnableVertexAttribArray(normal_loc1)
glEnableVertexAttribArray(pos_loc1)

# hw6
#Create the 4x4 translation matrix to translate the object to the origin
x, y, z = center

translation_matrix2 = m4.create_from_translation(pyrr.Vector3([x, -y, 0]))
# pos for dragon2
translation_matrix1 = m4.create_from_translation(pyrr.Vector3([x - dia / 3, -y, 0]))
# pos for dragon3
translation_matrix3 = m4.create_from_translation(pyrr.Vector3([x + dia / 3, -y, 0]))

material_color = (1.0, 0.0, 0.0)
light_pos = np.array([2, 2, 2, None], dtype=np.float32)
specular_color = (1.0, 1.0, 1.0)
ambient_intensity = 0.25

# Create the scaling matrix
scale_factor1 = 3.0 / obj1.dia

scaling_matrix = m4.create_from_scale(pyrr.Vector3([scale_factor1] * 3))

gui = SimpleGUI("assignment 7")
angleY_slider = gui.add_slider("camera Y angle", min_value=-180, max_value=180, initial_value=0)
angleX_slider = gui.add_slider("camera X angle", min_value=-90, max_value=90, initial_value=0)
fov_slider = gui.add_slider("fov", min_value=25, max_value=120, initial_value=60)

shininess_slider = gui.add_slider("shininess", min_value=1, max_value=128, initial_value=32, resolution=1)
K_s_slider = gui.add_slider("K_s", min_value=0.00, max_value=1.00, initial_value=0.5, resolution=0.01)

material_color_picker = gui.add_color_picker("material color", initial_color=material_color)
specular_color_picker = gui.add_color_picker("specular color", initial_color=specular_color)
light_type_radio_button = gui.add_radio_buttons("light type", options_dict={"point": 1, "directional": 0}, initial_option = "point")

eye = [0,0,2]
model_mat_location1 = glGetUniformLocation(shader, "model_matrix")
view_mat_location1 = glGetUniformLocation(shader, "view_matrix")
projection_mat_location1 = glGetUniformLocation(shader, "projection_matrix")

eye_pos_loc = glGetUniformLocation(shader, "eye_pos")
light_pos_loc = glGetUniformLocation(shader, "light_pos")
ambient_intensity_loc = glGetUniformLocation(shader, "ambient_intensity")
material_color_loc = glGetUniformLocation(shader, "material_color")
specular_color_loc = glGetUniformLocation(shader, "specular_color")
shininess_loc = glGetUniformLocation(shader, "shininess")
K_s_loc = glGetUniformLocation(shader, "K_s")

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
    view_matrix = pyrr.matrix44.create_look_at(eye, [0, 0, 0], [0, 1, 0])

    light_pos[3] = light_type_radio_button.get_value()

    # for dragon 1
    shader = shaderLoader.compile_shader("shaders/vert.glsl", "shaders/frag.glsl")
    glUseProgram(shader)

    model_matrix1 = pyrr.matrix44.multiply(translation_matrix1, rotation_mat_x)
    model_matrix1 = pyrr.matrix44.multiply(model_matrix1, rotation_mat_y)
    model_matrix1 = pyrr.matrix44.multiply(model_matrix1, scaling_matrix)

    glUniformMatrix4fv(model_mat_location1, 1, GL_FALSE, model_matrix1)
    glUniformMatrix4fv(view_mat_location1, 1, GL_FALSE, view_matrix)
    glUniformMatrix4fv(projection_mat_location1, 1, GL_FALSE, projection_matrix)
    glUniform4fv(light_pos_loc, 1, light_pos)
    glUniform3fv(material_color_loc, 1, material_color_picker.get_color())

    glBindVertexArray(vao1)
    glDrawArrays(GL_TRIANGLES, 0, obj1.n_vertices)  # Draw the triangle

    glDeleteProgram(shader)

    # for the dragon2
    shader = shaderLoader.compile_shader("shaders/vert.glsl", "shaders/frag.glsl")
    glUseProgram(shader)

    model_matrix1 = pyrr.matrix44.multiply(translation_matrix2, rotation_mat_x)
    model_matrix1 = pyrr.matrix44.multiply(model_matrix1, rotation_mat_y)
    model_matrix1 = pyrr.matrix44.multiply(model_matrix1, scaling_matrix)

    glUniformMatrix4fv(model_mat_location1, 1, GL_FALSE, model_matrix1)
    glUniformMatrix4fv(view_mat_location1, 1, GL_FALSE, view_matrix)
    glUniformMatrix4fv(projection_mat_location1, 1, GL_FALSE, projection_matrix)

    glUniform3fv(eye_pos_loc, 1, eye)
    glUniform3fv(specular_color_loc, 1, specular_color_picker.get_color())
    glUniform1f(shininess_loc, shininess_slider.get_value())
    glUniform4fv(light_pos_loc, 1, light_pos)
    glUniform3fv(material_color_loc, 1, (0.0, 0.0, 0.0))
    glUniform1f(ambient_intensity_loc, ambient_intensity)
    glUniform1f(K_s_loc, K_s_slider.get_value())

    glBindVertexArray(vao1)
    glDrawArrays(GL_TRIANGLES, 0, obj1.n_vertices)  # Draw the triangle

    glDeleteProgram(shader)

    # for the dragon3
    shader = shaderLoader.compile_shader("shaders/vert.glsl", "shaders/frag.glsl")
    glUseProgram(shader)

    model_matrix3 = pyrr.matrix44.multiply(translation_matrix3, rotation_mat_x)
    model_matrix3 = pyrr.matrix44.multiply(model_matrix3, rotation_mat_y)
    model_matrix3 = pyrr.matrix44.multiply(model_matrix3, scaling_matrix)

    glUniformMatrix4fv(model_mat_location1, 1, GL_FALSE, model_matrix3)
    glUniformMatrix4fv(view_mat_location1, 1, GL_FALSE, view_matrix)
    glUniformMatrix4fv(projection_mat_location1, 1, GL_FALSE, projection_matrix)

    glUniform1f(ambient_intensity_loc, ambient_intensity)
    glUniform1f(K_s_loc, K_s_slider.get_value())
    glUniform3fv(eye_pos_loc, 1, eye)
    glUniform3fv(specular_color_loc, 1, specular_color_picker.get_color())
    glUniform1f(shininess_loc, shininess_slider.get_value())
    glUniform4fv(light_pos_loc, 1, light_pos)
    glUniform3fv(material_color_loc, 1, material_color_picker.get_color())

    glBindVertexArray(vao1)
    glDrawArrays(GL_TRIANGLES, 0, obj1.n_vertices)  # Draw the triangle

    glDeleteProgram(shader)

    # Refresh the display to show what's been drawn
    pg.display.flip()

# Cleanup
glDeleteVertexArrays(1, [vao1])
glDeleteBuffers(1, [vbo1])

glDeleteProgram(shader)

pg.quit()  # Close the graphics window
quit()  # Exit the program
