#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <utility>      // std::pair
#include "InputAction.hpp"
class InputManager{
public:
    InputManager(GLFWwindow* window);

    void init(GLFWwindow* window);
    bool isKeyPressed(int key);
    InputAction getDirection();
    void getMouseClickCoordinates();
    std::pair<int, int> getMousePosition();
private:
    GLFWwindow* i_window;
    int mouse_X;
    int mouse_Y;
};