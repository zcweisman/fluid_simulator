#ifndef _GLSL_H_
#define _GLSL_H_

typedef struct Point {
    GLfloat x;
    GLfloat y;
} Vector2D;

// Use only for programs with 1 set of shaders and 1 vbo
typedef struct Program {
    GLuint      program; //GL program parameter
    Vector2D    vertex_array[FLUIDSIZE*FLUIDSIZE]; //Array of vertices to be rendered
    Vector2D    velocity_array[FLUIDSIZE*FLUIDSIZE];
    float       density_array[FLUIDSIZE*FLUIDSIZE];
    GLuint      index_array[FLUIDSIZE*FLUIDSIZE];  //Ordered vertex indices
    GLint       velbo;
    GLint       dbo;
    GLint       vbo; //References the buffer object for the vertices
    GLint       ibo; //References the buffer object for the indices
    GLuint      attribute_vertex; // References location of attrib in shader
    GLuint      attribute_density;
    GLuint      attribute_velocity;
    bool        mouse_click;
} GLProgram;

#endif
