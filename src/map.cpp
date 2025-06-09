#include "map.h"
#include <cmath>
#include <vector>
#include <algorithm>
#include <iostream>

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
    
    int chunkX = x / chunkSize;
    int chunkY = y / chunkSize;

    chunkCoords.push_back(chunkX);
    chunkCoords.push_back(chunkY);
    
    return chunkCoords;
}

std::vector<Object> Map::generateObjects(int x, int y) {
    std::vector<int> chunkCoords = getChunkCoordinates(x, y);
    int chunkX = chunkCoords[0];
    int chunkY = chunkCoords[1];

    // std::vector<std::vector<int>> chunkObjects;
    // for (int i = chunkX - nbChunks; i <= chunkX + nbChunks; ++i) {
    //     for (int j = chunkY - nbChunks; j <= chunkY + nbChunks; ++j) {
    //         std::vector<int> objectCoords = {i, j};
    //         chunkObjects.push_back(objectCoords);
    //     }
    // }

    // for (auto& object : objects) {
    //     std::vector<GLfloat> pos = object.getPosition();
    //     int objectX = static_cast<int>(std::floor(pos[0])) / chunkSize;
    //     int objectY = static_cast<int>(std::floor(pos[2])) / chunkSize;

    //     if (objectX > chunkX + nbChunks || objectX < chunkX - nbChunks ||
    //         objectY > chunkY + nbChunks || objectY < chunkY - nbChunks) {
    //         objects.erase(std::remove(objects.begin(), objects.end(), object), objects.end());
    //         continue;
    //     }

    //     for (auto chunk : chunkObjects) {
    //         if (objectX == chunk[0] && objectY == chunk[1]) {
    //             chunkObjects.erase(std::remove(chunkObjects.begin(), chunkObjects.end(), chunk), chunkObjects.end());
    //             break;
    //         }
    //     }
    // }

    // for (const auto& chunk : chunkObjects) {
    //     int chunkX = chunk[0];
    //     int chunkY = chunk[1];

    //     Object newObject = Object::makeCube();
    //     newObject.move(chunkX * chunkSize, 0.0f, chunkY * chunkSize);
        
    //     objects.push_back(newObject);
    // }

    objects.clear();

    for (int i = chunkX - nbChunks; i <= chunkX + nbChunks; ++i) {
        for (int j = chunkY - nbChunks; j <= chunkY + nbChunks; ++j) {
            Object newObject = Object::makeCube();
            newObject.scale(chunkSize, 1.0f, chunkSize);
            newObject.move(i * chunkSize, 0.0f, j * chunkSize);
            assignColorToObject(newObject, getOctaveNoise(i + 10000, j + 10000));
            objects.push_back(newObject);
        }
    }

    return objects;
}

void Map::assignColorToObject(Object& object, float value) {
    float colorValue = (value + 1.0f) * 0.5f;
    colorValue = std::max(0.0f, std::min(1.0f, colorValue));

    int textureIndex = static_cast<int>(colorValue * textures.size());
    textureIndex = std::max(0, std::min(textureIndex, static_cast<int>(textures.size()) - 1));

    if (!textures.empty()) {
        object.setTexture(textures[textureIndex]);
    }
}