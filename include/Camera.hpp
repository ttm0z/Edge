#pragma once
#include <glm.hpp>
#include <string>

class Camera
{
public:
	Camera(float cameraWidth, float cameraHeight, float screenWidth, float screenHeight);
	
	glm::vec2 center;
	float xPos;
	float yPos;

	float tileX;
	float tileY;
	
	float width;
	float height;

	float s_width;
	float s_height;

	void translateCamera(float x, float y);

};