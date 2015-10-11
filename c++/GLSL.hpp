#ifndef _GLSL_HPP_
#define _GLSL_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string.h>

namespace GLSL {
    GLuint  loadShaders( const string, const string );
    char*   textFileRead( const char* );
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
    return prog;
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

#endif
