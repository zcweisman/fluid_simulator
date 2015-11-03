/*
 * Zachary Weisman
 * main.cpp
 * Accomplishes tasks related to initializing the GL pipeline
 */
 #include <iostream>
 #include <string>
 #include <stdio.h>
 #include <thread>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define FLUIDSIZE 55

#include "structs.h"

static GLProgram program;
static UpdateObject object;
static GLFWwindow* window;

#include "fluid.hpp"

static Fluid* field;

void initUpdateObject();

static void key_callback( GLFWwindow*, int, int, int, int );
static void error_callback( int, const char* );
static void cursor_position_callback( GLFWwindow*, double, double );
static void mouse_click_callback( GLFWwindow*, int, int, int );
static float angle = 0;

#include "camera.hpp"
#include "glsl.hpp"
#include "environment.hpp"

int main( void ) {
    const GLubyte* renderer;
    const GLubyte* version;
    int windowHeight, windowWidth;
    bool keys[128];

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

    //Camera camera((float)glfwGetTime(), windowWidth, windowHeight);

    glewExperimental = GL_TRUE;
    if ( glewInit() != GLEW_OK ) {
        fprintf( stderr, "Failed to initialize GLEW\n" );
        return -1;
    }
    std::cout << "Normal error <" << glGetError() << "> please ignore\n";

    glClearColor( 0.1, 0.45, 0.5, 1.0 );

    std::cout << "Initialzing fluid field...";

    field = new Fluid(FLUIDSIZE, windowWidth);
    //field->setDiffuse(0.5);
    /*
     * For 150^2 , use density and viscosity of 0.05, timestep of 0.0001.
     *
     */
    field->setTimeStep(0.01);
    //field->setViscosity(0.05); // Viscosity of water
    field->setDiffuse(0.05);
    field->setViscosity(0.3);
    field->setIterations(15);

    std::cout << "Completed\n";

    std::cout << "Installing shaders...";
    //program.program = GLSL::loadShaders("shd/vert.glsl", "shd/frag.glsl");
    program.program = GLSL::loadShadersGeom("shd/vert.glsl", "shd/geom.glsl", "shd/frag.glsl");
    std::cout << "Completed\n";

    GLSL::initShaderVars();
    GLSL::initArrays();
    GLSL::initVBO();

    //Environment::addObstacle();

    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_TRUE);

    //General framecount variable
    int count = 0;
    glm::mat4 view = glm::mat4(1.f);
    glm::mat4 projection = glm::perspective(80.f, 1.f, 0.1f, 100.f);
    glm::mat4 scale = glm::scale(glm::mat4(1.f), glm::vec3(0.5, 0.5f, 0.5f));
    glm::mat4 translate = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -0.5f));
    glm::mat4 rotate;
    glm::mat4 model;

    while (!glfwWindowShouldClose(window)) {
        //Pass the simulator system updates
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram( program.program );

        //Camera::update();
        field->addVelocity( object.velocityXAmount, object.velocityYAmount, object.velocityZAmount,
         object.velocityXPos, object.velocityYPos, object.velocityZPos );
        field->addDensity( object.densityAmount, object.densityXPos,
         object.densityYPos, object.densityZPos );
        field->addDensity( 255, 24, 24, 24 );
        field->addDensity( 255, 24, 24, 27 );
        field->addDensity( 255, 27, 24, 24 );
        field->addDensity( 255, 27, 24, 27 );
        field->addDensity( 255, 24, 27, 24 );
        field->addDensity( 255, 24, 27, 27 );
        field->addDensity( 255, 27, 27, 24 );
        field->addDensity( 255, 27, 27, 27 );

        field->addVelocity(-255, -255, -255, 25, 25, 25);
        field->addVelocity(-255, -255, 255, 25, 25, 26);
        field->addVelocity(-255, 255, -255, 25, 26, 25);
        field->addVelocity(-255, 255, 255, 25, 26, 26);
        field->addVelocity(255, -255, -255, 26, 25, 25);
        field->addVelocity(255, -255, 255, 26, 25, 26);
        field->addVelocity(255, 255, -255, 26, 26, 25);
        field->addVelocity(255, 255, 255, 26, 26, 26);
        field->update();

        GLSL::bufferData(field);

        glEnableVertexAttribArray( program.attribute_vertex );
        glBindBuffer( GL_ARRAY_BUFFER, program.vbo );
        glVertexAttribPointer( program.attribute_vertex, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

        // Enable density array
        glEnableVertexAttribArray( program.attribute_density ); //Problem
        glBindBuffer( GL_ARRAY_BUFFER, program.dbo );
        glVertexAttribPointer( program.attribute_density, 1, GL_FLOAT, GL_FALSE, 0, (void*)0 ); //Problem

        // Enable velocity array
        glEnableVertexAttribArray( program.attribute_velocity_x );
        glBindBuffer( GL_ARRAY_BUFFER, program.velxbo );
        glVertexAttribPointer( program.attribute_velocity_x, 1, GL_FLOAT, GL_FALSE, 0, (void*)0 ); //Problem

        // Enable velocity array
        glEnableVertexAttribArray( program.attribute_velocity_y );
        glBindBuffer( GL_ARRAY_BUFFER, program.velybo );
        glVertexAttribPointer( program.attribute_velocity_y, 1, GL_FLOAT, GL_FALSE, 0, (void*)0 ); //Problem

        rotate = glm::rotate(glm::mat4(1.f), angle, glm::vec3(0.f, 1.f, 0.f));
        model = translate*scale*rotate;

        //Bind the index array
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, program.ibo );
        glUniform1i( program.uniform_size, FLUIDSIZE );
        glUniformMatrix4fv(program.uniform_proj_matrix, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(program.uniform_view_matrix, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(program.uniform_model_matrix, 1, GL_FALSE, glm::value_ptr(model));

        assert(glGetError() == GL_NO_ERROR);
        glDrawElements( GL_POINTS, FLUIDSIZE*FLUIDSIZE*FLUIDSIZE, GL_UNSIGNED_INT, 0 );
        assert(glGetError() == GL_NO_ERROR);

        glDisableVertexAttribArray(program.attribute_vertex);
        glDisableVertexAttribArray(program.attribute_density);
        glDisableVertexAttribArray(program.attribute_velocity_x);
        glDisableVertexAttribArray(program.attribute_velocity_y);
        glDisableVertexAttribArray(program.attribute_velocity_z);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glUseProgram (0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &program.vbo);
    glDeleteBuffers(1, &program.ibo);
    glDeleteBuffers(1, &program.velxbo);
    glDeleteBuffers(1, &program.velybo);
    glDeleteBuffers(1, &program.velzbo);
    glDeleteBuffers(1, &program.dbo);

    glfwTerminate();
    delete field;

    return 0;
}

static void key_callback(GLFWwindow* window, int key, int scancode,
int action, int mods) {
            if (key == GLFW_KEY_A) angle -= 2.5f;
            if (key == GLFW_KEY_D) angle += 2.5f;
            if (key == GLFW_KEY_W) {
                if (object.mouseZPos+1 <= FLUIDSIZE)
                    object.mouseZPos++;
            }
            if (key == GLFW_KEY_S) {
                if (object.mouseZPos-1 >=1)
                    object.mouseZPos--;
            }
}

static void error_callback(int error, const char* description) {
    fputs(description, stderr);
}

static void cursor_position_callback( GLFWwindow* w, double x, double y ) {
    double realY = 768 - y;
    object.mouseXPos = (int)((x/(768-1))*FLUIDSIZE - 0.5f);
    object.mouseYPos = (int)((realY/(768-1))*FLUIDSIZE - 0.5f);

    if ( program.mouse_click ) {
        double dx = object.mouseXPos - object.mouseXPos0;
        double dy = object.mouseYPos - object.mouseYPos0;
        double dz = object.mouseZPos - object.mouseYPos0;
        static double x0 = 0;
        static double y0 = 0;
        static double z0 = 0;

        object.velocityXAmount = (x - x0);
        object.velocityYAmount = (realY - y0);
        object.velocityZAmount = object.mouseZPos - dz;
        x0 = x;
        y0 = realY;
        z0 = object.mouseZPos;

        object.velocityXPos = object.mouseXPos; // Need to translate this to object space
        object.velocityYPos = object.mouseYPos;
        object.velocityZPos = object.mouseZPos;

        object.densityAmount += 200;//???
        object.densityXPos = object.mouseXPos;
        object.densityYPos = object.mouseYPos;
        object.densityZPos = object.mouseZPos;
    } else {
        object.densityAmount = 0;
        object.densityXPos = object.mouseXPos;
        object.densityYPos = object.mouseYPos;
    }

    object.mouseXPos0 = object.mouseXPos;
    object.mouseYPos0 = object.mouseYPos;
    object.mouseZPos0 = object.mouseZPos;

    return;
}

static void mouse_click_callback(GLFWwindow* w, int button, int action, int mods) {
    if ( action == GLFW_PRESS ) {
        program.mouse_click = true;
        object.densityAmount += 5; //Some arbitrary number

    } else if ( action == GLFW_RELEASE ) {
        program.mouse_click = false;
        object.densityAmount += 100;
        std::cout << "CLICKITY CLICK\n";
    }

    return;
}

void initUpdateObject() {
    object.densityXPos      = 0;
    object.densityYPos      = 0;
    object.densityZPos      = 0;
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
    object.mouseZPos        = 1;
    object.mouseZPos0       = 1;

    program.attribute_vertex = 0; // References location of attrib in shader
    program.attribute_density = 0;
    program.attribute_velocity_x = 0;
    program.attribute_velocity_y = 0;
    program.attribute_velocity_z = 0;
    program.uniform_size = 0;
}

/*void setParams(radius) {
    if (object.velocityXPos < 1)
}*/
