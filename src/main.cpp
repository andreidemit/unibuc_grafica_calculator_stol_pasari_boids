#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "config.h"
#include "init.h"
#include "boids.h"
#include "render.h"
#include "input.h"

int main() {
    if (!glfwInit()) {
        std::cout << "Nu am putut initializa GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Stol de Pasari - Boids", NULL, NULL);
    if (!window) { 
        std::cout << "Nu am putut crea fereastra" << std::endl;
        glfwTerminate(); 
        return -1; 
    }
    
    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, key_callback);
    
    glewExperimental = GL_TRUE;
    GLenum err = glewInit(); // Initializez GLEW penru a avea acces la functiile OpenGL
    if (err != GLEW_OK) {
        std::cout << "Eroare GLEW: " << glewGetErrorString(err) << std::endl;
        return -1;
    }
    
    init();
    
    std::cout << "=== Stol de Pasari - Boids ===" << std::endl;
    std::cout << "Algoritmul Boids: Separare, Aliniere, Coeziune" << std::endl;
    std::cout << "Controale:" << std::endl;
    std::cout << "  Click: seteaza punctul de atractie" << std::endl;
    std::cout << "  R: resetare stol" << std::endl;
    
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        
        actualizeaza();
        desenareStol();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
