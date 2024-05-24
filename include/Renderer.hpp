// Renderer.hpp
#pragma once

#include <GL/glew.h>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp> // For glm::ortho
#include <glm.hpp>
#include <string>
#include "Window.hpp" 
#include "Camera.hpp"
#include "PlayerSprite.hpp"
#include "LoadShader.hpp"
#include <vector>





class Renderer {
public:
    Renderer(float sWidth, float sHeight, float tWidth, float tHeight);
    ~Renderer();
    
    void init();
    

    GLuint getShaderProgram();

    void loadTilemap(std::string tileMapPath, std::string textureAtlasPath);
    void renderTilemapTile(int tileID, float i, float j);
    void renderTilemap();
    void renderPlayerSprite();
    void renderCursor(double mouseX, double mouseY);
    void renderPartialTilemapTile(int tileID, float tileX, float tileY, float clipX, float clipY, float clipWidth, float clipHeight);

    void translateCamera(float x, float y);
    void setCamera(Camera cam);

    Player testPlayer;
    Camera camera;

    GLuint loadTexture(const std::string& filename);

    std::vector<GLuint> textures;

    GLsizei vertexCount;
    GLsizei vertexTextureCount;

    void setShaderProgram(GLuint programID);

private:
    GLuint programID;
    GLuint vao_ID;
    GLuint vbo_ID;


    int screenWidth;
    int screenHeight;
    int tileWidth;
    int tileHeight;

    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;

};
