#pragma once

#include <GL/glew.h>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp> // For glm::ortho
#include <glm.hpp>
#include <string>
#include "LoadShader.hpp"
#include <tmxlite/Map.hpp>
#include "Camera.hpp"
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Layer.hpp>
#include <vector>
#include <algorithm> 

// tilemap consists of a tilemap and a texture atlas
class Tilemap {

public:
    //constructor
    Tilemap(
        const char* tileMapPath,
        const char* textureAtlasPath,
        const char* vertexShaderPath,
        const char* fragmentShaderPath,
        int tileWidth,
        int tileHeight,
        int tileMapWidth,
        int tileMapHeight
    );
    
    void init();
    GLuint loadTexture(const std::string& filename);

    tmx::Map getTilemap();
    void render(Camera camera);
    void renderTilemapTile(Camera camera, int tileID, float i, float j);


    void LoadTilemap(std::string tileMapPath);
    void LoadAtlas(std::string atlasPath);
    void setShaderProgram(GLuint programID);

     
private:

    // buffers
    GLuint tileVAO, tileVBO, tileTexVBO;
    
    // tilemap and texture atlas data
    tmx::Map m_tilemap;
    GLuint m_textureAtlas;
    GLuint m_shaderId;

    // tables for asset mapping
    std::unordered_map<int, int> m_atlasTable;
    std::unordered_map<int, glm::vec4> m_tileUVs;
    std::vector<int> m_tileIDs;

    // positions and texture coordinates
    std::vector<glm::vec2> m_tilemapCellPositions;
    std::vector<glm::vec2> m_tilemapTexCoords;

    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;

    // dimensional data
    int m_tileWidth;
    int m_tileHeight;
    int m_tileMapWidth;
    int m_tileMapHeight;

};