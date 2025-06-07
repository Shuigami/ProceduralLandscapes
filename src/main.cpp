#include "settings.h"
#include "program.h"

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <chrono>

int main(int argc, char** argv) {
    if (!init_glfw()) return -1;
    if (!init_glew()) return -1;
    if (!init_gl()) return -1;
    if (!init_shader()) return -1;
    if (!init_object()) return -1;

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        if (!init_pov()) return -1;
        display();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}