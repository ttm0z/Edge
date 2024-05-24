#include "Window.hpp"
#include <iostream>
#include <iomanip> // For std::setw
#include <algorithm> // For std::find

//constructor, initialze
//create window and initialize glfw context
Window::Window(int width, int height, const char* title) 
: width(width), height(height), title(title),
	mousePosX(0), mousePosY(0)
{
    if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make macOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	window = glfwCreateWindow(width, height, title, NULL, NULL);
    
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
        //delete window ~Window();
	}
	glfwMakeContextCurrent(window);


    // Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	glViewport(0, 0, width, height);

};

//descructor - cleanup
Window::~Window(){
	glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::update() {
	processInput();
	pollEvents();
	swapBuffers();
	glfwGetCursorPos(window, &mousePosX, &mousePosY);
}

void Window::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// check for ESC
void Window::processInput() {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
	}
}

GLFWwindow* Window::getWindow(){return window;}

bool Window::isOpen() const { return !glfwWindowShouldClose(window); };

void Window::pollEvents(){ glfwPollEvents(); };

void Window::swapBuffers(){ glfwSwapBuffers(window); };

int Window::getWidth() const {return width;}

int Window::getHeight() const {return height;}

double Window::getMousePosX() {return mousePosX;}

double Window::getMousePosY() {return mousePosY;}