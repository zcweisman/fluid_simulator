#ifndef _STRUCTS_H_
#define _STRUCTS_H_

// Use only for programs with 1 set of shaders and 1 vbo
typedef struct Program {
    // GL program reference
    GLuint      program;

    // Vertex arrays
    GLfloat     vertex_array[FLUIDSIZE*FLUIDSIZE*3];
    GLfloat     velocity_x_array[FLUIDSIZE*FLUIDSIZE*FLUIDSIZE];
    GLfloat     velocity_y_array[FLUIDSIZE*FLUIDSIZE*FLUIDSIZE];
    GLfloat     velocity_z_array[FLUIDSIZE*FLUIDSIZE*FLUIDSIZE];
    GLfloat     density_array[FLUIDSIZE*FLUIDSIZE*FLUIDSIZE];
    GLuint      index_array[FLUIDSIZE*FLUIDSIZE*FLUIDSIZE];
    bool        locked_index_array[FLUIDSIZE*FLUIDSIZE*FLUIDSIZE];

    // Buffer objects
    GLuint      velxbo;
    GLuint      velybo;
    GLuint      velzbo;
    GLuint      dbo;
    GLuint      vbo;
    GLuint      ibo;

    // Shader references
    GLint       attribute_vertex;
    GLint       attribute_density;
    GLint       attribute_velocity_x;
    GLint       attribute_velocity_y;
    GLint       attribute_velocity_z;
    GLint       uniform_size;
    GLint       uniform_model_matrix;
    GLint       uniform_view_matrix;
    GLint       uniform_proj_matrix;
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
