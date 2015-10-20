#ifndef _STRUCTS_H_
#define _STRUCTS_H_

// Use only for programs with 1 set of shaders and 1 vbo
typedef struct Program {
    GLuint      program; //GL program parameter
    GLfloat     vertex_array[FLUIDSIZE*FLUIDSIZE*3]; //Array of vertices to be rendered
    GLfloat     velocity_array[FLUIDSIZE*FLUIDSIZE*2];
    GLfloat     density_array[FLUIDSIZE*FLUIDSIZE];
    GLuint      index_array[FLUIDSIZE*FLUIDSIZE];  //Ordered vertex indices
    GLuint      velbo;
    GLuint      dbo;
    GLuint      vbo; //References the buffer object for the vertices
    GLuint      ibo; //References the buffer object for the indices
    GLint       attribute_vertex = 0; // References location of attrib in shader
    GLint       attribute_density = 0;
    GLint       attribute_velocity = 0;
    GLint       uniform_size = 0;
    bool        mouse_click;
} GLProgram;

typedef struct Object {
    int     densityXPos;
    int     densityYPos;
    float   densityAmount;
    float   velocityXAmount;
    float   velocityYAmount;
    int     velocityXPos;
    int     velocityYPos;
    int     velocityXPos0;
    int     velocityYPos0;
    int     mouseXPos;
    int     mouseXPos0;
    int     mouseYPos;
    int     mouseYPos0;
} UpdateObject;

#endif
