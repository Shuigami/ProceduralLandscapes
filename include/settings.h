#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "object.h"

class Map;

bool init_glfw();
bool init_glew();
bool init_gl();
bool init_shader();
bool init_object(Object& object);
bool init_pov();
void update_camera();
void update_fog(const Map& map);
void display(GLuint textureID = 0);
void display(const Object& object);
void render_object(Object& object);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

extern GLFWwindow *window;
extern float z_far;
extern int windowWidth;
extern int windowHeight;