#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {
public:

    Window(int width, int height, const char* title);
    ~Window();

    void update();
    void clear();
    bool isOpen() const;
    void pollEvents();
    void swapBuffers();
    GLFWwindow* getWindow();
    void processInput();
    int getWidth() const;
    int getHeight() const;
    double getMousePosX();
    double getMousePosY();

    // Add more methods as needed for window management

private:
    int width;
    int height;

    double mousePosX;
    double mousePosY;

    const char* title;
    GLFWwindow* window;
    
};

#endif // WINDOW_H
