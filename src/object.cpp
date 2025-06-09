#include "object.h"
#include <iostream>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Object Object::makeCube() {
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

    std::vector<GLfloat> texCoords = {
        // Front face texture coordinates
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        // Back face texture coordinates
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,

        // Left face texture coordinates
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,

        // Right face texture coordinates
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,

        // Top face texture coordinates
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        // Bottom face texture coordinates
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
    };

    return Object(vertices, normals, texCoords);
}

Object Object::makeCylinder() {
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texCoords;

    const int segments = 36;
    const float radius = 0.5f;
    const float height = 1.0f;

    // Top circle triangles
    for (int i = 0; i < segments; ++i) {
        float angle1 = 2.0f * M_PI * i / segments;
        float angle2 = 2.0f * M_PI * (i + 1) / segments;
        
        // Center point
        vertices.push_back(0.0f);
        vertices.push_back(height / 2.0f);
        vertices.push_back(0.0f);
        normals.push_back(0.0f);
        normals.push_back(1.0f);
        normals.push_back(0.0f);
        texCoords.push_back(0.5f);
        texCoords.push_back(0.5f);
        
        // First edge point
        vertices.push_back(radius * cos(angle1));
        vertices.push_back(height / 2.0f);
        vertices.push_back(radius * sin(angle1));
        normals.push_back(0.0f);
        normals.push_back(1.0f);
        normals.push_back(0.0f);
        texCoords.push_back(0.5f + 0.5f * cos(angle1));
        texCoords.push_back(0.5f + 0.5f * sin(angle1));
        
        // Second edge point
        vertices.push_back(radius * cos(angle2));
        vertices.push_back(height / 2.0f);
        vertices.push_back(radius * sin(angle2));
        normals.push_back(0.0f);
        normals.push_back(1.0f);
        normals.push_back(0.0f);
        texCoords.push_back(0.5f + 0.5f * cos(angle2));
        texCoords.push_back(0.5f + 0.5f * sin(angle2));
    }

    // Bottom circle triangles
    for (int i = 0; i < segments; ++i) {
        float angle1 = 2.0f * M_PI * i / segments;
        float angle2 = 2.0f * M_PI * (i + 1) / segments;
        
        // Center point
        vertices.push_back(0.0f);
        vertices.push_back(-height / 2.0f);
        vertices.push_back(0.0f);
        normals.push_back(0.0f);
        normals.push_back(-1.0f);
        normals.push_back(0.0f);
        texCoords.push_back(0.5f);
        texCoords.push_back(0.5f);
        
        // First edge point (reversed winding for bottom)
        vertices.push_back(radius * cos(angle2));
        vertices.push_back(-height / 2.0f);
        vertices.push_back(radius * sin(angle2));
        normals.push_back(0.0f);
        normals.push_back(-1.0f);
        normals.push_back(0.0f);
        texCoords.push_back(0.5f + 0.5f * cos(angle2));
        texCoords.push_back(0.5f + 0.5f * sin(angle2));
        
        // Second edge point
        vertices.push_back(radius * cos(angle1));
        vertices.push_back(-height / 2.0f);
        vertices.push_back(radius * sin(angle1));
        normals.push_back(0.0f);
        normals.push_back(-1.0f);
        normals.push_back(0.0f);
        texCoords.push_back(0.5f + 0.5f * cos(angle1));
        texCoords.push_back(0.5f + 0.5f * sin(angle1));
    }

    // Side faces
    for (int i = 0; i < segments; ++i) {
        float angle1 = 2.0f * M_PI * i / segments;
        float angle2 = 2.0f * M_PI * (i + 1) / segments;
        float u1 = (float)i / segments;
        float u2 = (float)(i + 1) / segments;

        // First triangle
        vertices.push_back(radius * cos(angle1));
        vertices.push_back(height / 2.0f);
        vertices.push_back(radius * sin(angle1));
        normals.push_back(cos(angle1));
        normals.push_back(0.0f);
        normals.push_back(sin(angle1));
        texCoords.push_back(u1);
        texCoords.push_back(1.0f);

        vertices.push_back(radius * cos(angle2));
        vertices.push_back(height / 2.0f);
        vertices.push_back(radius * sin(angle2));
        normals.push_back(cos(angle2));
        normals.push_back(0.0f);
        normals.push_back(sin(angle2));
        texCoords.push_back(u2);
        texCoords.push_back(1.0f);

        vertices.push_back(radius * cos(angle1));
        vertices.push_back(-height / 2.0f);
        vertices.push_back(radius * sin(angle1));
        normals.push_back(cos(angle1));
        normals.push_back(0.0f);
        normals.push_back(sin(angle1));
        texCoords.push_back(u1);
        texCoords.push_back(0.0f);

        // Second triangle
        vertices.push_back(radius * cos(angle2));
        vertices.push_back(height / 2.0f);
        vertices.push_back(radius * sin(angle2));
        normals.push_back(cos(angle2));
        normals.push_back(0.0f);
        normals.push_back(sin(angle2));
        texCoords.push_back(u2);
        texCoords.push_back(1.0f);

        vertices.push_back(radius * cos(angle2));
        vertices.push_back(-height / 2.0f);
        vertices.push_back(radius * sin(angle2));
        normals.push_back(cos(angle2));
        normals.push_back(0.0f);
        normals.push_back(sin(angle2));
        texCoords.push_back(u2);
        texCoords.push_back(0.0f);

        vertices.push_back(radius * cos(angle1));
        vertices.push_back(-height / 2.0f);
        vertices.push_back(radius * sin(angle1));
        normals.push_back(cos(angle1));
        normals.push_back(0.0f);
        normals.push_back(sin(angle1));
        texCoords.push_back(u1);
        texCoords.push_back(0.0f);
    }

    return Object(vertices, normals, texCoords);
}

GLuint Object::loadTexture(const std::string& filename) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
    
    if (data) {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        else {
            std::cerr << "Unsupported number of channels: " << nrChannels << std::endl;
            stbi_image_free(data);
            glDeleteTextures(1, &textureID);
            return 0;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        glDeleteTextures(1, &textureID);
        return 0;
    }
    
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;
}

void Object::setTexture(GLuint textureID) {
    this->textureID = textureID;
}

GLuint Object::getTexture() const {
    return textureID;
}

std::vector<GLfloat> Object::getVertices() {
    return vertices;
}

std::vector<GLfloat> Object::getNormals() {
    return normals;
}

std::vector<GLfloat> Object::getTexCoords() {
    return texCoords;
}

std::vector<GLfloat> Object::getPosition() const {
    return {x, y, z};
}

void Object::move(const GLfloat& x, const GLfloat& y, const GLfloat& z) {
    for (size_t i = 0; i < vertices.size(); i += 3) {
        vertices[i] += x;
        vertices[i + 1] += y;
        vertices[i + 2] += z;
    }

    this->x += x;
    this->y += y;
    this->z += z;
}

void Object::scale(const GLfloat& x, const GLfloat& y, const GLfloat& z) {
    for (size_t i = 0; i < vertices.size(); i += 3) {
        vertices[i] *= x;
        vertices[i + 1] *= y;
        vertices[i + 2] *= z;
    }
    
    for (size_t i = 0; i < normals.size(); i += 3) {
        normals[i] *= x;
        normals[i + 1] *= y;
        normals[i + 2] *= z;
    }
}

bool Object::operator==(const Object& other) const {
    return this->x == other.x && this->y == other.y && this->z == other.z && this->vertices == other.vertices && this->normals == other.normals && this->texCoords == other.texCoords && this->textureID == other.textureID;
}