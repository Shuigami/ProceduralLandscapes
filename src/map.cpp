#include "map.h"
#include <cmath>
#include <vector>
#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <utility>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

Map::Map(int seed, float scale, int octaves, float persistence, float lacunarity)
    : seed(seed), scale(scale), octaves(octaves), persistence(persistence), lacunarity(lacunarity) {
        for (int i = 0; i < 10; ++i) {
            std::string texturePath = "textures/color_" + std::to_string(i) + ".png";
            std::cout << "Loading texture: " << texturePath << std::endl;
            GLuint textureID = Object::loadTexture(texturePath);
            if (textureID != 0) {
                textures.push_back(textureID);
            } else {
                std::cerr << "Failed to load texture: " << texturePath << std::endl;
            }
        }
}


float Map::getValue(float x, float y) {
    return getOctaveNoise(x, y);
}

float Map::getOctaveNoise(float x, float y) {
    float value = 0.0f;
    float amplitude = 1.0f;
    float frequency = scale;
    float maxValue = 0.0f;
    
    for (int i = 0; i < octaves; i++) {
        value += perlinNoise(x * frequency, y * frequency) * amplitude;
        maxValue += amplitude;
        amplitude *= persistence;
        frequency *= lacunarity;
    }
    
    return value / maxValue;
}

void Map::setSeed(int newSeed) {
    seed = newSeed;
}

void Map::setScale(float newScale) {
    scale = newScale;
}

void Map::setOctaves(int newOctaves) {
    octaves = newOctaves;
}

void Map::setPersistence(float newPersistence) {
    persistence = newPersistence;
}

void Map::setLacunarity(float newLacunarity) {
    lacunarity = newLacunarity;
}

int Map::getSeed() const {
    return seed;
}

float Map::getScale() const {
    return scale;
}

int Map::getOctaves() const {
    return octaves;
}

float Map::getPersistence() const {
    return persistence;
}

float Map::getLacunarity() const {
    return lacunarity;
}

float Map::fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

float Map::lerp(float t, float a, float b) {
    return a + t * (b - a);
}

float Map::grad(int hash, float x, float y) {
    int h = hash & 15;
    float u = h < 8 ? x : y;
    float v = h < 4 ? y : (h == 12 || h == 14 ? x : 0);
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float Map::perlinNoise(float x, float y) {
    int X = static_cast<int>(std::floor(x)) & 255;
    int Y = static_cast<int>(std::floor(y)) & 255;

    x -= std::floor(x);
    y -= std::floor(y);

    float u = fade(x);
    float v = fade(y);

    int A = hash(X, Y);
    int B = hash(X + 1, Y);
    int C = hash(X, Y + 1);
    int D = hash(X + 1, Y + 1);

    float gradA = grad(A, x, y);
    float gradB = grad(B, x - 1, y);
    float gradC = grad(C, x, y - 1);
    float gradD = grad(D, x - 1, y - 1);
    
    return lerp(v, lerp(u, gradA, gradB),
                   lerp(u, gradC, gradD));
}

int Map::hash(int x, int y) {
    int h = seed + x * 374761393 + y * 668265263;
    h = (h ^ (h >> 13)) * 1274126177;
    return h ^ (h >> 16);
}


bool Map::saveNoiseAsImage(const std::string& filename, int width, int height, 
                          float offsetX, float offsetY) {
    std::vector<unsigned char> imageData(width * height * 3);
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float noiseValue = getOctaveNoise(x + offsetX, y + offsetY);
            
            noiseValue = (noiseValue + 1.0f) * 0.5f;
            
            noiseValue = std::max(0.0f, std::min(1.0f, noiseValue));
            
            unsigned char pixelValue = static_cast<unsigned char>(noiseValue * 255);
            
            int index = (y * width + x) * 3;
            imageData[index] = pixelValue;
            imageData[index + 1] = pixelValue;
            imageData[index + 2] = pixelValue;
        }
    }
    
    int result = stbi_write_png(filename.c_str(), width, height, 3, 
                               imageData.data(), width * 3);
    
    return result != 0;
}

std::vector<int> Map::getChunkCoordinates(int x, int y) {
    std::vector<int> chunkCoords;
    
    int chunkX = x;
    int chunkY = y;

    chunkCoords.push_back(chunkX);
    chunkCoords.push_back(chunkY);
    
    return chunkCoords;
}

struct PairHash {
    std::size_t operator()(const std::pair<int, int>& p) const {
        return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
    }
};

std::vector<Object> Map::generateObjects(int x, int y) {
    std::vector<int> chunkCoords = getChunkCoordinates(x, y);
    int chunkX = chunkCoords[0];
    int chunkY = chunkCoords[1];

    std::unordered_set<std::pair<int, int>, PairHash> requiredChunks;
    for (int i = chunkX - nbChunks; i <= chunkX + nbChunks; ++i) {
        for (int j = chunkY - nbChunks; j <= chunkY + nbChunks; ++j) {
            requiredChunks.insert({i, j});
        }
    }

    auto it = objects.begin();
    while (it != objects.end()) {
        std::vector<GLfloat> pos = it->getPosition();
        int objectX = static_cast<int>(std::floor(pos[0]));
        int objectY = static_cast<int>(std::floor(pos[2]));
    
        if (objectX > chunkX + nbChunks || objectX < chunkX - nbChunks ||
            objectY > chunkY + nbChunks || objectY < chunkY - nbChunks) {
            it = objects.erase(it);
        } else {
            requiredChunks.erase({objectX, objectY});
            ++it;
        }
    }

    objects.reserve(objects.size() + requiredChunks.size());
    auto multiplier = 100.0f;
    for (const auto& chunk : requiredChunks) {
        auto value = getOctaveNoise(chunk.first + 10000, chunk.second + 10000);
        Object newObject = Object::makeCube();
            
        assignColorToObject(newObject, value);

        value = (value + 1.0f) * 0.5f;
        value = std::max(0.0f, std::min(1.0f, value));
        newObject.scale(1.0f, value * multiplier, 1.0f);
        newObject.move(chunk.first * 1.0f, value * multiplier * 0.5f - 0.5f * multiplier, chunk.second * 1.0f);

        objects.push_back(newObject);
    }

    return objects;
}

void Map::assignColorToObject(Object& object, float value) {
    value *= 2.0f;
    value = std::max(-1.0f, std::min(1.0f, value));
    value = (value + 1.0f) * 0.5f;
    value = std::max(0.0f, std::min(1.0f, value));
    value = 1.0f - value;
    int textureIndex = static_cast<int>(value * textures.size());
    textureIndex = std::max(0, std::min(textureIndex, static_cast<int>(textures.size()) - 1));

    if (!textures.empty()) {
        object.setTexture(textures[textureIndex]);
    }
}

Object Map::generateTerrain(float spacing, float heightMultiplier, float offsetX, float offsetY) {
    Object terrain = Object::makeTerrain(chunkSize, chunkSize, spacing);

    std::vector<GLfloat> vertices = terrain.getVertices();
    std::vector<GLfloat> normals = terrain.getNormals();
    
    for (int i = 0; i < vertices.size(); i += 3) {
        float x = vertices[i] + offsetX;
        float z = vertices[i + 2] + offsetY;
        
        float noiseValue = getOctaveNoise(x, z);
        
        float normalizedHeight = (noiseValue + 1.0f) * 0.5f;
        normalizedHeight = std::max(0.0f, std::min(1.0f, normalizedHeight));
        
        vertices[i + 1] = normalizedHeight * heightMultiplier;
    }
    
    for (int i = 0; i < vertices.size(); i += 9) {
        if (i + 8 < vertices.size()) {
            glm::vec3 v1(vertices[i], vertices[i + 1], vertices[i + 2]);
            glm::vec3 v2(vertices[i + 3], vertices[i + 4], vertices[i + 5]);
            glm::vec3 v3(vertices[i + 6], vertices[i + 7], vertices[i + 8]);
            
            glm::vec3 edge1 = v2 - v1;
            glm::vec3 edge2 = v3 - v1;
            glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));
            
            if (normal.y < 0.0f) {
                normal = -normal;
            }
            
            for (int j = 0; j < 3; j++) {
                int normalIndex = i + j * 3;
                normals[normalIndex] = normal.x;
                normals[normalIndex + 1] = normal.y;
                normals[normalIndex + 2] = normal.z;
            }
        }
    }
    
    Object modifiedTerrain(vertices, normals, terrain.getTexCoords());
    
    float avgHeight = 0.0f;
    int heightCount = 0;
    for (int i = 1; i < vertices.size(); i += 3) {
        avgHeight += vertices[i];
        heightCount++;
    }
    if (heightCount > 0) {
        avgHeight /= heightCount;
        float normalizedAvgHeight = (avgHeight / heightMultiplier) * 2.0f - 1.0f;
        assignColorToObject(modifiedTerrain, normalizedAvgHeight);
    }
    
    return modifiedTerrain;
}

std::vector<Object> Map::generateTerrains(float spacing, float heightMultiplier, float offsetX, float offsetY) {
    std::vector<Object> terrains;
    
    for (int i = 0; i < nbChunks; ++i) {
        for (int j = 0; j < nbChunks; ++j) {
            float actualChunkSpacing = (chunkSize - 1) * spacing;
            Object terrain = generateTerrain(spacing, heightMultiplier, offsetX + i * actualChunkSpacing, offsetY + j * actualChunkSpacing);
            int hashValue = std::hash<int>()(i) ^ (std::hash<int>()(j) << 1) ^ (seed << 2);
            int textureIndex = std::abs(hashValue) % textures.size();
            terrain.setTexture(textures[textureIndex]);
            terrain.move(i * actualChunkSpacing, 0.0f, j * actualChunkSpacing);
            terrains.push_back(terrain);
        }
    }
    
    return terrains;
}