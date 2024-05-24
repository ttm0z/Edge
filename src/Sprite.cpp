#include "Sprite.hpp"
#include <iostream>

Sprite::Sprite(Texture spriteTexture, ShaderProgram& spriteShader, int frameWidth, int frameHeight, int numFrames, float frameTime) 
:
    m_spriteTexture(spriteTexture),
    m_spriteShader(spriteShader),
    frameWidth(frameWidth),
    frameHeight(frameHeight),
    numFrames(numFrames),
    frameTime(frameTime),
    currentFrameTime(0.0f),
    currentFrame(0)
{
    cellPositions = {
        glm::vec2(0.5f, 0.5f), // Top Right
        glm::vec2(0.5f, -0.5f), // Bottom Right
        glm::vec2(-0.5f, -0.5f), // Bottom Left
        glm::vec2(-0.5f, 0.5f)  // Top Left
    };

    updateTexCoords();

    position = glm::vec2(0.0f);
    scale = glm::vec2(1.0f);
    rotation = 0.0f;
    modelMatrix = glm::mat4(1.0f);

    std::cout << "sprite initialized" << std::endl;
}

void Sprite::initRenderData() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &texVBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, cellPositions.size() * sizeof(glm::vec2), cellPositions.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, texVBO);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), texCoords.data(), GL_STATIC_DRAW);

    // Position attribute
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coord attribute
    glBindBuffer(GL_ARRAY_BUFFER, texVBO);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Sprite::draw() {
    m_spriteShader.use();
    m_spriteShader.setMat4("model", modelMatrix);
    m_spriteShader.setInt("spriteTexture", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_spriteTexture.getTextureId());

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
}

void Sprite::setPosition(const glm::vec2& position) {
    this->position = position;
    updateModelMatrix();
}

void Sprite::move(const glm::vec2& delta) {
    position += delta;
    updateModelMatrix();
}

void Sprite::setScale(const glm::vec2& scale) {
    this->scale = scale;
    updateModelMatrix();
}

void Sprite::setRotation(float angle) {
    rotation = angle;
    updateModelMatrix();
}

void Sprite::update(float deltaTime) {
    currentFrameTime += deltaTime;
    if (currentFrameTime >= frameTime) {
        currentFrame = (currentFrame + 1) % numFrames;
        currentFrameTime = 0.0f;
        updateTexCoords();
        glBindBuffer(GL_ARRAY_BUFFER, texVBO);
        glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), texCoords.data(), GL_STATIC_DRAW);
    }
}

GLuint Sprite::getTexture() const {
    return m_spriteTexture.getTextureId();
}

glm::vec2 Sprite::getPosition() const {
    return position;
}

GLuint Sprite::getShaderProgram() const {
    return m_spriteShader.getProgramId();
}

glm::mat4 Sprite::getModelMatrix() const {
    return modelMatrix;
}

GLuint Sprite::getVAO() const {
    return VAO;
}

GLuint Sprite::getVBO() const {
    return VBO;
}

GLuint Sprite::getTexVBO() const {
    return texVBO;
}

void Sprite::updateModelMatrix() {
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(position, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(scale, 1.0f));
    modelMatrix = glm::rotate(modelMatrix, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Sprite::updateTexCoords() {
    float texWidth = 1.0f / (m_spriteTexture.getWidth() / frameWidth);
    float texHeight = 1.0f / (m_spriteTexture.getHeight() / frameHeight);

    int column = currentFrame % (m_spriteTexture.getWidth() / frameWidth);
    int row = currentFrame / (m_spriteTexture.getWidth() / frameWidth);

    float u = column * texWidth;
    float v = row * texHeight;

    texCoords = {
        glm::vec2(u + texWidth, v + texHeight), // Top Right
        glm::vec2(u + texWidth, v), // Bottom Right
        glm::vec2(u, v), // Bottom Left
        glm::vec2(u, v + texHeight) // Top Left
    };
}
