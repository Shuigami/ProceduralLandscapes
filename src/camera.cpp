#include "camera.h"
#include "map.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Vector3 cameraPos(0.0f, 50.0f, 0.0f);
Vector3 cameraFront(0.0f, 0.0f, -1.0f);
Vector3 cameraUp(0.0f, 1.0f, 0.0f);

float cameraSpeed = 50.0f;

float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 800.0f / 2.0f;
float lastY = 600.0f / 2.0f;
bool firstMouse = true;

void keyboardCallback(GLFWwindow* window, float deltaTime, Map& map) {
    float frameSpeed = cameraSpeed * deltaTime;
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos = cameraPos + cameraFront * frameSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos = cameraPos - cameraFront * frameSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos = cameraPos - cameraFront.cross(cameraUp).normalized() * frameSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos = cameraPos + cameraFront.cross(cameraUp).normalized() * frameSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cameraPos = cameraPos + cameraUp * frameSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        cameraPos = cameraPos - cameraUp * frameSpeed;

    static bool plusKeyPressed = false;
    static bool minusKeyPressed = false;
    
    if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
        if (!plusKeyPressed) {
            int currentDistance = map.getRenderDistance();
            if (currentDistance < 10) {
                map.setRenderDistance(currentDistance + 1);
                std::cout << "Render distance increased to: " << map.getRenderDistance() << std::endl;
            }
            plusKeyPressed = true;
        }
    } else {
        plusKeyPressed = false;
    }
    
    if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
        if (!minusKeyPressed) {
            int currentDistance = map.getRenderDistance();
            if (currentDistance > 1) {
                map.setRenderDistance(currentDistance - 1);
                std::cout << "Render distance decreased to: " << map.getRenderDistance() << std::endl;
            }
            minusKeyPressed = true;
        }
    } else {
        minusKeyPressed = false;
    }
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float sensitivity = 0.07f;
    float xoffset = (xpos - lastX) * sensitivity;
    float yoffset = (lastY - ypos) * sensitivity;
    lastX = xpos;
    lastY = ypos;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    Vector3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = front.normalized();
}
