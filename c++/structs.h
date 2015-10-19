#ifndef _STRUCTS_H_
#define _STRUCTS_H_

typedef struct Point2D {
    GLfloat x;
    GLfloat y;
} Vector2D;

typedef struct Point3D {
    GLfloat x;
    GLfloat y;
    GLfloat z;
} Vector3D;

// Use only for programs with 1 set of shaders and 1 vbo
typedef struct Program {
    GLuint      program; //GL program parameter
    Vector2D    vertex_array[FLUIDSIZE*FLUIDSIZE]; //Array of vertices to be rendered
    Vector2D    velocity_array[FLUIDSIZE*FLUIDSIZE];
    GLfloat     density_array[FLUIDSIZE*FLUIDSIZE];
    GLuint      index_array[FLUIDSIZE*FLUIDSIZE];  //Ordered vertex indices
    GLuint      velbo;
    GLuint      dbo;
    GLuint      vbo; //References the buffer object for the vertices
    GLuint      ibo; //References the buffer object for the indices
    GLuint      attribute_vertex; // References location of attrib in shader
    GLuint      attribute_density;
    GLuint      attribute_velocity;
    GLuint      uniform_size;
    bool        mouse_click;
} GLProgram;

typedef struct Object {
    float   densityXPos;
    float   densityYPos;
    float   densityAmount;
    float   velocityXAmount;
    float   velocityYAmount;
    float   velocityXPos;
    float   velocityYPos;
    double  mouseXPos;
    double  mouseYPos;
} UpdateObject;

#endif
