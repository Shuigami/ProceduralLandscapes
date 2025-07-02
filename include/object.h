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
    Object& operator=(const Object&) = default;
    bool operator==(const Object& other) const;

    static Object makeCube();
    static Object makeCylinder();
    static Object makeTerrain(int width, int height, float spacing = 1.0f);
    static std::vector<Object> makeTree(float x, float y, float z);

    static GLuint loadTexture(const std::string& filename);

    void setTexture(GLuint textureID);
    void setTerrainTextures(GLuint grassTextureID, GLuint rockTextureID, float blendHeight);
    void setTerrainTextures(GLuint grassTextureID, GLuint rockTextureID, GLuint snowTextureID, float rockBlendHeight, float snowBlendHeight);
    void setBlendHeight(float height);
    void setSnowBlendHeight(float height);
    void enableTerrainBlending(bool enable);
    GLuint getTexture() const;
    GLuint getGrassTexture() const;
    GLuint getRockTexture() const;
    GLuint getSnowTexture() const;
    float getBlendHeight() const;
    float getSnowBlendHeight() const;
    bool isTerrainBlendingEnabled() const;
    std::vector<GLfloat> getVertices();
    std::vector<GLfloat> getNormals();
    std::vector<GLfloat> getTexCoords();
    std::vector<GLfloat> getPosition() const;

    void move(const GLfloat& x, const GLfloat& y, const GLfloat& z);
    void rotate(const GLfloat& angle, const std::vector<GLfloat>& axis);
    void scale(const GLfloat& x, const GLfloat& y, const GLfloat& z);

    static Object loadFromObj(const std::string& filename, const std::string& mtlFilename = "");
private:
    static std::string parseMTLForTexture(const std::string& mtlFilename);
    
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texCoords;
    GLuint textureID = 0;
    GLuint grassTextureID = 0;
    GLuint rockTextureID = 0;
    GLuint snowTextureID = 0;
    float blendHeight = 20.0f;
    float snowBlendHeight = 40.0f;
    bool useTerrainBlending = false;
    GLfloat x = 0.0f;
    GLfloat y = 0.0f;
    GLfloat z = 0.0f;
};