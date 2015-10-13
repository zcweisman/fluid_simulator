/*
 * Zachary Weisman
 * main.cpp
 * Accomplishes tasks related to initializing the GL pipeline
 */

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "GLSL.hpp"
#include "simulator.hpp"

#define FLUIDSIZE 8

static void key_callback(GLFWwindow*, int, int, int, int);
static void error_callback(int, const char*);
static void cursor_position_callback(GLFWwindow*, double, double);

static GLProgram program;

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

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    Simulator sim = new Simulator(FLUIDSIZE, windowWidth);

    while ( 1 ) {
        sim.update();
    }

    return 0;
}

static void key_callback(GLFWwindow* window, int key, int scancode,
int action, int mods) {
            ;
}

static void error_callback(int error, const char* description) {
    fputs(description, error);
}

static void cursor_positon_callback(GLFWwindow* w, double x, double y) {
    static double last_x = x;
    static double last_y = y;

    static double dx = x - last_x;
    static double dy = y - last_y;
}
