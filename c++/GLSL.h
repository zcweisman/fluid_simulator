#ifndef _GLSL_H_
#define _GLSL_H_

// Use only for programs with 1 set of shaders and 1 vbo
typedef struct Program {
    GLuint program;
    glm::vec2 vertex_array[FLUIDSIZE];
    GLint vbo;
} GLProgram;

#endif
