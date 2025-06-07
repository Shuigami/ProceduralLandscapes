#include "settings.h"

#include <iostream>
#include <vector>
#include <cmath>

#include "matrix.h"
#include "program.h"
#include "camera.h"

GLuint program;
std::vector<GLfloat> vertices;
std::vector<GLfloat> normals;
GLuint VAO;
GLuint VBO;
GLuint normalsVBO;
GLFWwindow *window;

std::vector<std::vector<GLfloat>> createCube() {
    std::vector vertices = {
        // Front face
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        // Back face
        -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,

        // Left face
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,

        // Right face
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,

        // Top face
        -0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,

        // Bottom face
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f
    };

    std::vector<GLfloat> normals = {
        // Front face normals
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        // Back face normals
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,

        // Left face normals
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,

        // Right face normals
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        // Top face normals
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        // Bottom face normals
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f
    };

    return {vertices, normals};
}

std::vector<std::vector<GLfloat>> createCylinder() {
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;

    const int segments = 36;
    const float radius = 0.5f;
    const float height = 1.0f;

    // Top circle triangles
    for (int i = 0; i < segments; ++i) {
        float angle1 = 2.0f * M_PI * i / segments;
        float angle2 = 2.0f * M_PI * (i + 1) / segments;
        
        vertices.push_back(0.0f);
        vertices.push_back(height / 2.0f);
        vertices.push_back(0.0f);
        normals.push_back(0.0f);
        normals.push_back(1.0f);
        normals.push_back(0.0f);
        
        vertices.push_back(radius * cos(angle1));
        vertices.push_back(height / 2.0f);
        vertices.push_back(radius * sin(angle1));
        normals.push_back(0.0f);
        normals.push_back(1.0f);
        normals.push_back(0.0f);
        
        vertices.push_back(radius * cos(angle2));
        vertices.push_back(height / 2.0f);
        vertices.push_back(radius * sin(angle2));
        normals.push_back(0.0f);
        normals.push_back(1.0f);
        normals.push_back(0.0f);

    }

    // Bottom circle triangles
    for (int i = 0; i < segments; ++i) {
        float angle1 = 2.0f * M_PI * i / segments;
        float angle2 = 2.0f * M_PI * (i + 1) / segments;
        
        vertices.push_back(0.0f);
        vertices.push_back(-height / 2.0f);
        vertices.push_back(0.0f);
        normals.push_back(0.0f);
        normals.push_back(-1.0f);
        normals.push_back(0.0f);
        
        vertices.push_back(radius * cos(angle2));
        vertices.push_back(-height / 2.0f);
        vertices.push_back(radius * sin(angle2));
        normals.push_back(0.0f);
        normals.push_back(-1.0f);
        normals.push_back(0.0f);
        
        vertices.push_back(radius * cos(angle1));
        vertices.push_back(-height / 2.0f);
        vertices.push_back(radius * sin(angle1));
        normals.push_back(0.0f);
        normals.push_back(-1.0f);
        normals.push_back(0.0f);
    }

    // Side faces
    for (int i = 0; i < segments; ++i) {
        float angle1 = 2.0f * M_PI * i / segments;
        float angle2 = 2.0f * M_PI * (i + 1) / segments;

        // First triangle
        vertices.push_back(radius * cos(angle1));
        vertices.push_back(height / 2.0f);
        vertices.push_back(radius * sin(angle1));
        normals.push_back(cos(angle1));
        normals.push_back(0.0f);
        normals.push_back(sin(angle1));

        vertices.push_back(radius * cos(angle2));
        vertices.push_back(height / 2.0f);
        vertices.push_back(radius * sin(angle2));
        normals.push_back(cos(angle2));
        normals.push_back(0.0f);
        normals.push_back(sin(angle2));

        vertices.push_back(radius * cos(angle1));
        vertices.push_back(-height / 2.0f);
        vertices.push_back(radius * sin(angle1));
        normals.push_back(cos(angle1));
        normals.push_back(0.0f);
        normals.push_back(sin(angle1));

        // Second triangle
        vertices.push_back(radius * cos(angle2));
        vertices.push_back(height / 2.0f);
        vertices.push_back(radius * sin(angle2));
        normals.push_back(cos(angle2));
        normals.push_back(0.0f);
        normals.push_back(sin(angle2));

        vertices.push_back(radius * cos(angle2));
        vertices.push_back(-height / 2.0f);
        vertices.push_back(radius * sin(angle2));
        normals.push_back(cos(angle2));
        normals.push_back(0.0f);
        normals.push_back(sin(angle2));

        vertices.push_back(radius * cos(angle1));
        vertices.push_back(-height / 2.0f);
        vertices.push_back(radius * sin(angle1));
        normals.push_back(cos(angle1));
        normals.push_back(0.0f);
        normals.push_back(sin(angle1));
    }

    return {vertices, normals};
}

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

bool init_object() {
    auto data = createCube();
    // auto data = createCylinder();
    vertices = data[0];
    normals = data[1];

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &normalsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(1);

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

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glBindVertexArray(VAO);
    
    GLfloat color[] = {1.0f, 1.0f, 1.0f};
    auto color_loc = glGetUniformLocation(program, "color");
    glUniform3fv(color_loc, 1, color);

    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);

    glBindVertexArray(0);
}