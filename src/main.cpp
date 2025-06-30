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
    if (!init_glfw()) return -1;
    if (!init_glew()) return -1;
    if (!init_gl()) return -1;
    if (!init_shader()) return -1;
    if (!init_pov()) return -1;

    Map map(42, 0.02f, 6, 0.5f, 2.0f, 3);

    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    auto lastTime = std::chrono::high_resolution_clock::now();
    auto lastFrameTime = std::chrono::high_resolution_clock::now();
    int frameCount = 0;
    int secondsElapsed = 0;

    printf("Renderer: %s\n", glGetString(GL_RENDERER));

    Object cube = Object::makeCube();
    cube.setTexture(Object::loadTexture("textures/color_0.png"));
    cube.move(0.0f, 0.0f, 0.0f);
    cube.scale(1.0f, 1.0f, 1.0f);

    float terrainSpacing = 2.0f;
    float terrainHeightMultiplier = 100.0f;

    while (!glfwWindowShouldClose(window)) {
        auto currentFrameTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentFrameTime - lastFrameTime).count();
        lastFrameTime = currentFrameTime;
        
        keyboardCallback(window, deltaTime, map);

        glClearColor(0.12f, 0.65f, 0.85f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        update_camera();

        std::vector<Object*> terrains = map.getVisibleTerrains(cameraPos.x, cameraPos.z, terrainSpacing, terrainHeightMultiplier);
        for (Object* terrain : terrains) {
            render_object(*terrain);
        }

        render_object(cube);

        glfwSwapBuffers(window);
        glfwPollEvents();
        
        frameCount++;
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastTime).count();
        if (elapsed >= 1) {
            std::cout << "FPS: " << frameCount / elapsed << std::endl;
            frameCount = 0;
            lastTime = currentTime;
            secondsElapsed += elapsed;
        }
    }

    return 0;
}
