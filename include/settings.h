#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

bool init_glfw();
bool init_glew();
bool init_gl();
bool init_shader();
bool init_object();
bool init_pov();
void display();
void setCurrentTexture(GLuint textureID);

extern GLFWwindow *window;