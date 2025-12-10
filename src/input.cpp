#include <GL/glew.h>
#include "input.h"
#include "config.h"
#include "globals.h"
#include "init.h"

void mouse_button_callback(GLFWwindow* window, int button, int action, int /*mods*/) {
    if (action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        targetX = xpos;
        targetY = WINDOW_HEIGHT - ypos;  // Inverseaza Y pentru coordonate OpenGL
    }
}

void key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_R) {
            initPasari();  // Reseteaza stolul
        }
    }
}
