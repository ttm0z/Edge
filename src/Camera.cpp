#include "Camera.hpp"


Camera::Camera(float cameraWidth, float cameraHeight, float screenWidth, float screenHeight)
: width(cameraWidth), height(cameraHeight), s_width(screenWidth), s_height(screenHeight)
{
    //xPos = s_width / 2;
    //yPos = s_height / 2;
    xPos = 0;
    yPos = 0;
    tileX = xPos / 32.0;
    tileY = yPos / 32.0;
    
}

void Camera::translateCamera(float x, float y){
    
    xPos += x;
    yPos += y;
    tileX = xPos / 32.0;
    tileY = yPos / 32.0;
}