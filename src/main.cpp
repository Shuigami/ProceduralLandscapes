#include "settings.h"
#include "program.h"
#include "camera.h"
#include "object.h"
#include "map.h"

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <chrono>

int main(int argc, char** argv) {
    Map map(42, 0.02f, 6, 0.5f, 2.0f);
    if (map.saveNoiseAsImage("noise_basic.png", 512, 512)) {
        std::cout << "✓ Basic noise saved as 'noise_basic.png'" << std::endl;
    } else {
        std::cout << "✗ Failed to save basic noise image" << std::endl;
    }
    if (!init_glfw()) return -1;
    if (!init_glew()) return -1;
    if (!init_gl()) return -1;
    if (!init_shader()) return -1;

    Object cube = Object::makeCube();
    cube.loadTexture("textures/wood_0053_color_1k.jpg");
    cube.move(0.0f, 0.0f, -5.0f);

    Object cube2 = Object::makeCube();
    cube2.move(2.0f, 0.0f, -5.0f);

    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    auto lastTime = std::chrono::high_resolution_clock::now();
    int frameCount = 0;
    
    while (!glfwWindowShouldClose(window)) {
        keyboardCallback(window);

        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (!init_pov()) return -1;
        render_object(cube);
        render_object(cube2);

        glfwSwapBuffers(window);
        glfwPollEvents();
        
        frameCount++;
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastTime).count();
        if (elapsed >= 1) {
            std::cout << "FPS: " << frameCount / elapsed << std::endl;
            frameCount = 0;
            lastTime = currentTime;
        }
    }

    return 0;
}