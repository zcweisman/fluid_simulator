/*
 * Zachary Weisman
 * main.cpp
 * Accomplishes tasks related to initializing the GL pipeline
 */

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "structs.h"
#include "fluid.hpp"
#include "GLSL.hpp"

#define FLUIDSIZE 8

void initUpdateObject();

static void key_callback(GLFWwindow*, int, int, int, int);
static void error_callback(int, const char*);
static void cursor_position_callback(GLFWwindow*, double, double);

static GLProgram program;
static UpdateObject object;
static GLFWwindow* window;

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

    initUpdateObject();

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_positon_callback);
    glfwSetMouseButtonCallback(window, mouse_click_callback);

    Fluid* field = new Fluid(FLUIDSIZE, windowWidth);
    field->setDiffuse(0.2);
    field->setTimeStep(0.1);
    field->setViscosity(0.7);
    field->setIterations(20);

    GLSL::loadShaders("vert.glsl", "frag.glsl");
    GLSL::initShaderVars();
    GLSL::initVBO();

    glClearColor(0.1, 0.6, 0.5, 0.5);

    while ( 1 ) {
        //Pass the simulator system updates
        field->addVelocity( object.velocityXAmount, object.velocityYAmount,
         object.velocityXPos, object.velocityYPos );
        field->addDensity( object.densityAmount, object.densityXPos,
         object.densityYPos );
        field->update();
    }

    return 0;
}

static void key_callback(GLFWwindow* window, int key, int scancode,
int action, int mods) {
            ;
            //So far nothing for key press.
}

static void error_callback(int error, const char* description) {
    fputs(description, error);
}

static void cursor_positon_callback(GLFWwindow* w, double x, double y) {
    if ( program.mouse_click ) {
        static double last_x = 0;
        static double last_y = 0;

        update.velocityXAmount = x - last_x;
        update.velocityYAmount = y - last_y;
        update.velocityXPos = x; // Need to translate this to object space
        update.velocityYPos = y;

        last_x = x;
        last_y = y;

        update.densityAmount += 0.5;//???
        update.densityXPos = x;
        update.densityYPos = y;
    } else {
        density = 0;
        static double dx = 0;
        static double dy = 0;
    }

    object.mouseXPos = x;
    object.mouseYPos = y;

    return;
}

static void mouse_click_callback(GLFWwindow* w, int button, int action, int mods) {
    if ( action == GLFW_PRESS ) {
        program.mouse_click = true;
        object.densityAmount += 0.5; //Some arbitrary number
    } else if ( action == GLFW_RELEASE ) {
        program.mouse_click = false;
        object.densityAmount = 0;
    }

    return;
}

void initUpdateObject() {
    object.densityXPos      = 0.0;
    object.densityYPos      = 0.0;
    object.densityAmount    = 0.0;
    object.velocityXAmount  = 0.0;
    object.velocityYAmount  = 0.0;
    object.velocityXPos     = 0.0;
    object.velocityYPos     = 0.0;
    object.mouseXPos        = 0.0;
    object.mouseYPos        = 0.0;
}
