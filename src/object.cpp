#include "object.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>

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

Object Object::makeTerrain(int width, int height, float spacing) {
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texCoords;
    
    // Generate vertices for a grid mesh
    // Create triangles for each quad in the grid
    for (int z = 0; z < height - 1; z++) {
        for (int x = 0; x < width - 1; x++) {
            // Calculate the four corners of the current quad
            float x0 = x * spacing;
            float x1 = (x + 1) * spacing;
            float z0 = z * spacing;
            float z1 = (z + 1) * spacing;
            
            // Texture coordinates for the quad corners
            float u0 = (float)x / (width - 1);
            float u1 = (float)(x + 1) / (width - 1);
            float v0 = (float)z / (height - 1);
            float v1 = (float)(z + 1) / (height - 1);
            
            // First triangle (bottom-left, top-left, bottom-right) - CCW
            // Bottom-left
            vertices.push_back(x0);
            vertices.push_back(0.0f);
            vertices.push_back(z0);
            texCoords.push_back(u0);
            texCoords.push_back(v0);
            
            // Top-left
            vertices.push_back(x0);
            vertices.push_back(0.0f);
            vertices.push_back(z1);
            texCoords.push_back(u0);
            texCoords.push_back(v1);
            
            // Bottom-right
            vertices.push_back(x1);
            vertices.push_back(0.0f);
            vertices.push_back(z0);
            texCoords.push_back(u1);
            texCoords.push_back(v0);
            
            // Second triangle (bottom-right, top-left, top-right) - CCW
            // Bottom-right
            vertices.push_back(x1);
            vertices.push_back(0.0f);
            vertices.push_back(z0);
            texCoords.push_back(u1);
            texCoords.push_back(v0);
            
            // Top-left
            vertices.push_back(x0);
            vertices.push_back(0.0f);
            vertices.push_back(z1);
            texCoords.push_back(u0);
            texCoords.push_back(v1);
            
            // Top-right
            vertices.push_back(x1);
            vertices.push_back(0.0f);
            vertices.push_back(z1);
            texCoords.push_back(u1);
            texCoords.push_back(v1);
            
            // Add normals (pointing up, will be recalculated later)
            for (int i = 0; i < 6; i++) {
                normals.push_back(0.0f);
                normals.push_back(1.0f);
                normals.push_back(0.0f);
            }
        }
    }
    
    return Object(vertices, normals, texCoords);
}

std::vector<Object> Object::makeTree(float x, float y, float z) {
    std::vector<Object> trees;
    
    Object trunk = Object::loadFromObj("models/wood.obj", "textures/wood.png");
    trunk.move(x, y, z);
    
    Object leaves = Object::loadFromObj("models/leaf.obj", "textures/leaf.png");
    leaves.move(x, y, z);
    
    trees.push_back(trunk);
    trees.push_back(leaves);
    
    return trees;
}

std::string Object::parseMTLForTexture(const std::string& mtlFilename) {
    std::ifstream file(mtlFilename);
    if (!file.is_open()) {
        // Don't print error - just return empty string
        return "";
    }
    
    std::string line;
    std::string basePath = mtlFilename.substr(0, mtlFilename.find_last_of("/\\") + 1);
    
    while (std::getline(file, line)) {
        // Trim whitespace
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;
        
        // Look for texture map directives (in order of preference)
        if (prefix == "map_Kd" || prefix == "map_Ka" || prefix == "map_Ks" || 
            prefix == "map_Bump" || prefix == "map_bump" || prefix == "bump") {
            std::string textureFile;
            iss >> textureFile;
            
            if (!textureFile.empty()) {
                // If the texture path is relative, prepend the MTL file's directory
                if (textureFile.find("/") == std::string::npos && 
                    textureFile.find("\\") == std::string::npos) {
                    textureFile = basePath + textureFile;
                }
                
                file.close();
                return textureFile;
            }
        }
    }
    
    file.close();
    return ""; // No texture found
}

GLuint Object::loadTexture(const std::string& filename) {
    // Check file extension to determine how to handle the file
    std::string extension = filename.substr(filename.find_last_of(".") + 1);
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
    
    std::string textureFilename = filename;
    
    // If it's an MTL file, parse it to find texture references
    if (extension == "mtl") {
        textureFilename = parseMTLForTexture(filename);
        if (textureFilename.empty()) {
            // No texture found in MTL file - this is not necessarily an error
            // Just return 0 to indicate no texture should be loaded
            return 0;
        }
    }
    
    // Load the texture file (PNG, JPG, etc.)
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(textureFilename.c_str(), &width, &height, &nrChannels, 0);
    
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
        std::cerr << "Failed to load texture: " << textureFilename << std::endl;
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

void Object::rotate(const GLfloat& angle, const std::vector<GLfloat>& axis) {
    GLfloat rad = angle * M_PI / 180.0f;
    GLfloat c = cos(rad);
    GLfloat s = sin(rad);
    GLfloat x = axis[0];
    GLfloat y = axis[1];
    GLfloat z = axis[2];
    
    for (size_t i = 0; i < vertices.size(); i += 3) {
        GLfloat vx = vertices[i] - this->x;
        GLfloat vy = vertices[i + 1] - this->y;
        GLfloat vz = vertices[i + 2] - this->z;

        // Rotate around the specified axis
        vertices[i] = (c + (1 - c) * x * x) * vx + ((1 - c) * x * y - s * z) * vy + ((1 - c) * x * z + s * y) * vz + this->x;
        vertices[i + 1] = ((1 - c) * y * x + s * z) * vx + (c + (1 - c) * y * y) * vy + ((1 - c) * y * z - s * x) * vz + this->y;
        vertices[i + 2] = ((1 - c) * z * x - s * y) * vx + ((1 - c) * z * y + s * x) * vy + (c + (1 - c) * z * z) * vz + this->z;
    }
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

Object Object::loadFromObj(const std::string& filename, const std::string& textureFile) {
    std::vector<GLfloat> temp_vertices;
    std::vector<GLfloat> temp_normals;
    std::vector<GLfloat> temp_texCoords;
    
    std::vector<GLfloat> final_vertices;
    std::vector<GLfloat> final_normals;
    std::vector<GLfloat> final_texCoords;
    
    std::string mtlFile = "";
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open OBJ file: " << filename << std::endl;
        return Object();
    }
    
    std::string line;
    while (std::getline(file, line)) {
        // Trim whitespace
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;
        
        if (prefix == "mtllib") {
            // Material library reference
            iss >> mtlFile;
            // Prepend the OBJ file's directory to the MTL file path
            std::string basePath = filename.substr(0, filename.find_last_of("/\\") + 1);
            mtlFile = basePath + mtlFile;
        }
        else if (prefix == "v") {
            // Vertex position
            GLfloat x, y, z;
            iss >> x >> y >> z;
            temp_vertices.push_back(x);
            temp_vertices.push_back(y);
            temp_vertices.push_back(z);
        }
        else if (prefix == "vn") {
            // Vertex normal
            GLfloat x, y, z;
            iss >> x >> y >> z;
            temp_normals.push_back(x);
            temp_normals.push_back(y);
            temp_normals.push_back(z);
        }
        else if (prefix == "vt") {
            // Texture coordinate
            GLfloat u, v;
            iss >> u >> v;
            temp_texCoords.push_back(u);
            temp_texCoords.push_back(v);
        }
        else if (prefix == "f") {
            // Face definition - handle triangles and quads
            std::vector<std::string> faceVertices;
            std::string vertex;
            
            // Read all vertices in the face
            while (iss >> vertex) {
                faceVertices.push_back(vertex);
            }
            
            // Skip if not enough vertices for a triangle
            if (faceVertices.size() < 3) {
                continue;
            }
            
            // Parse each vertex (format: v/vt/vn or v//vn or v/vt or v)
            auto parseVertex = [&](const std::string& vertexStr) {
                std::istringstream viss(vertexStr);
                std::string part;
                std::vector<int> indices(3, 0); // Initialize with 3 zeros
                int index = 0;
                
                while (std::getline(viss, part, '/') && index < 3) {
                    if (!part.empty()) {
                        try {
                            indices[index] = std::stoi(part);
                        } catch (const std::exception&) {
                            indices[index] = 0;
                        }
                    }
                    index++;
                }
                
                return indices;
            };
            
            // Triangulate the face (for quads and higher)
            for (size_t i = 1; i < faceVertices.size() - 1; i++) {
                auto v1_indices = parseVertex(faceVertices[0]);
                auto v2_indices = parseVertex(faceVertices[i]);
                auto v3_indices = parseVertex(faceVertices[i + 1]);
                
                // Add vertices (OBJ indices are 1-based, convert to 0-based)
                for (auto indices : {v1_indices, v2_indices, v3_indices}) {
                    int v_idx = indices[0] - 1;
                    int vt_idx = indices[1] - 1;
                    int vn_idx = indices[2] - 1;
                    
                    // Add vertex position
                    if (v_idx >= 0 && (size_t)v_idx < temp_vertices.size() / 3) {
                        final_vertices.push_back(temp_vertices[v_idx * 3]);
                        final_vertices.push_back(temp_vertices[v_idx * 3 + 1]);
                        final_vertices.push_back(temp_vertices[v_idx * 3 + 2]);
                    } else {
                        // Invalid vertex index - use default
                        final_vertices.push_back(0.0f);
                        final_vertices.push_back(0.0f);
                        final_vertices.push_back(0.0f);
                    }
                    
                    // Add texture coordinate
                    if (vt_idx >= 0 && (size_t)vt_idx < temp_texCoords.size() / 2) {
                        final_texCoords.push_back(temp_texCoords[vt_idx * 2]);
                        final_texCoords.push_back(temp_texCoords[vt_idx * 2 + 1]);
                    } else {
                        // Default texture coordinates if not provided
                        final_texCoords.push_back(0.0f);
                        final_texCoords.push_back(0.0f);
                    }
                    
                    // Add vertex normal
                    if (vn_idx >= 0 && (size_t)vn_idx < temp_normals.size() / 3) {
                        final_normals.push_back(temp_normals[vn_idx * 3]);
                        final_normals.push_back(temp_normals[vn_idx * 3 + 1]);
                        final_normals.push_back(temp_normals[vn_idx * 3 + 2]);
                    } else {
                        // Default normal if not provided (pointing up)
                        final_normals.push_back(0.0f);
                        final_normals.push_back(1.0f);
                        final_normals.push_back(0.0f);
                    }
                }
            }
        }
    }
    
    file.close();
    
    Object obj(final_vertices, final_normals, final_texCoords);
    
    // Try to load texture from provided textureFile parameter first
    if (!textureFile.empty()) {
        GLuint textureID = loadTexture(textureFile);
        if (textureID != 0) {
            obj.setTexture(textureID);
        }
    }
    // If no texture was loaded and we found an MTL file, try to load texture from it
    else if (!mtlFile.empty() && obj.getTexture() == 0) {
        // Check if MTL file exists, if not, try in textures directory
        std::ifstream mtlCheck(mtlFile);
        if (!mtlCheck.is_open()) {
            // Try in textures directory
            std::string mtlBasename = mtlFile.substr(mtlFile.find_last_of("/\\") + 1);
            std::string alternativeMtlPath = "textures/" + mtlBasename;
            mtlFile = alternativeMtlPath;
        }
        mtlCheck.close();
        
        GLuint textureID = loadTexture(mtlFile);
        if (textureID != 0) {
            obj.setTexture(textureID);
        }
    }
    
    return obj;
}

bool Object::operator==(const Object& other) const {
    return this->x == other.x && this->y == other.y && this->z == other.z && this->vertices == other.vertices && this->normals == other.normals && this->texCoords == other.texCoords && this->textureID == other.textureID;
}