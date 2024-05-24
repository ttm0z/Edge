#include <iostream>
#include <string>
#include <Tilemap.hpp>

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Layer.hpp>
#include <algorithm> 


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

const char* tileMapVertexPath;
const char* tileMapFragmentPath;
Tilemap::Tilemap(
    const char* tileMapPath, 
    const char* textureAtlasPath,
    const char* vertexShaderPath,
    const char* fragmentShaderPath,
    int tileWidth,
    int tileHeight,
    int tileMapWidth,
    int tileMapHeight
)
:   
    
    m_shaderId(),
    m_tileWidth(tileWidth),
    m_tileHeight(tileHeight),
    m_tilemap(),
    m_tileUVs(),
    m_tileIDs(),
    m_tileMapWidth(tileMapWidth),
    m_tileMapHeight(tileMapHeight),
    tileVAO(0), tileVBO(0), tileTexVBO(0)

{
    tileMapVertexPath = vertexShaderPath;
    tileMapFragmentPath = fragmentShaderPath;

}

void Tilemap::LoadTilemap(std::string tileMapPath) {
    

    float tileSizeX = 32;
    float tileSizeY = 32;

    float atlasWidth = 32;
    float atlasHeight = 320;

    if (!m_tilemap.load(tileMapPath)) {
        std::cout << "Could not load the tilemap" << std::endl;
    } else {
        std::cout << "Tilemap Loaded successfully" << std::endl;
        const auto& layers = m_tilemap.getLayers();
        int count = 0;
        for (const auto& layer : layers) {
            if (layer->getType() == tmx::Layer::Type::Tile) {
                const tmx::TileLayer* tileLayer = dynamic_cast<const tmx::TileLayer*>(layer.get());
                if (tileLayer) {
                    const auto& tiles = tileLayer -> getTiles();
                    for (std::size_t i = 0; i < tiles.size(); ++i) {
                        const auto& tile = tiles[i];
                        
                        int tileMapID = tile.ID;
                        int atlasID = m_atlasTable[tileMapID];
 
                        //calculate uv coordinates for each tile
                        float tileX = 0.0;
                        float tileY = (float) 32 * atlasID;
                        float u = (float) tileX / 32;
                        float v = (float) tileY / 320;
                        float u2 = 1.0f;
                        float v2 = (float) (tileY + 32) / 320;
                        m_tileUVs[tileMapID] = glm::vec4(u,v,u2,v2);
                        m_tileIDs.push_back(tileMapID);
                    }
                }
            } 
            else{
                    std::cerr << "Failed to cast layer to TileLayer" << std::endl;
            }
        }
    }
}

void Tilemap::LoadAtlas(std::string atlasPath){
    m_textureAtlas = loadTexture(atlasPath);
}

void Tilemap::init(){
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    m_shaderId = LoadShaders(tileMapVertexPath, tileMapFragmentPath);
    setShaderProgram(m_shaderId);
    m_atlasTable =         
        {
            {1478, 0},
            {1469, 1},
            {1467, 1},
            {260, 2},
            {591, 3}, 
            {590, 4},
            {1471, 1},
            {1473, 1},
            {258, 4},
            {258, 3}

    };
    
    // Generate and bind Vertex Array Object (VAO)
    glGenVertexArrays(1, &tileVAO);
    glBindVertexArray(tileVAO);

    // Generate and bind VBOs
    glGenBuffers(1, &tileVBO);
    glBindBuffer(GL_ARRAY_BUFFER, tileVBO);

    glGenBuffers(1, &tileTexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, tileTexVBO);

    //positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)tilemapCellPositions);
    glEnableVertexAttribArray(0);

    //texture coordinates
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)tilemapTexCoords);
    glEnableVertexAttribArray(1);

    GLuint shaderLoc = m_shaderId;

    GLuint projectionLoc = glGetUniformLocation(shaderLoc, "projection");
    glm::mat4 projection = glm::ortho(0.0f, (float)1.0f, 0.0f, (float)1.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    GLuint viewLoc = glGetUniformLocation(shaderLoc, "view");
    glm::mat4 view = glm::mat4(1.0f);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    GLuint modelLoc = glGetUniformLocation(shaderLoc, "model");
    glm::mat4 model = glm::translate(glm::mat4(1.0f), 
    glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::mat4(1.0f), 
    glm::vec3(1.0f, 1.0f, 1.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

}

void Tilemap::render(Camera camera){

    float numTilesX = 25.0;
    float numTilesY = 25.0;
    int tileMapWidth = 75;
    int tileMapHeight = 50;
    
    
    int startX = std::max(0, static_cast<int>(camera.tileX));
    int startY = std::max(0, static_cast<int>(camera.tileY));
    int endX = std::min(m_tileMapWidth, startX + 25);
    int endY = std::min(m_tileMapHeight, startY + 25);

    //int startX = 0;
    //int startY = 0;
    //int endX = startX + 25;
    //int endY = startX + 25;

     //render tile
    for(float i = startX; i < endX; i += 1.0){
        for(float j = startY; j< endY; j += 1.0){
            int tileID = m_tileIDs[j * m_tileMapWidth + i];
            
            // in pixels
            float tileX = i * 32.0;
            float tileY = j * 32.0;
            
            renderTilemapTile(camera, tileID, i, j);
            
        }
    }
}

void Tilemap::renderTilemapTile(Camera camera, int tileID, float i, float j){
    
    glBindTexture(GL_TEXTURE_2D, m_textureAtlas);

    //update the model view matrix
    GLuint modelLoc = glGetUniformLocation(m_shaderId, "model");
    glm::mat4 model = glm::translate(glm::mat4(1.0f), 
    glm::vec3((i - camera.tileX) / 25.0, (j - camera.tileY) / 25.0, 1.0f)) * glm::scale(glm::mat4(1.0f), 
    glm::vec3(1.0f / 25.0, 1.0f / 25.0, 1.0f));
            
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(tileVAO);
    const auto& uv = m_tileUVs[tileID];
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
    glBindBuffer(GL_ARRAY_BUFFER, tileTexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textureVertices), textureVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}



void Tilemap::setShaderProgram(GLuint programID){
        glUseProgram(programID);
}
