#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "GLSL.hpp"
#include "simulator.hpp"

void main() {
    const GLubyte* renderer;
    const GLubyte* version;

    srand(time(NULL));

    fprintf(stderr, "Initializeing GLFW...");
    if ( !glfwInit() ) exit(EXIT_FAILURE);
    fprintf(stderr, "Complete\n");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    windowHeight    = 768;
    windowWidth     = 768;

    window          = glfwCreateWindow(windowWidth, windowHeight, "2D CFD Simulator", NULL, NULL);

    if ( window == NULL ) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    Simulator sim   = new Simulator(8, );

    return 0;
}
