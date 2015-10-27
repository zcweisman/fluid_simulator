#ifndef _STRUCTS_H_
#define _STRUCTS_H_

// Use only for programs with 1 set of shaders and 1 vbo
typedef struct Program {
    GLuint      program; //GL program parameter
    GLfloat     vertex_array[FLUIDSIZE*FLUIDSIZE*3];
    GLfloat     velocity_x_array[FLUIDSIZE*FLUIDSIZE];
    GLfloat     velocity_y_array[FLUIDSIZE*FLUIDSIZE];
    GLfloat     velocity_z_array[FLUIDSIZE*FLUIDSIZE*FLUIDSIZE];
    GLfloat     density_array[FLUIDSIZE*FLUIDSIZE];
    GLuint      index_array[FLUIDSIZE*FLUIDSIZE];
    bool        locked_index_array[FLUIDSIZE*FLUIDSIZE];
    GLuint      velxbo;
    GLuint      velybo;
    GLuint      velzbo;
    GLuint      dbo;
    GLuint      vbo;
    GLuint      ibo;
    GLint       attribute_vertex;
    GLint       attribute_density;
    GLint       attribute_velocity_x;
    GLint       attribute_velocity_y;
    GLint       attribute_velocity_z;
    GLint       uniform_size;
    bool        mouse_click;
} GLProgram;

typedef struct Object {
    int     densityXPos;
    int     densityYPos;
    float   densityAmount;

    float   velocityXAmount;
    float   velocityYAmount;
    float   velocityZAmount;
    int     velocityXPos;
    int     velocityYPos;
    int     velocityZPos;
    int     velocityXPos0;
    int     velocityYPos0;
    int     velocityZPos0;

    int     mouseXPos;
    int     mouseXPos0;
    int     mouseYPos;
    int     mouseYPos0;
} UpdateObject;

#endif
