#include "settings.h"

#include <iostream>
#include <vector>
#include <cmath>

#include "matrix.h"
#include "object.h"
#include "program.h"
#include "camera.h"

const Vector3 SUN_DIRECTION_WORLD(-0.3f, 1.0f, -0.5f);
const Vector3 SUN_COLOR(1.0f, 0.95f, 0.8f);
const Vector3 AMBIENT_COLOR(0.2f, 0.2f, 0.25f);

GLuint program;
std::vector<GLfloat> vertices;
std::vector<GLfloat> normals;
std::vector<GLfloat> texCoords;
GLuint VAO = 0;
GLuint VBO = 0;
GLuint normalsVBO = 0;
GLuint texCoordsVBO = 0;
GLFWwindow *window;

bool init_glfw() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1024, 1024, "Test OpenGL - POGL", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    return true;
}

bool init_glew() {
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "Error initializing GLEW\n");
        return false;
    }
    return true;
}

bool init_gl() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClearDepth(1.0f);

    return true;
}

bool init_shader() {
    program = Program::make_program("shaders/vertex.shd", "shaders/fragment.shd")->get_program();
    if (program == 0) {
        std::cerr << "Failed to create shader program." << std::endl;
        return false;
    }
    glUseProgram(program);

    return true;
}

bool init_object(Object& object) {
    if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }
    if (VBO != 0) {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }
    if (normalsVBO != 0) {
        glDeleteBuffers(1, &normalsVBO);
        normalsVBO = 0;
    }
    if (texCoordsVBO != 0) {
        glDeleteBuffers(1, &texCoordsVBO);
        texCoordsVBO = 0;
    }

    vertices = object.getVertices();
    normals = object.getNormals();
    texCoords = object.getTexCoords();

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Vertex positions
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Normals
    glGenBuffers(1, &normalsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(1);

    // Texture coordinates
    glGenBuffers(1, &texCoordsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, texCoordsVBO);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float), texCoords.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return true;
}

bool init_pov() {
    Matrix4 model_view = Matrix4::lookAt(
        cameraPos, cameraPos + cameraFront, cameraUp
    );
    Matrix4 projection = Matrix4::frustum(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 100.0f);

    auto model_view_loc = glGetUniformLocation(program, "model_view");
    auto projection_loc = glGetUniformLocation(program, "projection");

    glUniformMatrix4fv(model_view_loc, 1, GL_FALSE, model_view.data);
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, projection.data);

    Vector3 worldSunDirection = SUN_DIRECTION_WORLD.normalized();
    
    GLfloat viewSunDirX = model_view.data[0] * worldSunDirection.x + model_view.data[4] * worldSunDirection.y + model_view.data[8] * worldSunDirection.z;
    GLfloat viewSunDirY = model_view.data[1] * worldSunDirection.x + model_view.data[5] * worldSunDirection.y + model_view.data[9] * worldSunDirection.z;
    GLfloat viewSunDirZ = model_view.data[2] * worldSunDirection.x + model_view.data[6] * worldSunDirection.y + model_view.data[10] * worldSunDirection.z;
    
    GLfloat sunDirection[] = {viewSunDirX, viewSunDirY, viewSunDirZ};
    GLfloat sunColor[] = {SUN_COLOR.x, SUN_COLOR.y, SUN_COLOR.z};
    GLfloat ambientLightIntensity[] = {AMBIENT_COLOR.x, AMBIENT_COLOR.y, AMBIENT_COLOR.z};

    auto sunDirection_loc = glGetUniformLocation(program, "sunDirection");
    auto sunColor_loc = glGetUniformLocation(program, "sunColor");
    auto ambientLightIntensity_loc = glGetUniformLocation(program, "ambientLightIntensity");

    glUniform3fv(sunDirection_loc, 1, sunDirection);
    glUniform3fv(sunColor_loc, 1, sunColor);
    glUniform3fv(ambientLightIntensity_loc, 1, ambientLightIntensity);

    return true;
}

void update_camera() {
    Matrix4 model_view = Matrix4::lookAt(
        cameraPos, cameraPos + cameraFront, cameraUp
    );

    auto model_view_loc = glGetUniformLocation(program, "model_view");
    glUniformMatrix4fv(model_view_loc, 1, GL_FALSE, model_view.data);

    Vector3 worldSunDirection = SUN_DIRECTION_WORLD.normalized();
    
    GLfloat viewSunDirX = model_view.data[0] * worldSunDirection.x + model_view.data[4] * worldSunDirection.y + model_view.data[8] * worldSunDirection.z;
    GLfloat viewSunDirY = model_view.data[1] * worldSunDirection.x + model_view.data[5] * worldSunDirection.y + model_view.data[9] * worldSunDirection.z;
    GLfloat viewSunDirZ = model_view.data[2] * worldSunDirection.x + model_view.data[6] * worldSunDirection.y + model_view.data[10] * worldSunDirection.z;
    
    GLfloat sunDirection[] = {viewSunDirX, viewSunDirY, viewSunDirZ};
    auto sunDirection_loc = glGetUniformLocation(program, "sunDirection");
    glUniform3fv(sunDirection_loc, 1, sunDirection);
}

void display(GLuint textureID) {
    glBindVertexArray(VAO);
    
    if (textureID != 0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);

        auto textureUniform = glGetUniformLocation(program, "objectTexture");
        glUniform1i(textureUniform, 0);
        
        auto useTextureUniform = glGetUniformLocation(program, "useTexture");
        glUniform1i(useTextureUniform, 1);
    } else {
        auto useTextureUniform = glGetUniformLocation(program, "useTexture");
        glUniform1i(useTextureUniform, 0);
        
        GLfloat color[] = {1.0f, 1.0f, 1.0f};
        auto color_loc = glGetUniformLocation(program, "color");
        glUniform3fv(color_loc, 1, color);
    }

    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);

    glBindVertexArray(0);
}

void render_object(Object& object) {
    init_object(object);
    display(object.getTexture());
}
