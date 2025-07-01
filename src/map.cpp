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

Map::Map(int seed, float scale, int octaves, float persistence, float lacunarity, int renderDistance)
    : seed(seed), scale(scale), octaves(octaves), persistence(persistence), lacunarity(lacunarity), renderDistance(renderDistance) {
        chunkWorldSize = (chunkSize - 1) * 2.0f;
        
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
    float result = 0.0f;
    float frequency = 1.0f;
    float amplitude = 1.0f;
    float maxAmplitude = 0.0f;

    for (int i = 0; i < octaves; ++i) {
        result += perlinNoise(x * frequency * scale, y * frequency * scale) * amplitude;
        maxAmplitude += amplitude;
        amplitude *= persistence;
        frequency *= lacunarity;
    }

    if (maxAmplitude == 0.0f) {
        return 0.0f;
    }

    result /= maxAmplitude;

    return result;
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

void Map::setRenderDistance(int distance) {
    renderDistance = distance;
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

int Map::getRenderDistance() const {
    return renderDistance;
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
    for (int i = chunkX - renderDistance; i <= chunkX + renderDistance; ++i) {
        for (int j = chunkY - renderDistance; j <= chunkY + renderDistance; ++j) {
            requiredChunks.insert({i, j});
        }
    }

    auto it = objects.begin();
    while (it != objects.end()) {
        std::vector<GLfloat> pos = it->getPosition();
        int objectX = static_cast<int>(std::floor(pos[0]));
        int objectY = static_cast<int>(std::floor(pos[2]));
    
        if (objectX > chunkX + renderDistance || objectX < chunkX - renderDistance ||
            objectY > chunkY + renderDistance || objectY < chunkY - renderDistance) {
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

        normalizedHeight = std::pow(normalizedHeight, 6.0f) * 2000.0f;
        
        vertices[i + 1] = normalizedHeight;
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
    float hashValue = static_cast<float>(hash(static_cast<int>(offsetX), static_cast<int>(offsetY)));
    hashValue = std::abs(hashValue);
    int textureIndex = static_cast<int>(hashValue) % textures.size();
    if (!textures.empty()) {
        modifiedTerrain.setTexture(textures[textureIndex]);
    }
    
    return modifiedTerrain;
}

ChunkCoord Map::worldToChunkCoord(float worldX, float worldZ) {
    int chunkX = static_cast<int>(std::floor(worldX / chunkWorldSize));
    int chunkZ = static_cast<int>(std::floor(worldZ / chunkWorldSize));
    return {chunkX, chunkZ};
}

Object Map::generateTerrainChunk(ChunkCoord coord, float spacing, float heightMultiplier) {
    float offsetX = coord.x * chunkWorldSize;
    float offsetZ = coord.z * chunkWorldSize;
    
    Object terrain = generateTerrain(spacing, heightMultiplier, offsetX + 1000.0f, offsetZ + 1000.0f);
    terrain.move(offsetX, 0.0f, offsetZ);
    
    return terrain;
}

void Map::updateChunks(float cameraX, float cameraZ, float spacing, float heightMultiplier) {
    chunkWorldSize = (chunkSize - 1) * spacing;
    ChunkCoord cameraChunk = worldToChunkCoord(cameraX, cameraZ);
    
    bool cameraMovedToNewChunk = (cameraChunk.x != lastCameraChunk.x || cameraChunk.z != lastCameraChunk.z);
    bool renderDistanceChanged = (renderDistance != lastRenderDistance);
    
    if (!cameraMovedToNewChunk && !renderDistanceChanged) {
        return;
    }
    
    if (renderDistanceChanged) {
        std::cout << "Render distance changed from " << lastRenderDistance << " to " << renderDistance << std::endl;
        lastRenderDistance = renderDistance;
    }
    
    lastCameraChunk = cameraChunk;
    
    removeDistantChunks(cameraChunk);
    
    for (int x = cameraChunk.x - renderDistance; x <= cameraChunk.x + renderDistance; x++) {
        for (int z = cameraChunk.z - renderDistance; z <= cameraChunk.z + renderDistance; z++) {
            ChunkCoord coord = {x, z};
            
            if (terrainChunks.find(coord) == terrainChunks.end()) {
                std::cout << "Generating chunk (" << x << ", " << z << ")" << std::endl;
                terrainChunks[coord] = generateTerrainChunk(coord, spacing, heightMultiplier);
            }
        }
    }
}

void Map::removeDistantChunks(ChunkCoord centerChunk) {
    auto it = terrainChunks.begin();
    while (it != terrainChunks.end()) {
        ChunkCoord coord = it->first;
        int distanceX = std::abs(coord.x - centerChunk.x);
        int distanceZ = std::abs(coord.z - centerChunk.z);
        
        if (distanceX > renderDistance + 1 || distanceZ > renderDistance + 1) {
            std::cout << "Removing chunk (" << coord.x << ", " << coord.z << ")" << std::endl;
            it = terrainChunks.erase(it);
        } else {
            ++it;
        }
    }
}

std::vector<Object*> Map::getVisibleTerrains(float cameraX, float cameraZ, float spacing, float heightMultiplier) {
    updateChunks(cameraX, cameraZ, spacing, heightMultiplier);
    
    std::vector<Object*> visibleTerrains;
    ChunkCoord cameraChunk = worldToChunkCoord(cameraX, cameraZ);
    
    for (int x = cameraChunk.x - renderDistance; x <= cameraChunk.x + renderDistance; x++) {
        for (int z = cameraChunk.z - renderDistance; z <= cameraChunk.z + renderDistance; z++) {
            ChunkCoord coord = {x, z};
            auto it = terrainChunks.find(coord);
            if (it != terrainChunks.end()) {
                visibleTerrains.push_back(&it->second);
            }
        }
    }
    
    return visibleTerrains;
}