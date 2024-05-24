#include "InputManager.hpp"
#include <iostream>


InputManager::InputManager(GLFWwindow* window){
    std::cout << "Input Manager Initialized" << std::endl;
    init(window);
}


InputAction InputManager::getDirection() {
    // switch to a vector of input actions
    if (glfwGetKey(i_window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(i_window, GLFW_KEY_A) == GLFW_PRESS) {
        return InputAction::Left;
    }
    if (glfwGetKey(i_window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(i_window, GLFW_KEY_D) == GLFW_PRESS) {
        return InputAction::Right;
    }
    if (glfwGetKey(i_window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(i_window, GLFW_KEY_S) == GLFW_PRESS) {
        return InputAction::Down;
    }
    if (glfwGetKey(i_window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(i_window, GLFW_KEY_W) == GLFW_PRESS) {
        return InputAction::Up;
    }
    return InputAction::null;
}

void InputManager::getMouseClickCoordinates(){
    
    if(glfwGetMouseButton(i_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        std::cout << "Left Mouse Click" << std::endl;
    }
}


void InputManager::init(GLFWwindow* window){
    i_window = window;
}

bool InputManager::isKeyPressed(int key){return false;}

std::pair<int, int> InputManager::getMousePosition(){
    return {mouse_X, mouse_Y};
} 