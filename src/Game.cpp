#include "Game.hpp"
#include "Tilemap.hpp"
#include <iostream>


Game::Game()
    : window(800, 800, "Game"), 
    inputManager(window.getWindow()),
    g_tilemap(
        "./assets/desert_test_map.tmx",
        "./assets/SimpleTextureAtlas1.png",
        "./shaders/tileVert.glsl",
        "./shaders/tileFrag.glsl",
        32,
        32,
        100,
        100
    ),
    player("./assets/redspritesheet.png", "./shaders/tileVert.glsl", "./shaders/tileFrag.glsl"),
    lastPrintTime(Clock::now()),
    camera(200.0,200.0,800.0,800.0)
{
    // initialize glew
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


}



Game::~Game()
{

}

void Game::run()
{
    double lastFrameTime = glfwGetTime();
    const float frameRate = 1.0f / 60.0f;
    
    
    g_tilemap.init();
    g_tilemap.LoadAtlas("./assets/SimpleTextureAtlas1.png");
    g_tilemap.LoadTilemap("./assets/desert_test_map.tmx");

    while(window.isOpen()){
        
        double currentFrameTime = glfwGetTime();
        double deltaTime = currentFrameTime - lastFrameTime;
        
        
        InputAction dir = inputManager.getDirection();
        
        inputManager.getMouseClickCoordinates();
        switch(dir){
            case(InputAction::Up):                
                camera.translateCamera(0.0, 1.0);
                break;
            
            case(InputAction::Down):
                camera.translateCamera(0.0, -1.0);
                break;
            
            case(InputAction::Left):
                camera.translateCamera(-1.0, 0.0);
                break;
            
            case(InputAction::Right):
                camera.translateCamera(1.0, 0.0);
                break;
        }
        update(deltaTime);
        lastFrameTime = currentFrameTime;
        render();
    }
}

void Game::update(int deltaTime)
{
    window.update();

    mouseX = window.getMousePosX();
    mouseY = window.getMousePosY();
}

void Game::render()
{

    glClear(GL_COLOR_BUFFER_BIT);
    
    g_tilemap.render(camera);
    player.render();
}
