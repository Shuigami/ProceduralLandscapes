#pragma once

#include <GL/glew.h>

#include <string>
#include <vector>

class Object {
public:
    Object() = default;
    Object(const std::vector<GLfloat>& vertices, const std::vector<GLfloat>& normals)
        : vertices(vertices), normals(normals) {}
    Object(const std::vector<GLfloat>& vertices, const std::vector<GLfloat>& normals, const std::vector<GLfloat>& texCoords)
        : vertices(vertices), normals(normals), texCoords(texCoords) {}
    Object(const Object&) = default;
    Object(Object&&) = default;

    static Object makeCube();
    static Object makeCylinder();

    GLuint loadTexture(const std::string& filename);

    void setTexture(GLuint textureID);
    GLuint getTexture() const;
    std::vector<GLfloat> getVertices();
    std::vector<GLfloat> getNormals();
    std::vector<GLfloat> getTexCoords();

    void move(const GLfloat& x, const GLfloat& y, const GLfloat& z);
private:
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texCoords;
    GLuint textureID = 0;
};