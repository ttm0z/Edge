#ifndef CURSOR_HPP
#define CURSOR_HPP

#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp> // For glm::ortho
#include "Window.hpp"
#include <glm.hpp>

class Cursor {
public:
    Cursor(Window window);
    ~Cursor();

    void render(double mouseX, double mouseY);

private:
    Window window;
};

#endif // CURSOR_HPP
