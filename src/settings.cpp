#include "settings.h"

#include <iostream>
#include <vector>
#include <cmath>

#include "matrix.h"
#include "object.h"
#include "program.h"
#include "camera.h"

GLuint program;
std::vector<GLfloat> vertices;
std::vector<GLfloat> normals;
std::vector<GLfloat> texCoords;
GLuint VAO;
GLuint VBO;
GLuint normalsVBO;
GLuint texCoordsVBO;
GLuint currentTexture = 0;
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
    program = Program::make_program("/home/shui/Work/ING2/POGL/pogl-project/shaders/vertex.shd", "/home/shui/Work/ING2/POGL/pogl-project/shaders/fragment.shd")->get_program();
    if (program == 0) {
        std::cerr << "Failed to create shader program." << std::endl;
        return false;
    }
    glUseProgram(program);

    return true;
}

bool init_object(Object& object) {
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

    glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, model_view.data);
    glUniformMatrix4fv(projection_loc, 1, GL_TRUE, projection.data);

    GLfloat lightPos[] = {-5.0f, 5.0f, 5.0f};
    GLfloat lightColor[] = {1.0f, 1.0f, 1.0f};
    GLfloat ambientLightIntensity[] = {0.1f, 0.1f, 0.1f};

    auto lightPos_loc = glGetUniformLocation(program, "lightPos");
    auto lightColor_loc = glGetUniformLocation(program, "lightColor");
    auto ambientLightIntensity_loc = glGetUniformLocation(program, "ambientLightIntensity");

    glUniform3fv(lightPos_loc, 1, lightPos);
    glUniform3fv(lightColor_loc, 1, lightColor);
    glUniform3fv(ambientLightIntensity_loc, 1, ambientLightIntensity);

    return true;
}

void display(GLuint textureID) {
    glBindVertexArray(VAO);
    
    // Bind texture if available
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

void setCurrentTexture(GLuint textureID) {
    currentTexture = textureID;
}