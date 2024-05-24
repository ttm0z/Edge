#pragma once

#include <GL/glew.h>
#include <glm.hpp>
#include <string>
#include <vector>
#include "LoadShader.hpp"
#include <gtc/matrix_transform.hpp>
#include "LoadTexture.hpp"

#include <gtc/type_ptr.hpp> // For glm::ortho
#include <glm.hpp>

class PlayerSprite{

public:

    PlayerSprite(
        const char* spritesheetPath,
        const char* vertexShaderPath,
        const char* fragmentShaderPath
);
    
    void setPosition(const glm::vec2& position);
    
    void move(const glm::vec2& delta);
    
    void setScale(const glm::vec2& scale);
    
    void setRotation(float angle);
    
    GLuint getPlayerTexture();

    glm::vec2 getPosition();

    GLuint getShaderProgram();
    
    glm::mat4 getModelMatrix();

    GLuint getVAO();

    void setShaderProgram(GLuint programID);

    GLuint getVBO();

    GLuint getTexVBO();

    std::vector<glm::vec2> getCellPositions();

    std::vector<glm::vec2> getTexCoords();

    void updateModelMatrix();
    
    void init();
    
    void render(int spritesheet_frame, int direction);

private:
    
    GLuint VAO, VBO, texVBO, EBO;
    
    glm::vec2 position;
    
    glm::vec2 scale;
    
    float rotation;
    
    glm::mat4 modelMatrix;

    GLuint m_spriteTexture;
    
    GLuint m_playerShader;

    GLuint playerTextureID;

    std::vector<glm::vec2> cellPositions;
    
    std::vector<glm::vec2> texCoords;

};