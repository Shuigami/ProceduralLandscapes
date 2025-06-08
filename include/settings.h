#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "object.h"

bool init_glfw();
bool init_glew();
bool init_gl();
bool init_shader();
bool init_object(Object& object);
bool init_pov();
void display(GLuint textureID = 0);
void render_object(Object& object);
void setCurrentTexture(GLuint textureID);

extern GLFWwindow *window;