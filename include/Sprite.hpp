#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Texture.hpp"
#include "ShaderProgram.hpp"

class Sprite {
public:
    Sprite(
        Texture spriteTexture, 
        ShaderProgram& spriteShader, 
        int frameWidth, 
        int frameHeight, 
        int numFrames, 
        float frameTime
    );

    void initRenderData();
    void draw();

    void setPosition(const glm::vec2& position);
    void move(const glm::vec2& delta);
    void setScale(const glm::vec2& scale);
    void setRotation(float angle);
    void update(float deltaTime);

    GLuint getTexture() const;
    glm::vec2 getPosition() const;
    GLuint getShaderProgram() const;
    glm::mat4 getModelMatrix() const;
    GLuint getVAO() const;
    GLuint getVBO() const;
    GLuint getTexVBO() const;

private:
    GLuint m_spriteTexture;
    GLuint m_spriteShader;

    GLuint VAO, VBO, texVBO;
    glm::vec2 position;
    glm::vec2 scale;
    float rotation;
    glm::mat4 modelMatrix;

    int frameWidth;
    int frameHeight;
    int numFrames;
    float frameTime;
    float currentFrameTime;
    int currentFrame;

    std::vector<glm::vec2> cellPositions;
    std::vector<glm::vec2> texCoords;

    void updateModelMatrix();
    void updateTexCoords();
};

#endif
