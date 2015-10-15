/*
 * Zachary Weisman
 * GLSL.hpp
 * A namespace for GL-task related functions
 */

#ifndef _GLSL_HPP_
#define _GLSL_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string.h>
#include "glm/glm.hpp"
#include "GLSL.h"

namespace GLSL {
    GLuint  loadShaders( const std::string, const std::string ); //Done
    GLuint  initShaderVars();                           //Done
    char*   textFileRead( const char* );                //Done
    int     initVBO();
    void    draw();
}

//Done
GLuint GLSL::loadShaders( const std::string &vShader, const std::string &fShader ) {
    fprintf(stderr, "Installing shaders...\n");
    GLuint prog, vao;
    GLint rc;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint VS = glCreateShader(GL_VERTEX_SHADER);
    GLuint FS = glCreateShader(GL_FRAGMENT_SHADER);

    const char* vshader = textFileRead(vShader.c_str());
    const char* fshader = textFileRead(fShader.c_str());

    glShaderSource(VS, 1, &vshader, NULL);
    glShaderSource(FS, 1, &fshader, NULL);

    glCompileShader(VS);
    glGetShaderiv(VS, GL_COMPILE_STATUS, &rc);
    if ( !rc ) {
        printf("Error compiling vertex shader %s\n", vShader.c_str());
        return false;
    }

    glCompileShader(FS);
    glGetShaderiv(FS, GL_COMPILE_STATUS, &rc);
    if ( !rc ) {
        printf("Error compiling fragment shader %s\n", fShader.c_str());
        return false;
    }

    prog = glCreateProgram();
    glAttachShader(prog, VS);
    glAttachShader(prog, FS);
    glLinkProgram(prog);
    glGetProgramiv(prog, GL_LINK_STATUS, &rc);

    if ( !rc ) {
        printf("Error linking shaders %s and %s\n", vShader.c_str(), fShader.c_str());
        return false;
    }

    glUseProgram(prog);
    assert(glGetError() == GL_NO_ERROR);
    program.program = prog;

    fprintf(stderr, "Successfully installed shaders\n");
    return prog;
}

//Done
GLuint GLSL::initShaderVars() {
    program.attribute_vertex = glGetAttribLocation(
        program.program, "vertex_position"
    );
    program.attribute_density = glGetAttribLocation(
        program.program, "vertex_density"
    );
    program.attribute_velocity = glGetAttribLocation(
        program.program, "vertex_velocity"
    );

}

char* GLSL::textFileRead( const char* fn ) {
    FILE *fp;
    char *content = NULL;
    int count = 0;
    if(fn != NULL) {
        fp = fopen(fn,"rt");
        if(fp != NULL) {
            fseek(fp, 0, SEEK_END);
            count = (int)ftell(fp);
            rewind(fp);
            if(count > 0) {
                content = (char *)malloc(sizeof(char) * (count+1));
                count = (int)fread(content,sizeof(char),count,fp);
                content[count] = '\0';
            }
            fclose(fp);
        } else {
            printf("error loading %s\n", fn);
        }
    }
    return content;
}

int GLSL::initVBO() {
    int i, j, count = 0;

    for ( j = 0; j < FLUIDSIZE; j++ ) {
        for ( i = 0; i < FLUIDSIZE; i++, count++ ) {
                program.vertex_array[count].x = (i+1-0.5)/FLUIDSIZE;
                program.vertex_array[count].y = (j+1-0.5)/FLUIDSIZE;
                program.index_array[count] = count; //Simply references the current
                                                    //index to handle in shaders
        }
    }

    glGenBuffers( 1, &(program.vbo) );
    glBindBuffer( GL_ARRAY_BUFFER, program.vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( Vector2D )*FLUIDSIZE*FLUIDSIZE,
     program.vertex_array, GL_STATIC_DRAW );

    glGenBuffers( 1, &(program.velbo) );
    glBindBuffer( GL_ARRAY_BUFFER, program.velbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( Vector2D )*FLUIDSIZE*FLUIDSIZE,
     program.velocity_array, GL_STATIC_DRAW );

    glGenBuffers( 1, &(program.dbo) );
    glBindBuffer( GL_ARRAY_BUFFER, program.dbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( float )*FLUIDSIZE*FLUIDSIZE,
     program.density_array )

    glGenBuffers( 1, &(program.ibo) );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, program.ibo );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, FLUIDSIZE*FLUIDSIZE*sizeof(GLuint),
     program.index_array, GL_STATIC_DRAW );
}

void GLSL::draw() {
    glEnableVertexAttribArray( program.attribute_reference );
    glBindBuffer( GL_ARRAY_BUFFER, program.vbo );
    glVertexAttribPointer( program.attribute_reference, 2, GL_FLOAT, GL_FALSE, 0, 0 );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, program.ibo );
    //Transformations here
    glDrawElements( GL_POINTS, FLUIDSIZE*FLUIDSIZE, GL_UNSIGNED_INT, 0 );
}


#endif
