# Import necessary libraries
import pygame as pg
from OpenGL.GL import *
import numpy as np
import pyrr

import shaderLoaderV3
from guiV3 import SimpleGUI
import pyrr.matrix44 as m4
from objLoaderV4 import ObjLoader
import shaderLoaderV3 as shaderLoader
from utils import load_image

def load_cubemap_texture(filenames):
    # Generate a texture ID
    texture_id = glGenTextures(1)

    # Bind the texture as a cubemap
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id)

    # Define texture parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE)
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE)
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST)
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR)


    # Define the faces of the cubemap
    faces = [GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
             GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
             GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z]

    # Load and bind images to the corresponding faces
    for i in range(6):
        img_data, img_w, img_h = load_image(filenames[i], format="RGB", flip=False)
        glTexImage2D(faces[i], 0, GL_RGB, img_w, img_h, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data)

    # Generate mipmaps
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP)

    # Unbind the texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0)

    return texture_id

# Initialize pygame
pg.init()

# Set up OpenGL context version
pg.display.gl_set_attribute(pg.GL_CONTEXT_MAJOR_VERSION, 3)
pg.display.gl_set_attribute(pg.GL_CONTEXT_MINOR_VERSION, 3)

# Create a window for graphics using OpenGL
width = 640
height = 480
pg.display.set_mode((width * 2, height), pg.OPENGL | pg.DOUBLEBUF)


glEnable(GL_DEPTH_TEST)
glClearColor(0.3, 0.4, 0.5, 1.0)

# shader1 for rabbit
shaderProgram_obj = shaderLoader.ShaderProgram("shaders/vert.glsl", "shaders/frag.glsl")
glUseProgram(shaderProgram_obj.shader)

# Todo: Part 1: Read the 3D model
# Lets setup our scene geometry.
obj1 = ObjLoader("stormtrooper/stormtrooper.obj")

vertices = np.array(obj1.vertices, dtype="float32")
center = obj1.center
dia = obj1.dia

size_position = 3
size_texture = 2
size_normal = 3
size_color = 3



stride = 32
offset_position = 0
offset_normal = 20
offset_texture = size_position * 4
offset_color = (size_position + size_texture + size_normal) * 4   # offset of the color data. Color data starts after 5 floats (20 bytes) of position and texture data

# Create a Vertex Buffer Object (VBO) to store the vertex data for the obj1
vao1 = glGenVertexArrays(1)
glBindVertexArray(vao1)                 # Bind the VAO. That is, make it the active one.
vbo1 = glGenBuffers(1)                  # Generate one buffer and store its ID.
glBindBuffer(GL_ARRAY_BUFFER, vbo1)     # Bind the buffer. That is, make it the active one.
glBufferData(GL_ARRAY_BUFFER, size=vertices.nbytes,  # size of entire vertex data in bytes
             data=vertices,
             usage=GL_STATIC_DRAW)  # Upload the data to the GPU

position_loc = 0
glBindAttribLocation(shaderProgram_obj.shader, position_loc, "position")
glVertexAttribPointer(position_loc, size_position, GL_FLOAT, GL_FALSE, stride, ctypes.c_void_p(offset_position))
glEnableVertexAttribArray(position_loc)

normal_loc = 3
glBindAttribLocation(shaderProgram_obj.shader, normal_loc, "normal")
glVertexAttribPointer(normal_loc, size_normal, GL_FLOAT, GL_FALSE, stride, ctypes.c_void_p(offset_normal))
glEnableVertexAttribArray(normal_loc)

# texture attribute
texture_loc = 1
glBindAttribLocation(shaderProgram_obj.shader, texture_loc, "uv")
glVertexAttribPointer(texture_loc, size_texture, GL_FLOAT, GL_FALSE, stride, ctypes.c_void_p(offset_texture))
glEnableVertexAttribArray(texture_loc)

color_loc = 2
glBindAttribLocation(shaderProgram_obj.shader, color_loc, "color")
glVertexAttribPointer(color_loc, size_color, GL_FLOAT, GL_FALSE, stride, ctypes.c_void_p(offset_color))
glEnableVertexAttribArray(color_loc)

# hw9
# 2d texture
img_data, img_width, img_height = load_image("stormtrooper/stormtrooper.jpg", flip=True)

texture_id = glGenTextures(1)
glBindTexture(GL_TEXTURE_2D, texture_id)
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT)    # Set the texture wrapping parameters
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT)
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)    # Set texture filtering parameters
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)

# Upload the image data to the GPU
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data)

glUseProgram(shaderProgram_obj.shader)
shaderProgram_obj["cubeMapTex"] = 0   # Okay this might be confusing. Here 0 indicates texture unit 0. Note that "cubeMapTex" is a sampler variable in the fragment shader. It is not an integer.


# environment mapping
cubemap_images = ["skybox3/right.png", "skybox3/left.png",
                  "skybox3/top.png", "skybox3/bottom.png",
                  "skybox3/front.png", "skybox3/back.png"]
cubemap_id = load_cubemap_texture(cubemap_images)

shaderProgram_obj["cubeMapTex"] = 1   # Okay this might be confusing. Here 0 indicates texture unit 0. Note that "cubeMapTex" is a sampler variable in the fragment shader. It is not an integer.



#Create the 4x4 translation matrix to translate the object to the origin
x, y, z = center

translation_matrix1 = m4.create_from_translation(pyrr.Vector3([0, -y, 0]))

# Create the scaling matrix
scale_factor1 = 4.0 / obj1.dia
scaling_matrix = m4.create_from_scale(pyrr.Vector3([scale_factor1] * 3))

gui = SimpleGUI("assignment 9")
angleY_slider = gui.add_slider("camera Y angle", min_value=-180, max_value=180, initial_value=0)
angleX_slider = gui.add_slider("camera X angle", min_value=-90, max_value=90, initial_value=0)
fov_slider = gui.add_slider("fov", min_value=25, max_value=120, initial_value=100)
tex_type_radio_button = gui.add_radio_buttons("Texture type", options_dict={"Environment Mapping": 1, "2D Texture": 0, "Mix": 2}, initial_option = "2D Texture")


eye = [0,0,2]
model_mat_location1 = glGetUniformLocation(shaderProgram_obj.shader, "model_matrix")
view_mat_location1 = glGetUniformLocation(shaderProgram_obj.shader, "view_matrix")
projection_mat_location1 = glGetUniformLocation(shaderProgram_obj.shader, "projection_matrix")
eye_pos_loc = glGetUniformLocation(shaderProgram_obj.shader, "eye_pos")
cubeMapTex_loc = glGetUniformLocation(shaderProgram_obj.shader, "cubeMapTex")
tex_loc = glGetUniformLocation(shaderProgram_obj.shader, "tex")
texType_loc = glGetUniformLocation(shaderProgram_obj.shader, "texType")

# Run a loop to keep the program running
draw = True
while draw:
    for event in pg.event.get():
        if event.type == pg.QUIT:
            draw = False

    # Clear color buffer and depth buffer before drawing each frame
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)  # Clear the color buffer and depth buffer before drawing each frame using glClear()

    glUseProgram(shaderProgram_obj.shader)

    # Rotate the camera position using the rotation matrix
    rotation_mat_x = pyrr.Matrix44.from_x_rotation(np.deg2rad(angleX_slider.get_value()))
    rotation_mat_y = pyrr.Matrix44.from_y_rotation(np.deg2rad(angleY_slider.get_value()))

    projection_matrix = m4.create_perspective_projection_matrix(fov_slider.get_value(), width / height, 0.1, 10)
    view_matrix = pyrr.matrix44.create_look_at(eye, [0, 0, 0], [0, 1, 0])

    model_matrix1 = pyrr.matrix44.multiply(translation_matrix1, rotation_mat_x)
    model_matrix1 = pyrr.matrix44.multiply(model_matrix1, rotation_mat_y)
    model_matrix1 = pyrr.matrix44.multiply(model_matrix1, scaling_matrix)

    glUniformMatrix4fv(model_mat_location1, 1, GL_FALSE, model_matrix1)
    glUniformMatrix4fv(view_mat_location1, 1, GL_FALSE, view_matrix)
    glUniformMatrix4fv(projection_mat_location1, 1, GL_FALSE, projection_matrix)
    glUniform3fv(eye_pos_loc, 1, eye)
    glUniform1f(texType_loc, float(tex_type_radio_button.get_value()))

    shaderProgram_obj["tex"] = 0  # Okay this might be confusing. Here 0 indicates texture unit 0. Note that "cubeMapTex" is a sampler variable in the fragment shader. It is not an integer.
    glActiveTexture(GL_TEXTURE0)
    glBindTexture(GL_TEXTURE_2D, texture_id)

    shaderProgram_obj["cubeMapTex"] = 1  # Okay this might be confusing. Here 0 indicates texture unit 0. Note that "cubeMapTex" is a sampler variable in the fragment shader. It is not an integer.
    glActiveTexture(GL_TEXTURE1)
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_id)

    glBindVertexArray(vao1)
    glDrawArrays(GL_TRIANGLES, 0, obj1.n_vertices)  # Draw the triangle

    # Refresh the display to show what's been drawn
    pg.display.flip()

# Cleanup
glDeleteVertexArrays(1, [vao1])
glDeleteBuffers(1, [vbo1])

glDeleteProgram(shaderProgram_obj.shader)

pg.quit()  # Close the graphics window
quit()  # Exit the program


