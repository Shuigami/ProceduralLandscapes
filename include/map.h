#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <climits>
#include <functional>

#include "object.h"

struct ChunkCoord {
    int x, z;
    
    bool operator==(const ChunkCoord& other) const {
        return x == other.x && z == other.z;
    }
};

struct ChunkCoordHash {
    std::size_t operator()(const ChunkCoord& coord) const {
        return std::hash<int>()(coord.x) ^ (std::hash<int>()(coord.z) << 1);
    }
};

class Map {
    private:
        int renderDistance = 3;
        int chunkSize = 32;
        int seed;
        float scale;
        int octaves;
        float persistence;
        float lacunarity;

        std::function<float(float)> function = [](float x) { return x; };

        std::chrono::high_resolution_clock::time_point lastTime;

        std::vector<Object> objects;
        std::unordered_map<std::string, GLuint> textures;

        std::unordered_map<ChunkCoord, std::vector<Object>, ChunkCoordHash> terrainChunks;
        ChunkCoord lastCameraChunk = {INT_MAX, INT_MAX};
        int lastRenderDistance = -1;
        float chunkWorldSize;

        float fade(float t);
        float lerp(float t, float a, float b);
        float grad(int hash, float x, float y);
        float perlinNoise(float x, float y);
        
        int hash(int x, int y);
        
        ChunkCoord worldToChunkCoord(float worldX, float worldZ);
        std::vector<Object> generateTerrainChunk(ChunkCoord coord, float spacing);
        void updateChunks(float cameraX, float cameraZ, float spacing);
        void removeDistantChunks(ChunkCoord centerChunk);
        
    public:
        Map(int seed = 12345, float scale = 0.01f, int octaves = 4, 
            float persistence = 0.5f, float lacunarity = 2.0f, int renderDistance = 3);
        
        float getValue(float x, float y);
        
        float getOctaveNoise(float x, float y);
        float getNoise(float x, float y);
        
        void setSeed(int newSeed);
        void setScale(float newScale);
        void setOctaves(int newOctaves);
        void setPersistence(float newPersistence);
        void setLacunarity(float newLacunarity);
        void setRenderDistance(int distance);
        void setFunction(std::function<float(float)> func);
        
        int getSeed() const;
        float getScale() const;
        int getOctaves() const;
        float getPersistence() const;
        float getLacunarity() const;
        int getRenderDistance() const;
        std::function<float(float)> getFunction() const;
        
        bool saveNoiseAsImage(const std::string& filename, int width, int height, 
                             float offsetX = 0.0f, float offsetY = 0.0f);

        std::vector<int> getChunkCoordinates(int x, int y);
        std::vector<Object> generateObjects(int x, int y);
        
        std::vector<Object*> getVisibleTerrains(float cameraX, float cameraZ, 
                                               float spacing = 1.0f);

        std::vector<Object> generateTerrain(float spacing = 1.0f, 
                              float offsetX = 0.0f, float offsetY = 0.0f);
        void assignColorToObject(Object& object, float value);
};