#pragma once

#include "Window.hpp"
#include "InputManager.hpp"
#include "WorldGenerator.hpp"
#include "Tilemap.hpp"
#include "Camera.hpp"
#include "PlayerSprite.hpp"
#include <chrono>

using Clock = std::chrono::steady_clock;
using TimePoint = std::chrono::time_point<Clock>;



class Game {
public:
    Game();
    ~Game();
    void run();
    void update(int deltaTime);
    void render();

    int tileWidth;
    int tileHeight;
    int screenWidth;
    int screenHeight;
    double mouseX;
    double mouseY;
    TimePoint lastPrintTime;

private:
    Window window;
    InputManager inputManager;
    WorldGenerator generator;
    Camera camera;
    PlayerSprite player;
    std::vector<std::vector<int>> worldMap;
    static const GLfloat vertexBufferData[];
    GLsizei vertexCount;
    Tilemap g_tilemap;
};