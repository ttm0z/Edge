#include "Game.hpp"
#include "Tilemap.hpp"
#include <iostream>

int playerDirection, spriteSheetFrame;

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
    player("./assets/redspritesheet.png", "./shaders/tileVert.glsl", "./shaders/tileFrag.glsl", 5.0f, 4.0f),
    lastPrintTime(Clock::now()),
    camera(200.0,200.0,800.0,800.0),
    animationTimer(0.0)
{
    // initialize glew
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    spriteSheetFrame = 0;
    playerDirection = 0;
}



Game::~Game()
{

}

void Game::run()
{
    double lastFrameTime = glfwGetTime();
    const float frameRate = 1.0f / 60.0f;
    
    player.init();

    g_tilemap.init();
    g_tilemap.LoadAtlas("./assets/SimpleTextureAtlas1.png");
    g_tilemap.LoadTilemap("./assets/desert_test_map.tmx");

    while(window.isOpen()){
        
        double currentFrameTime = glfwGetTime();
        double deltaTime = currentFrameTime - lastFrameTime;
        
        animationTimer += deltaTime;
        
            InputAction dir = inputManager.getDirection();
            
            inputManager.getMouseClickCoordinates();

            switch(dir){
                case(InputAction::Up):                
                    camera.translateCamera(0.0, 1.0);
                    playerDirection = 3;
                    break;
                
                case(InputAction::Down):
                    camera.translateCamera(0.0, -1.0);
                    playerDirection = 0;
                    break;
                
                case(InputAction::Left):
                    camera.translateCamera(-1.0, 0.0);
                    playerDirection = 1;
                    break;
                
                case(InputAction::Right):
                    camera.translateCamera(1.0, 0.0);
                    playerDirection = 2;
                    break;
            
            }
            
            //update animations
            if(dir != InputAction::null && animationTimer > 0.2){
                player.updateDirection(dir);
                player.updateSpriteFrame();
                animationTimer = 0.0; // Reset the animation timer
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
