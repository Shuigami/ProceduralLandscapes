#pragma once

#include "vector.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Map;

void keyboardCallback(GLFWwindow* window, float deltaTime, Map& map);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);

extern Vector3 cameraPos;
extern Vector3 cameraFront;
extern Vector3 cameraUp;
extern float cameraSpeed;
extern float yaw;
extern float pitch;
extern bool firstMouse;
extern float lastX;
extern float lastY;