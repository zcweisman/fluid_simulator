/*
 * Zachary Weisman
 * main.cpp
 * Accomplishes tasks related to initializing the GL pipeline
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <string>
#include <stdio.h>

#define FLUIDSIZE 8

#include "structs.h"
#include "fluid.hpp"

void initUpdateObject();

static void key_callback( GLFWwindow*, int, int, int, int );
static void error_callback( int, const char* );
static void cursor_position_callback( GLFWwindow*, double, double );
static void mouse_click_callback( GLFWwindow*, int, int, int );

static GLProgram program;
static UpdateObject object;
static GLFWwindow* window;

#include "GLSL.hpp"

int main( void ) {
    const GLubyte* renderer;
    const GLubyte* version;
    int windowHeight, windowWidth;

    srand( time( NULL ) );

    fprintf(stderr, "Initializeing GLFW...");
    if ( !glfwInit() ) exit(EXIT_FAILURE);

    glfwWindowHint( GLFW_SAMPLES, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    windowHeight    = 768;
    windowWidth     = 768;

    window          = glfwCreateWindow(windowWidth, windowHeight,
                        "2D Navier-Stokes Simulator", NULL, NULL);

    if ( !window ) { glfwTerminate(); exit( EXIT_FAILURE ); }

    glfwMakeContextCurrent( window );
    glfwSwapInterval(1);
    //glfwSetInputMode(window, )
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_click_callback);

    glewExperimental = true;
    GLenum error = glGetError();
    if ( error != GL_NO_ERROR ) { std::cout << "OpenGL Error: " << error; }
    if ( glewInit() != GLEW_OK ) {
        fprintf( stderr, "Failed to initialize GLEW\n" );
        return -1;
    }
    std::cout << glGetError() << std::endl;
    assert(glGetError() == GL_NO_ERROR);

    initUpdateObject();

    std::cout << "Initialzing fluid field...";
    Fluid* field = new Fluid(FLUIDSIZE, windowWidth);
    field->setDiffuse(0.2);
    field->setTimeStep(0.1);
    field->setViscosity(0.00089); // Viscosity of water
    field->setIterations(20);
    std::cout << "Completed\n";

    std::cout << "Installing shaders...";
    GLSL::loadShaders("shd/vert.glsl", "shd/frag.glsl");
    std::cout << "Completed\n";
    GLSL::initShaderVars();
    GLSL::initVBO();

    glClearColor(0.1, 0.6, 0.5, 0.5);

    while ( !glfwWindowShouldClose( window ) ) {
        //Pass the simulator system updates
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        field->addVelocity( object.velocityXAmount, object.velocityYAmount,
         object.velocityXPos, object.velocityYPos );
        field->addDensity( object.densityAmount, object.densityXPos,
         object.densityYPos );
        field->update();
        GLSL::draw();
        glfwSwapBuffers( window );
        glfwPollEvents();
    }

    glfwTerminate();
    delete field;

    return 0;
}

static void key_callback(GLFWwindow* window, int key, int scancode,
int action, int mods) {
            ;
            //So far nothing for key press.
}

static void error_callback(int error, const char* description) {
    fputs(description, stderr);
}

static void cursor_position_callback( GLFWwindow* w, double x, double y ) {
    if ( program.mouse_click ) {
        static double last_x = 0;
        static double last_y = 0;

        object.velocityXAmount = x - last_x;
        object.velocityYAmount = y - last_y;
        object.velocityXPos = x; // Need to translate this to object space
        object.velocityYPos = y;

        last_x = x;
        last_y = y;

        object.densityAmount += 0.5;//???
        object.densityXPos = x;
        object.densityYPos = y;
    } else {
        object.densityAmount = 0;
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
        std::cout << "CLICKITY CLICK\n";
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
