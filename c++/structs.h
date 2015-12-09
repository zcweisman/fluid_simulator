#ifndef _STRUCTS_H_
#define _STRUCTS_H_

enum {FRAMEBUFFER, BLUR, NUM_PROGRAMS};

// Use only for programs with 1 set of shaders and 1 vbo
typedef struct Program {
    // GL program reference
    GLuint      program[NUM_PROGRAMS];

    // Vertex arrays
    GLfloat quad_vertex_array[18] = {-1.0f, -1.0f, 0.0f,
                                        1.0f, -1.0f, 0.0f,
                                        -1.0f, 1.0f, 0.0f,
                                        -1.0f, 1.0f, 0.0f,
                                        1.0f, -1.0f, 0.0f,
                                        -1.0f, 1.0f, 0.0f
                                        };
    GLfloat     vertex_array[FLUIDSIZE*FLUIDSIZE*FLUIDSIZE*3];
    GLfloat     velocity_x_array[FLUIDSIZE*FLUIDSIZE*FLUIDSIZE];
    GLfloat     velocity_y_array[FLUIDSIZE*FLUIDSIZE*FLUIDSIZE];
    GLfloat     velocity_z_array[FLUIDSIZE*FLUIDSIZE*FLUIDSIZE];
    GLfloat     density_array[FLUIDSIZE*FLUIDSIZE*FLUIDSIZE];
    GLuint      index_array[FLUIDSIZE*FLUIDSIZE*FLUIDSIZE];
    bool        locked_index_array[FLUIDSIZE*FLUIDSIZE*FLUIDSIZE];

    // Buffer objects
    GLuint      quad_vertex_buffer;
    GLuint      quad_vertex_array_id;
    GLuint      frameBuffer;
    GLuint      texture;
    GLuint      fvbo;
    GLuint      velxbo;
    GLuint      velybo;
    GLuint      velzbo;
    GLuint      dbo;
    GLuint      vbo;
    GLuint      ibo;

    // Shader references
    GLint       attribute_tex_vertex;
    GLint       attribute_vertex;
    GLint       attribute_density;
    GLint       attribute_velocity_x;
    GLint       attribute_velocity_y;
    GLint       attribute_velocity_z;
    GLint       uniform_size;
    GLint       uniform_model_matrix;
    GLint       uniform_view_matrix;
    GLint       uniform_proj_matrix;
    GLint       uniform_sampler;
    GLint       uniform_color;
    GLint       uniform_shading_option;
    GLint       uniform_pixel_size;
    bool        mouse_click;
} GLProgram;

typedef struct Object {
    float   angle;
    float   zoom;
    float   dt;
    char    iterations;
    float   diffusion;
    float   viscosity;

    int     densityXPos;
    int     densityYPos;
    int     densityZPos;
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
    float   permeability;

    int     mouseXPos;
    int     mouseXPos0;
    int     mouseYPos;
    int     mouseYPos0;
    int     mouseZPos;
    int     mouseZPos0;

    int     sourceXOffset;
    int     sourceYOffset;

    glm::vec3 color;
    int     colorChoice;
    int     pixelSize;
} UpdateObject;

#endif
