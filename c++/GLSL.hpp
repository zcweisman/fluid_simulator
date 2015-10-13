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
    GLuint  loadShaders( const string, const string ); //Done
    GLuint  initShaderVars( GLuint );
    char*   textFileRead( const char* );                //Done
    int     initVBO( glm::vec2* );
    void    draw();
}

GLuint GLSL::loadShaders( const string &vShader, const string &fShader ) {
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
    GLProgram.program = prog;
    return prog;
}

GLuint GLSL::initShaderVars( GLuint prog ) {
    GLProgram.vbo = glGetAttribLocation( GLProgram.program, "vertex_position" );

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

int GLSL::initVBO( glm::vec2* array ) {
    int i, j, count = 0;

    for ( j = 0; j < FLUIDSIZE; j++ ) {
        for ( i = 0; i < FLUIDSIZE; i++, count++ )
                vertex_array[count] = glm::vec2(i+1, j+1);
    }

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * FLUIDSIZE*FLUIDSIZE,
     array, GL_STATIC_DRAW);
}

void GLSL::draw() {
    glEnableVertexAttribArray(/*Vertex position buffer object "aPos"*/);
    glBindBuffer(GL_ARRAY_BUFFER, /*pbo*/);
    glVertexAttribPointer()
}


#endif
