#include "camera.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Vector3 cameraPos(0.0f, 0.0f, 3.0f);
Vector3 cameraFront(0.0f, 0.0f, -1.0f);
Vector3 cameraUp(0.0f, 1.0f, 0.0f);

float cameraSpeed = 0.1f;

float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 800.0f / 2.0f;
float lastY = 600.0f / 2.0f;
bool firstMouse = true;

void keyboardCallback(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos = cameraPos + cameraFront * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos = cameraPos - cameraFront * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos = cameraPos - cameraFront.cross(cameraUp).normalized() * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos = cameraPos + cameraFront.cross(cameraUp).normalized() * cameraSpeed;
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float sensitivity = 0.1f;
    float xoffset = (xpos - lastX) * sensitivity;
    float yoffset = (lastY - ypos) * sensitivity; // inversé
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
