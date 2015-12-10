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

#define FLUIDSIZE 60

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

#include "camera.hpp"
#include "glsl.hpp"
#include "environment.hpp"

int main(void) {
    const GLubyte* renderer;
    const GLubyte* version;
    int windowHeight, windowWidth;
    bool keys[128];

    initUpdateObject();

    srand( time( NULL ) );

    fprintf(stderr, "Initializeing GLFW...");
    if ( !glfwInit() ) exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_SAMPLES, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    windowHeight    = 1280;
    windowWidth     = 1280;

    window          = glfwCreateWindow(windowWidth, windowHeight,
                        "3D Navier-Stokes Simulator", NULL, NULL);

    if ( !window ) { glfwTerminate(); exit(EXIT_FAILURE); }

    glfwMakeContextCurrent(window);
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
    std::cout << "Normal error <" << glGetError() << "> please ignore\n\n";

    char* gpu = (char*)glGetString(GL_RENDERER);
    fprintf(stderr, "GPU being used: %s\n", gpu);

    glClearColor( 0.1, 0.45, 0.5, 1.0 );

    field = new Fluid(FLUIDSIZE, windowWidth);

    std::cout << "Installing shaders...";
    //program.program[BLUR] = GLSL::loadShaders("shd/poisson.vert", "shd/poisson.frag");
    program.program[FRAMEBUFFER] = GLSL::loadShadersGeom("shd/vert.glsl", "shd/geom.glsl", "shd/frag.glsl");
    std::cout << "Completed\n";

    //Here
    /*glGenFramebuffers(1, &program.frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, program.frameBuffer);
    glGenTextures(1, &program.texture);
    glBindTexture(GL_TEXTURE_2D, program.texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowHeight, windowWidth, 0,
        GL_RGB, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);*/
    //To here

    GLSL::initShaderVars();
    GLSL::initArrays();
    GLSL::initVBO();

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
    glm::mat4 scale;
    glm::mat4 translate;
    glm::mat4 rotate;
    glm::mat4 model;

    std::cout << "\n------ Simulator Controls ------\n";
    std::cout << "Press 'v' to add velocity to the system.\n";
    std::cout << "Press 'o' to empty the cube when it gets too full.\n";
    std::cout << "Press 'p' to stop emptyig the cube.\n";
    std::cout << "Press 'l' to toggle mobile spheres density insertion.\n";
    std::cout << "Use WSQE to move the spheres around.\n";

    std::cout << "\n------ Viewing Controls ------\n";
    std::cout << "Press '1' to view blue smoke shading.\n";
    std::cout << "Press '2' to view velocity magnitude shading.\n";
    std::cout << "Press '3' to view velocity magnitude negative shading.\n";
    std::cout << "Press 'a' to rotate the cube left.\n";
    std::cout << "Press 'd' to rotate the cube right.\n";
    std::cout << "Press '+' to zoom in.\n";
    std::cout << "Press '-' to zoom out.\n";


    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(program.program[FRAMEBUFFER]);

        //Camera::update();
        Environment::setParams(field);
        GLSL::bufferData(field);

        glEnableVertexAttribArray( program.attribute_vertex );
        glBindBuffer( GL_ARRAY_BUFFER, program.vbo );
        glVertexAttribPointer( program.attribute_vertex, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

        // Enable density array
        glEnableVertexAttribArray( program.attribute_density );
        glBindBuffer( GL_ARRAY_BUFFER, program.dbo );
        glVertexAttribPointer( program.attribute_density, 1, GL_FLOAT, GL_FALSE, 0, (void*)0 );

        // Enable velocity array
        glEnableVertexAttribArray( program.attribute_velocity_x );
        glBindBuffer( GL_ARRAY_BUFFER, program.velxbo );
        glVertexAttribPointer( program.attribute_velocity_x, 1, GL_FLOAT, GL_FALSE, 0, (void*)0 );

        // Enable velocity array
        glEnableVertexAttribArray( program.attribute_velocity_y );
        glBindBuffer( GL_ARRAY_BUFFER, program.velybo );
        glVertexAttribPointer( program.attribute_velocity_y, 1, GL_FLOAT, GL_FALSE, 0, (void*)0 );

        // Enable velocity array
        glEnableVertexAttribArray(program.attribute_velocity_z);
        glBindBuffer(GL_ARRAY_BUFFER, program.velzbo);
        glVertexAttribPointer(program.attribute_velocity_z, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);

        rotate = glm::rotate(glm::mat4(1.f), object.angle, glm::vec3(0.f, 1.f, 0.f));
        scale = glm::scale(glm::mat4(1.f), glm::vec3(object.zoom, object.zoom, object.zoom));
        translate = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -0.5f));
        model = translate*scale*rotate;

        //Bind the index array
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, program.ibo );
        glUniform1i( program.uniform_size, FLUIDSIZE );
        glUniformMatrix4fv(program.uniform_proj_matrix, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(program.uniform_view_matrix, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(program.uniform_model_matrix, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(program.uniform_shading_option, object.colorChoice);
        glUniform1i(program.uniform_pixel_size, object.pixelSize);
        glUniform3fv(program.uniform_color, 1, glm::value_ptr(object.color));
        //glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, program.texture, 0);
        //GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
        //glDrawBuffers(1, DrawBuffers);



        //glUseProgram(program.program[BLUR]);
        assert(glGetError() == GL_NO_ERROR);
        glDrawElements(GL_POINTS, FLUIDSIZE*FLUIDSIZE*FLUIDSIZE, GL_UNSIGNED_INT, 0);
        assert(glGetError() == GL_NO_ERROR);

        glDisableVertexAttribArray(program.attribute_vertex);
        glDisableVertexAttribArray(program.attribute_density);
        glDisableVertexAttribArray(program.attribute_velocity_x);
        glDisableVertexAttribArray(program.attribute_velocity_y);
        glDisableVertexAttribArray(program.attribute_velocity_z);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        //Here
        /*GLuint quad_vertexbuffer;
        glGenBuffers(1, &quad_vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(program.quad_vertex_array), program.quad_vertex_array, GL_STATIC_DRAW);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, windowWidth, windowHeight);
        glUseProgram(program.program[BLUR]);
        glDrawArrays(GL_TRIANGLES, 0, 6);*/
        //To here
        glUseProgram (0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteFramebuffers(1, &program.frameBuffer);
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
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_A) object.angle -= 0.25f;
        if (key == GLFW_KEY_D) object.angle += 0.25f;
        if (key == GLFW_KEY_W) {
            if (object.mouseZPos+1 <= FLUIDSIZE)
                object.mouseZPos++;
            object.sourceYOffset++;
        }
        if (key == GLFW_KEY_S) {
            if (object.mouseZPos-1 >=1)
                object.mouseZPos--;
            object.sourceYOffset--;
        }
        if (key == GLFW_KEY_Q) object.sourceXOffset--;
        if (key == GLFW_KEY_E) object.sourceXOffset++;
        if (key == GLFW_KEY_EQUAL) {
            object.zoom+=0.05;
            object.pixelSize++;
        }
        if (key == GLFW_KEY_MINUS) {
            object.zoom-0.05f>=0.0f ?
            object.zoom-=0.05 : object.zoom-=0.0f;
            object.pixelSize-1>=0 ?
            object.pixelSize-=1 : object.pixelSize-=0;
        }
        if (key == GLFW_KEY_1) object.colorChoice = 1;
        if (key == GLFW_KEY_2) object.colorChoice = 2;
        if (key == GLFW_KEY_3) object.colorChoice = 3;
        if (key == GLFW_KEY_O) object.permeability-0.05 >= 0 ?
            object.permeability-=0.05 : object.permeability-=0.0f;
        if (key == GLFW_KEY_P) object.permeability+=0.05;// >= 1 ?
        if (key == GLFW_KEY_V) object.addVelocity = !object.addVelocity;
        if (key == GLFW_KEY_L) object.densityLocation = !object.densityLocation;
    }
}

static void error_callback(int error, const char* description) {
    fputs(description, stderr);
}

static void cursor_position_callback( GLFWwindow* w, double x, double y ) {
    double realY = 1023 - y;
    object.mouseXPos = (int)((x/(1024-1))*FLUIDSIZE - 0.5f);
    object.mouseYPos = (int)((realY/(1024-1))*FLUIDSIZE - 0.5f);

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
    object.sourceXOffset    = 0;
    object.sourceYOffset    = 0;
    object.angle            = 0.f;
    object.zoom             = 0.25f;
    object.iterations       = 10;
    object.diffusion        = 0.005;
    object.viscosity        = 0.005;
    object.permeability     = 1.0f;
    object.dt               = 0.1f;
    object.color            = glm::vec3(0.5333f, 0.6509f, 1.0f);
    object.colorChoice      = 1;
    object.pixelSize        = 4;
    object.addVelocity      = false;
    object.densityLocation  = false;

    program.frameBuffer             = 0;
    program.attribute_vertex        = 0;
    program.attribute_density       = 0;
    program.attribute_velocity_x    = 0;
    program.attribute_velocity_y    = 0;
    program.attribute_velocity_z    = 0;
    program.uniform_size            = 0;
    program.uniform_color           = 0;
    program.uniform_shading_option  = 0;
    program.uniform_sampler         = 0;
    program.uniform_pixel_size      = 0;
}
