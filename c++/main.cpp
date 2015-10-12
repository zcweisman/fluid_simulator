#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "GLSL.hpp"
#include "simulator.hpp"

GLuint program;

static void key_callback(GLFWwindow*, int, int, int, int);
static void error_callback(int, const char*);
static void cursor_position_callback(GLFWwindow*, double, double);

void main() {
    const GLubyte* renderer;
    const GLubyte* version;
    string fShader = "frag.glsl";
    string vShader = "vert.glsl";

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

static void key_callback(GLFWwindow* window)
