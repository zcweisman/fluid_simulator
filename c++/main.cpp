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
static int reverse_index[FLUIDSIZE];

static GLProgram program;
static UpdateObject object;
static GLFWwindow* window;

#include "GLSL.hpp"

int main( void ) {
    const GLubyte* renderer;
    const GLubyte* version;
    int windowHeight, windowWidth;

    initUpdateObject();

    srand( time( NULL ) );

    fprintf(stderr, "Initializeing GLFW...");
    if ( !glfwInit() ) exit(EXIT_FAILURE);

    glfwWindowHint( GLFW_SAMPLES, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 2 );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

    windowHeight    = 768;
    windowWidth     = 768;

    window          = glfwCreateWindow(windowWidth, windowHeight,
                        "2D Navier-Stokes Simulator", NULL, NULL);

    if ( !window ) { glfwTerminate(); exit( EXIT_FAILURE ); }

    glfwMakeContextCurrent( window );
    glfwSwapInterval(1);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_click_callback);

    glewExperimental = GL_TRUE;
    std::cout << "GL ERROR: " << glGetError() << "\n";
    if ( glewInit() != GLEW_OK ) {
        fprintf( stderr, "Failed to initialize GLEW\n" );
        return -1;
    }
    std::cout << "GL ERROR: " << glGetError() << "\n";

    glClearColor( 0.5, 0.05, 0.06, 1.0 );

    std::cout << "Initialzing fluid field...";

    Fluid* field = new Fluid(FLUIDSIZE, windowWidth);
    field->setDiffuse(0.2);
    field->setTimeStep(0.1);
    field->setViscosity(0.00089); // Viscosity of water
    field->setIterations(20);

    int count = FLUIDSIZE-1, i = 0;
    for ( ; i < FLUIDSIZE; i++ ) reverse_index[i] = count--;

    std::cout << "Completed\n";

    std::cout << "Installing shaders...";
    program.program = GLSL::loadShaders("shd/vert.glsl", "shd/frag.glsl");
    std::cout << "Completed\n";

    GLSL::initShaderVars();
    GLSL::initArrays();
    GLSL::initVBO();

    while ( !glfwWindowShouldClose( window ) ) {
        //Pass the simulator system updates
        glClear( GL_COLOR_BUFFER_BIT );
        glUseProgram( program.program );

        field->addVelocity( object.velocityXAmount, object.velocityYAmount,
         object.velocityXPos, object.velocityYPos );
        field->addDensity( object.densityAmount, object.densityXPos,
         object.densityYPos );
        field->update();

        //GLSL::bufferData(field);

        glEnableVertexAttribArray( program.attribute_vertex );
        glBindBuffer( GL_ARRAY_BUFFER, program.vbo );
        glVertexAttribPointer( program.attribute_vertex, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

        // Enable density array
        glEnableVertexAttribArray( program.attribute_density ); //Problem
        glBindBuffer( GL_ARRAY_BUFFER, program.dbo );
        glVertexAttribPointer( program.attribute_density, 1, GL_FLOAT, GL_FALSE, 0, (void*)0 ); //Problem

        // Enable velocity array
        glEnableVertexAttribArray( program.attribute_velocity );
        glBindBuffer( GL_ARRAY_BUFFER, program.velbo );
        glVertexAttribPointer( program.attribute_velocity, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 ); //Problem

        //Bind the index array
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, program.ibo );
        //glUniform1i( program.uniform_size, FLUIDSIZE );
        //Transformations here

        assert(glGetError() == GL_NO_ERROR);
        glDrawElements( GL_TRIANGLES, FLUIDSIZE*FLUIDSIZE, GL_UNSIGNED_INT, 0 );
        //glDrawArrays( GL_TRIANGLES, 0, FLUIDSIZE*FLUIDSIZE );
        assert(glGetError() == GL_NO_ERROR);

        glDisableVertexAttribArray( program.attribute_vertex );
        glDisableVertexAttribArray( program.attribute_density );
        glDisableVertexAttribArray( program.attribute_velocity );
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
        glUseProgram ( 0 );
        glfwSwapBuffers( window );
        glfwPollEvents();
    }

    glDeleteBuffers( 1, &program.vbo );
    glDeleteBuffers( 1, &program.ibo );
    glDeleteBuffers( 1, &program.velbo );
    glDeleteBuffers( 1, &program.dbo );

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
    object.mouseXPos = (int)((x/768)*FLUIDSIZE - 0.5f);
    object.mouseYPos = reverse_index[(int)((y/768)*FLUIDSIZE - 0.5f)];

    if ( program.mouse_click ) {
        static double dx = object.mouseXPos - object.mouseXPos0;
        static double dy = object.mouseYPos - object.mouseYPos0;

        object.velocityXAmount = dx/FLUIDSIZE;
        object.velocityYAmount = dy/FLUIDSIZE;
        object.velocityXPos = object.mouseXPos; // Need to translate this to object space
        object.velocityYPos = object.mouseYPos;

        object.densityAmount += 0.05;//???
        object.densityXPos = object.mouseXPos;
        object.densityYPos = object.mouseYPos;
    } else {
        object.densityAmount = 0;
    }

    object.mouseXPos0 = object.mouseXPos;
    object.mouseYPos0 = object.mouseYPos;

    std::cout << object.mouseXPos << ", " << object.mouseYPos << std::endl;

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
    object.densityXPos      = 0;
    object.densityYPos      = 0;
    object.densityAmount    = 0.0;
    object.velocityXAmount  = 0.0;
    object.velocityYAmount  = 0.0;
    object.velocityXPos     = 0;
    object.velocityYPos     = 0;
    object.velocityXPos0    = 0;
    object.velocityYPos0    = 0;
    object.mouseXPos        = 0.0;
    object.mouseYPos        = 0.0;
    object.mouseXPos0       = 0;
    object.mouseYPos0       = 0;
}
