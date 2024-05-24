#include "Sprite.hpp"
#include <iostream>
float spriteCellPositions[] = {
    // Positions       
    -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
    
     1.0f,  1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
};

float spriteTexCoords[] = {
    0.0f, 0.1f, // Top Left (Vertex 1)
    1.0f, 0.1f, // Top Right (Vertex 2)
    0.0f, 0.0f, // Bottom Left (Vertex 3)
    
    1.0f, 0.1f, // Top Right (Vertex 4)
    0.0f, 0.0f, // Bottom Left (Vertex 5)
    1.0f, 0.0f  // Bottom Right (Vertex 6)
};

const char* vertexPath;
const char* fragmentPath;
const char* spritesheetTexturePath;

Sprite::Sprite(
    const char* spritesheetPath,
    const char* vertexShaderPath,
    const char* fragmentShaderPath,
    float spritesheet_width,
    float spritesheet_height
    ) 
{
    vertexPath = vertexShaderPath;
    fragmentPath = fragmentShaderPath;
    spritesheetTexturePath = spritesheetPath; 
    m_spritesheetWidth  = spritesheet_width;
    m_spritesheetHeight = spritesheet_height;
    m_spritesheet_frame = 0;
    m_spriteDirection = InputAction::null;
    directionMap = {
        {InputAction::Up, 3},
        {InputAction::Down, 0},
        {InputAction::Left, 1},
        {InputAction::Right, 2},
    };
    std::cout << "player character initialized" << std::endl;

};

void Sprite::updateSpriteFrame(){
    m_spritesheet_frame += 1;
    if(m_spritesheet_frame == m_spritesheetWidth)
        m_spritesheet_frame = 0;
}

void Sprite::updateDirection(InputAction dir){
    m_spriteDirection = dir;
}

void Sprite::init() {

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    m_playerShader = LoadShaders(vertexPath, fragmentPath);
    setShaderProgram(m_playerShader);
    m_spriteTexture = loadTexture(spritesheetTexturePath);
    std::cout << "spritesheetTex: "<< spritesheetTexturePath << std::endl;
    std::cout << "spritesheetTex: "<< vertexPath << std::endl;
    std::cout << "spritesheetTex: "<< fragmentPath << std::endl;


    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &texVBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cellPositions), spriteCellPositions, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, texVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), spriteTexCoords, GL_STATIC_DRAW);


    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);


    GLuint shaderLoc = m_playerShader;

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

void Sprite::render(){
        glBindTexture(GL_TEXTURE_2D, m_spriteTexture);
        GLuint modelLoc = glGetUniformLocation(m_playerShader, "model");

        glm::mat4 model = 
        glm::translate(glm::mat4(1.0f), 
        glm::vec3(0.5f, 0.5f, 1.0f)) * 
        glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * 
        glm::scale(glm::mat4(1.0f), glm::vec3(1.0f / 10, 1.0f / 10, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));           

        // get texture coordinates based on spritesheet frame
        // 0 front, 1 right, 2 left, 4 back
        //replace 5 and 4 with spritesheet width and height
        float direction = (float)directionMap[m_spriteDirection];

        float spriteTexCoords[] = {
        (0.0f + (1.0f * m_spritesheet_frame))  / m_spritesheetWidth, (1.0f + (1.0f * direction)) / m_spritesheetHeight, // Top Left (Vertex 1)
        (1.0f + (1.0f * m_spritesheet_frame)) / m_spritesheetWidth, (1.0f + (1.0f * direction)) / m_spritesheetHeight, // Top Right (Vertex 2)
        (0.0f + (1.0f * m_spritesheet_frame)) / m_spritesheetWidth, (0.0f + (1.0f * direction)) / m_spritesheetHeight, // Bottom Left (Vertex 3)
    
        (1.0f + (1.0f * m_spritesheet_frame))  / m_spritesheetWidth, (1.0f + (1.0f * direction)) / m_spritesheetHeight, // Top Right (Vertex 4)
        (0.0f + (1.0f * m_spritesheet_frame))  / m_spritesheetWidth, (0.0f + (1.0f * direction)) / m_spritesheetHeight, // Bottom Left (Vertex 5)
        (1.0f + (1.0f * m_spritesheet_frame))  / m_spritesheetWidth, (0.0f + (1.0f * direction)) / m_spritesheetHeight  // Bottom Right (Vertex 6)
    };


        glBindVertexArray(VAO);
            // bind position data
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(spriteCellPositions), spriteCellPositions, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);


        // bind texture data
        glBindBuffer(GL_ARRAY_BUFFER, texVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(spriteTexCoords), spriteTexCoords, GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
}

void Sprite::setShaderProgram(GLuint programID){
        glUseProgram(programID);
}
