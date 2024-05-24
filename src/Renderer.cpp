#include "Renderer.hpp"
#include <iostream>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Layer.hpp>
#include <algorithm> 


GLuint texture1;
GLuint texture2;
GLuint cursorTexture;

GLuint tileVAO;
GLuint tileVBO;
GLuint cursorVAO;
GLuint texCoordVBO;
GLuint tileEBO;

GLuint textureAtlasId;

GLuint tileShaderProgram;
float tileSize = 32.0;

GLuint projectionLoc;
GLuint modelLoc;
GLuint viewLoc;

tmx::Map tileMap;

float cameraTileWidth = 25.0;
float cameraTileHeight = 25.0;

float tilemapCellPositions[] = {
    // Positions       
    -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
    
     1.0f,  1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
};


float tilemapTexCoords[] = {
    0.0f, 0.1f, // Top Left (Vertex 1)
    1.0f, 0.1f, // Top Right (Vertex 2)
    0.0f, 0.0f, // Bottom Left (Vertex 3)
    
    1.0f, 0.1f, // Top Right (Vertex 4)
    0.0f, 0.0f, // Bottom Left (Vertex 5)
    1.0f, 0.0f  // Bottom Right (Vertex 6)
};

float playerTexCoords[] = {
    0.0f, 1.0f, // Top Left (Vertex 1)
    1.0f, 1.0f, // Top Right (Vertex 2)
    0.0f, 0.0f, // Bottom Left (Vertex 3)
    
    1.0f, 1.0f, // Top Right (Vertex 4)
    0.0f, 0.0f, // Bottom Left (Vertex 5)
    1.0f, 0.0f  // Bottom Right (Vertex 6)
};

float cursorTexCoords[] = {
    0.0f, 1.0f, // Top Left (Vertex 1)
    1.0f, 1.0f, // Top Right (Vertex 2)
    0.0f, 0.0f, // Bottom Left (Vertex 3)
    
    1.0f, 1.0f, // Top Right (Vertex 4)
    0.0f, 0.0f, // Bottom Left (Vertex 5)
    1.0f, 0.0f  // Bottom Right (Vertex 6)
};



std::vector<int> tileIDs;

std::unordered_map<int, int> textureAtlasTable = {
        {1478, 0},
        {1469, 1},
        {260, 2},
        {591, 3}, 
        {590, 4},
        {1471, 1}
    };

std::unordered_map<int, glm::vec4> tileUVs;


Renderer::Renderer(float sWidth, float sHeight, float tWidth, float tHeight) : 
    screenWidth(sWidth), screenHeight(sHeight), 
    tileWidth(tWidth), tileHeight(tHeight),
    camera(0.0f, 0.0f, 800.0f, 800.0f),
    testPlayer(400.0, 400.0, loadTexture("./assets/testWizard.png"))
   {
   
    // initialize glew
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
 
}

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &vao_ID);
    glDeleteBuffers(1, &vbo_ID);
}

void Renderer::init(){
    
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // create and set shader program
    tileShaderProgram = LoadShaders( "./shaders/tileVert.glsl", "./shaders/tileFrag.glsl");
    setShaderProgram(tileShaderProgram);

    //load water texture
    texture1 = loadTexture("./assets/water.png");
    texture2 = loadTexture("./assets/grass.png");
    textureAtlasId = loadTexture("./assets/SimpleTextureAtlas1.png");
    cursorTexture = loadTexture("./assets/Cursor1.png");


    
    // Generate and bind Vertex Array Object (VAO)
    glGenVertexArrays(1, &tileVAO);
    glBindVertexArray(tileVAO);

    glGenVertexArrays(1, &cursorVAO);
    // Generate and bind VBOs
    glGenBuffers(1, &tileVBO);
    glBindBuffer(GL_ARRAY_BUFFER, tileVBO);

    glGenBuffers(1, &texCoordVBO);
    glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);

    //positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)tilemapCellPositions);
    glEnableVertexAttribArray(0);

    //texture coordinates
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)tilemapTexCoords);
    glEnableVertexAttribArray(1);

    GLuint projectionLoc = glGetUniformLocation(tileShaderProgram, "projection");
    glm::mat4 projection = glm::ortho(0.0f, (float)1.0f, 0.0f, (float)1.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    GLuint viewLoc = glGetUniformLocation(tileShaderProgram, "view");
    glm::mat4 view = glm::mat4(1.0f);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    GLuint modelLoc = glGetUniformLocation(tileShaderProgram, "model");
    glm::mat4 model = glm::translate(glm::mat4(1.0f), 
    glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::mat4(1.0f), 
    glm::vec3(1.0f, 1.0f, 1.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    
}


void Renderer::loadTilemap(std::string tileMapPath, std::string textureAtlasPath) {
    textureAtlasId = loadTexture(textureAtlasPath);

    float tileSizeX = 32;
    float tileSizeY = 32;

    float atlasWidth = 32;
    float atlasHeight = 320;

    if (!tileMap.load(tileMapPath)) {
        std::cout << "Could not load the tilemap" << std::endl;
    } else {
        std::cout << "Tilemap Loaded successfully" << std::endl;
        const auto& layers = tileMap.getLayers();
        int count = 0;
        for (const auto& layer : layers) {
            if (layer->getType() == tmx::Layer::Type::Tile) {
                const tmx::TileLayer* tileLayer = dynamic_cast<const tmx::TileLayer*>(layer.get());
                if (tileLayer) {
                    const auto& tiles = tileLayer -> getTiles();
                    for (std::size_t i = 0; i < tiles.size(); ++i) {
                        const auto& tile = tiles[i];
                        
                        int tileMapID = tile.ID;
                        int atlasID = textureAtlasTable[tileMapID];
 
                        //calculate uv coordinates for each tile
                        float tileX = 0.0;
                        float tileY = (float) 32 * atlasID;
                        float u = (float) tileX / 32;
                        float v = (float) tileY / 320;
                        float u2 = 1.0f;
                        float v2 = (float) (tileY + 32) / 320;
                        tileUVs[tileMapID] = glm::vec4(u,v,u2,v2);
                        tileIDs.push_back(tileMapID);
                    }
                }
            } 
            else{
                    std::cerr << "Failed to cast layer to TileLayer" << std::endl;
            }
        }
    }
}


// render all of the tiles within the scene
// chage to render tiles based on camera positions
// camera position based on player world coordinates
// need to calculate clipping region for tiles
void Renderer::renderTilemap(){
    float numTilesX = 25.0;
    float numTilesY = 25.0;
    int tileMapWidth = 75;
    int tileMapHeight = 50;
    
    
    int startX = std::max(0, static_cast<int>(camera.tileX));
    int startY = std::max(0, static_cast<int>(camera.tileY));
    int endX = std::min(tileMapWidth, startX + 25);
    int endY = std::min(tileMapHeight, startY + 25);

    //render tile
    for(float i = startX; i < endX; i += 1.0){
        for(float j = startY; j< endY; j += 1.0){
            int tileID = tileIDs[j * tileMapWidth + i];
            
            // in pixels
            float tileX = i * tileSize;
            float tileY = j * tileSize;
            
            // Calculate the clipping region for the tile
            float clipX = std::max(0.0f, camera.tileX - i);
            float clipY = std::max(0.0f, camera.tileY - j);
            float clipWidth = std::min(tileSize, screenWidth - clipX);
            float clipHeight = std::min(tileSize, screenHeight - clipY);

            if(clipWidth == tileSize && clipHeight == tileSize){
                renderTilemapTile(tileID, i, j);
            }
            else{
                std::cout << "tile clipped" << std::endl;
                renderPartialTilemapTile(tileID, tileX, tileY, clipX, clipY, clipWidth, clipHeight);
            }
        }
    }
}

// Render a single tile from the tilemap
void Renderer::renderTilemapTile(int tileID, float i, float j){
    
    glBindTexture(GL_TEXTURE_2D, textureAtlasId);

    //update the model view matrix
    GLuint modelLoc = glGetUniformLocation(tileShaderProgram, "model");
    glm::mat4 model = glm::translate(glm::mat4(1.0f), 
    glm::vec3((i - camera.tileX) / 25.0, (j - camera.tileY) / 25.0, 1.0f)) * glm::scale(glm::mat4(1.0f), 
    glm::vec3(1.0f / 25.0, 1.0f / 25.0, 1.0f));
            
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(tileVAO);
    const auto& uv = tileUVs[tileID];
    float textureVertices[] = {
        
        uv.x, uv.y, // Top Left (Vertex 1)
        uv.z, uv.y, // Top Right (Vertex 2)
        uv.x, uv.w, // Bottom Left (Vertex 3)
                            
        uv.z, uv.y, // Top Right (Vertex 4)
        uv.x, uv.w, // Bottom Left (Vertex 5)
        uv.z, uv.w  // Bottom Right (Vertex 6)
    };

    glBindVertexArray(tileVAO);

    // bind position data
    glBindBuffer(GL_ARRAY_BUFFER, tileVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tilemapCellPositions), tilemapCellPositions, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // bind texture data
    glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textureVertices), textureVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Renderer::renderPartialTilemapTile(int tileID, float tileX, float tileY, float clipX, float clipY, float clipWidth, float clipHeight){
        glBindTexture(GL_TEXTURE_2D, textureAtlasId);

    // Update the model matrix to position the tile
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(tileX, tileY, 0.0f));

    // Calculate texture coordinates for the clipped region
    float u1 = clipX / tileSize;
    float v1 = clipY / tileSize;
    float u2 = (clipX + clipWidth) / tileSize;
    float v2 = (clipY + clipHeight) / tileSize;

    // Define the vertices and texture coordinates for the clipped region
    float vertices[] = {
        clipX, clipY, 0.0f,
        clipX + clipWidth, clipY, 0.0f,
        clipX, clipY + clipHeight, 0.0f,
        clipX + clipWidth, clipY, 0.0f,
        clipX, clipY + clipHeight, 0.0f,
        clipX + clipWidth, clipY + clipHeight, 0.0f
    };
    float texCoords[] = {
        u1, v1,
        u2, v1,
        u1, v2,
        u2, v1,
        u1, v2,
        u2, v2
    };

    // Bind vertex array object and buffers
    glBindVertexArray(tileVAO);

    // Update vertex buffer data
    glBindBuffer(GL_ARRAY_BUFFER, tileVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    // Update texture coordinate buffer data
    glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

    // Set model matrix uniform
    GLuint modelLoc = glGetUniformLocation(tileShaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // Draw the clipped portion of the tile
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Unbind vertex array object
    glBindVertexArray(0);
}

void Renderer::renderPlayerSprite(){
    
    // set the player texture
    glBindTexture(GL_TEXTURE_2D, testPlayer.playerTextureID);
    float xPos = testPlayer.playerPosX;
    float yPos = testPlayer.playerPosY;
    
    //update the model view matrix
    GLuint modelLoc = glGetUniformLocation(tileShaderProgram, "model");
    glm::mat4 model = 
        glm::translate(glm::mat4(1.0f), 
        glm::vec3(0.5f, 0.5f, 1.0f)) * 
        glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * 
        glm::scale(glm::mat4(1.0f), glm::vec3(1.0f / 25, 1.0f / 25, 1.0f));

            
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));           

    glBindVertexArray(tileVAO);
    
    // bind position data
    glBindBuffer(GL_ARRAY_BUFFER, tileVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tilemapCellPositions), tilemapCellPositions, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // bind texture data
    glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(playerTexCoords), playerTexCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

}

void Renderer::setShaderProgram(GLuint programID){
    glUseProgram(programID);
}

GLuint Renderer::getShaderProgram(){
    return programID;
}

void Renderer::setCamera(Camera cam){
    camera = cam;
}

void Renderer::translateCamera(float x, float y){
    camera.translateCamera(x, y);
}

void Renderer::renderCursor(double mouseX, double mouseY) {

    glBindTexture(GL_TEXTURE_2D, cursorTexture);

    float mX = mouseX / screenWidth;
    float mY = mouseY / screenHeight;


    GLuint modelLoc = glGetUniformLocation(tileShaderProgram, "model");
    glm::mat4 model = 
        glm::translate(glm::mat4(1.0f), 
        glm::vec3(mX, 1.0 - mY, 1.0f)) * 
        glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * 
        glm::scale(glm::mat4(1.0f), glm::vec3(1.0f / 50, 1.0f / 50, 1.0f));

            
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));  

    glBindVertexArray(cursorVAO);
    // bind position data
    glBindBuffer(GL_ARRAY_BUFFER, tileVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tilemapCellPositions), tilemapCellPositions, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // bind texture data
    glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cursorTexCoords), cursorTexCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);


}

GLuint Renderer::loadTexture(const std::string& filename){
    int width, height, numChannels;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &numChannels, 0);
     if (!data) {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum format = (numChannels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;
}
