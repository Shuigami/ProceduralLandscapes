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
            std::string textureName = "color_" + std::to_string(i);
            std::string texturePath = "textures/" + textureName + ".png";
            std::cout << "Loading texture: " << texturePath << std::endl;
            GLuint textureID = Object::loadTexture(texturePath);
            if (textureID != 0) {
                textures[textureName] = textureID;
            } else {
                std::cerr << "Failed to load texture: " << texturePath << std::endl;
            }
        }

        textures["grass"] = Object::loadTexture("textures/grass.png");
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

float Map::getNoise(float x, float y) {
    float noiseValue = getOctaveNoise(x, y);
    float normalizedHeight = (noiseValue + 1.0f) * 0.5f;
    normalizedHeight = std::max(0.0f, std::min(1.0f, normalizedHeight));

    normalizedHeight = function(normalizedHeight);

    return normalizedHeight;
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

void Map::setFunction(std::function<float(float)> func) {
    function = func;
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

std::function<float(float)> Map::getFunction() const {
    return function;
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
    std::string textureName = "color_" + std::to_string(static_cast<int>(value * textures.size()));
    if (textures.find(textureName) != textures.end()) {
        object.setTexture(textures[textureName]);
    }
}

std::vector<Object> Map::generateTerrain(float spacing, float offsetX, float offsetY) {
    Object terrain = Object::makeTerrain(chunkSize, chunkSize, spacing);
    float blendHeight = 19.0f;
    std::vector<Object> terrainObjects;

    std::vector<GLfloat> vertices = terrain.getVertices();
    std::vector<GLfloat> normals = terrain.getNormals();
    std::vector<GLfloat> texCoords = terrain.getTexCoords();
    
    for (int i = 0; i < vertices.size(); i += 3) {
        float x = vertices[i] + offsetX;
        float z = vertices[i + 2] + offsetY;
        
        float noiseValue = getNoise(x, z);
        vertices[i + 1] = noiseValue;
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

            if (rand() % 100000 < 5 && vertices[i + 1] < blendHeight) {
                float treeX = vertices[i];
                float treeY = vertices[i + 1];
                float treeZ = vertices[i + 2];
                
                std::vector<Object> tree = Object::makeTree(treeX, treeY, treeZ);
                for (auto& t : tree) {
                    terrainObjects.push_back(t);
                }
            }
        }
    }

    if (!vertices.empty() && textures.find("grass") != textures.end() && textures.find("color_4") != textures.end()) {
        Object blendedTerrain(vertices, normals, texCoords);
        
        blendedTerrain.setTerrainTextures(textures["grass"], textures["color_4"], blendHeight);
        blendedTerrain.enableTerrainBlending(true);
        
        terrainObjects.push_back(blendedTerrain);
    }

    return terrainObjects;
}

ChunkCoord Map::worldToChunkCoord(float worldX, float worldZ) {
    int chunkX = static_cast<int>(std::floor(worldX / chunkWorldSize));
    int chunkZ = static_cast<int>(std::floor(worldZ / chunkWorldSize));
    return {chunkX, chunkZ};
}

std::vector<Object> Map::generateTerrainChunk(ChunkCoord coord, float spacing) {
    float offsetX = coord.x * chunkWorldSize;
    float offsetZ = coord.z * chunkWorldSize;

    std::vector<Object> terrain = generateTerrain(spacing, offsetX + 1000.0f, offsetZ + 1000.0f);
    for (auto& object : terrain) {
        object.move(offsetX, 0.0f, offsetZ);
    }

    return terrain;
}

void Map::updateChunks(float cameraX, float cameraZ, float spacing) {
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
                terrainChunks[coord] = generateTerrainChunk(coord, spacing);
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

std::vector<Object*> Map::getVisibleTerrains(float cameraX, float cameraZ, float spacing) {
    updateChunks(cameraX, cameraZ, spacing);
    
    std::vector<Object*> visibleTerrains;
    ChunkCoord cameraChunk = worldToChunkCoord(cameraX, cameraZ);
    
    for (int x = cameraChunk.x - renderDistance; x <= cameraChunk.x + renderDistance; x++) {
        for (int z = cameraChunk.z - renderDistance; z <= cameraChunk.z + renderDistance; z++) {
            ChunkCoord coord = {x, z};
            auto it = terrainChunks.find(coord);
            if (it != terrainChunks.end()) {
                for (auto& object : it->second) {
                    visibleTerrains.push_back(&object);
                }
            }
        }
    }
    
    return visibleTerrains;
}

void Map::setFogNear(float near) {
    fogNear = near;
}

void Map::setFogFar(float far) {
    fogFar = far;
}

void Map::setFogColor(float r, float g, float b) {
    fogColorR = r;
    fogColorG = g;
    fogColorB = b;
}

float Map::getFogNear() const {
    return fogNear;
}

float Map::getFogFar() const {
    return fogFar;
}

float Map::getFogColorR() const {
    return fogColorR;
}

float Map::getFogColorG() const {
    return fogColorG;
}

float Map::getFogColorB() const {
    return fogColorB;
}