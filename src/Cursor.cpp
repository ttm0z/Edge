#include "Cursor.hpp"

Cursor::Cursor(Window window) : 
    window(window) 
{

}

float cursorPositions[] = {
    // Positions       
    -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
    
     1.0f,  1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
};

float cursorTexCoords[] = {
    0.0f, 1.0f, // Top Left (Vertex 1)
    1.0f, 1.0f, // Top Right (Vertex 2)
    0.0f, 0.0f, // Bottom Left (Vertex 3)
    
    1.0f, 1.0f, // Top Right (Vertex 4)
    0.0f, 0.0f, // Bottom Left (Vertex 5)
    1.0f, 0.0f  // Bottom Right (Vertex 6)
};

GLuint cursorVAO;
GLuint cursorVBO;

Cursor::~Cursor() {}

