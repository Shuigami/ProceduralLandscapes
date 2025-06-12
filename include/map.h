#pragma once

#include <string>
#include <vector>
#include <chrono>

#include "object.h"

class Map {
    private:
        int nbChunks = 16;
        int seed;
        float scale;
        int octaves;
        float persistence;
        float lacunarity;

        std::chrono::high_resolution_clock::time_point lastTime;

        std::vector<Object> objects;
        std::vector<GLuint> textures;

        float fade(float t);
        float lerp(float t, float a, float b);
        float grad(int hash, float x, float y);
        float perlinNoise(float x, float y);
        
        int hash(int x, int y);
        
    public:
        Map(int seed = 12345, float scale = 0.01f, int octaves = 4, 
            float persistence = 0.5f, float lacunarity = 2.0f);
        
        float getValue(float x, float y);
        
        float getOctaveNoise(float x, float y);
        
        void setSeed(int newSeed);
        void setScale(float newScale);
        void setOctaves(int newOctaves);
        void setPersistence(float newPersistence);
        void setLacunarity(float newLacunarity);
        
        int getSeed() const;
        float getScale() const;
        int getOctaves() const;
        float getPersistence() const;
        float getLacunarity() const;
        
        bool saveNoiseAsImage(const std::string& filename, int width, int height, 
                             float offsetX = 0.0f, float offsetY = 0.0f);

        std::vector<int> getChunkCoordinates(int x, int y);
        std::vector<Object> generateObjects(int x, int y);
        void assignColorToObject(Object& object, float value);
};