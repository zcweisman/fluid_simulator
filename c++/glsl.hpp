/*
 * Zachary Weisman
 * GLSL.hpp
 * A namespace for GL-task related functions
 */

#ifndef _GLSL_HPP_
#define _GLSL_HPP_

namespace GLSL {
    char*   textFileRead(const char*);
    GLuint  loadShaders(const std::string&, const std::string&);
    GLuint  loadShadersGeom(const std::string&, const std::string&, const std::string&);
    void    initShaderVars();
    void    initVBO();
    void    bufferData(Fluid*);
    void    initArrays();
}

GLuint GLSL::loadShaders( const std::string &vertShader, const std::string &fragShader ) {
    fprintf(stderr, "Installing shaders...\n");
    GLuint prog, vao;
    GLint rc, err;

    glGenVertexArrays( 1, &vao );
    assert( glGetError() == GLEW_OK );
    glBindVertexArray( vao );
    assert( glGetError() == GLEW_OK );

    fprintf( stderr, "Creating shader programs..." );
    GLuint VS = glCreateShader(GL_VERTEX_SHADER);
    GLuint FS = glCreateShader(GL_FRAGMENT_SHADER);
    fprintf( stderr, "Completed\n" );

    fprintf( stderr, "Reading shader files..." );
    const char* vshader = textFileRead(vertShader.c_str());
    const char* fshader = textFileRead(fragShader.c_str());
    fprintf( stderr, "Completed\n" );

    glShaderSource(VS, 1, &vshader, NULL);
    glShaderSource(FS, 1, &fshader, NULL);

    glCompileShader(VS);
    glGetShaderiv(VS, GL_COMPILE_STATUS, &rc);
    glGetShaderiv( VS, GL_INFO_LOG_LENGTH, &err );

    if ( !rc ) {
        printf("Error compiling vertex shader %s\n", vertShader.c_str());
        if ( err > 1 ) {
            GLchar* log_string = new char[err + 1];
            glGetShaderInfoLog( VS, err, 0, log_string );
            std::cout << "Log found  \n" << log_string << std::endl;

            delete log_string;
        }
        return false;
    }

    glCompileShader(FS);
    glGetShaderiv(FS, GL_COMPILE_STATUS, &rc);
    glGetShaderiv( FS, GL_INFO_LOG_LENGTH, &err );

    if ( !rc ) {
        printf("Error compiling fragment shader %s\n", fragShader.c_str());
        if ( err > 1 ) {
            GLchar* log_string = new char[err + 1];
            glGetShaderInfoLog( FS, err, 0, log_string );
            std::cout << "Log found  \n" << log_string << std::endl;

            delete log_string;
        }
        return false;
    }

    prog = glCreateProgram();
    glAttachShader(prog, VS);
    glAttachShader(prog, FS);
    glBindFragDataLocation( prog, 0, "fragcolor" );
    glLinkProgram(prog);
    glGetProgramiv(prog, GL_LINK_STATUS, &rc);

    if ( !rc ) {
        printf("Error linking shaders %s and %s\n", vertShader.c_str(), fragShader.c_str());
        return false;
    }

    glUseProgram( prog );

    fprintf(stderr, "Successfully installed shaders\n");
    return prog;
}

GLuint GLSL::loadShadersGeom( const std::string &vertShader, const std::string &geomShader, const std::string &fragShader ) {
    fprintf(stderr, "Installing shaders...\n");
    GLuint prog, vao;
    GLint rc, err;

    glGenVertexArrays( 1, &vao );
    assert( glGetError() == GLEW_OK );
    glBindVertexArray( vao );
    assert( glGetError() == GLEW_OK );

    fprintf( stderr, "Creating shader programs..." );
    GLuint VS = glCreateShader(GL_VERTEX_SHADER);
    GLuint GS = glCreateShader(GL_GEOMETRY_SHADER);
    GLuint FS = glCreateShader(GL_FRAGMENT_SHADER);
    fprintf( stderr, "Completed\n" );

    fprintf( stderr, "Reading shader files..." );
    const char* vshader = textFileRead(vertShader.c_str());
    const char* gshader = textFileRead(geomShader.c_str());
    const char* fshader = textFileRead(fragShader.c_str());
    fprintf( stderr, "Completed\n" );

    glShaderSource(VS, 1, &vshader, NULL);
    glShaderSource(GS, 1, &gshader, NULL);
    glShaderSource(FS, 1, &fshader, NULL);

    glCompileShader(VS);
    glGetShaderiv(VS, GL_COMPILE_STATUS, &rc);
    glGetShaderiv( VS, GL_INFO_LOG_LENGTH, &err );

    if ( !rc ) {
        printf("Error compiling vertex shader %s\n", vertShader.c_str());
        if ( err > 1 ) {
            GLchar* log_string = new char[err + 1];
            glGetShaderInfoLog( VS, err, 0, log_string );
            std::cout << "Log found  \n" << log_string << std::endl;

            delete log_string;
        }
        return false;
    }

    glCompileShader(GS);
    glGetShaderiv(GS, GL_COMPILE_STATUS, &rc);
    glGetShaderiv( GS, GL_INFO_LOG_LENGTH, &err );

    if ( !rc ) {
        printf("Error compiling vertex shader %s\n", geomShader.c_str());
        if ( err > 1 ) {
            GLchar* log_string = new char[err + 1];
            glGetShaderInfoLog( GS, err, 0, log_string );
            std::cout << "Log found  \n" << log_string << std::endl;

            delete log_string;
        }
        return false;
    }

    glCompileShader(FS);
    glGetShaderiv(FS, GL_COMPILE_STATUS, &rc);
    glGetShaderiv( FS, GL_INFO_LOG_LENGTH, &err );

    if ( !rc ) {
        printf("Error compiling fragment shader %s\n", fragShader.c_str());
        if ( err > 1 ) {
            GLchar* log_string = new char[err + 1];
            glGetShaderInfoLog( FS, err, 0, log_string );
            std::cout << "Log found  \n" << log_string << std::endl;

            delete log_string;
        }
        return false;
    }

    prog = glCreateProgram();
    glAttachShader(prog, VS);
    glAttachShader(prog, GS);
    glAttachShader(prog, FS);
    glBindFragDataLocation( prog, 0, "fragcolor" );
    glLinkProgram(prog);
    glGetProgramiv(prog, GL_LINK_STATUS, &rc);

    if ( !rc ) {
        printf("Error linking shaders %s, %s, and %s\n", vertShader.c_str(),
         geomShader.c_str(), fragShader.c_str());
        return false;
    }

    glUseProgram( prog );

    fprintf(stderr, "Successfully installed shaders\n");
    return prog;
}

void GLSL::initShaderVars() {
    glUseProgram(program.program[FRAMEBUFFER]);
    program.attribute_vertex = glGetAttribLocation(
        program.program[FRAMEBUFFER], "vertex_position"
    );

    program.attribute_density = glGetAttribLocation(
        program.program[FRAMEBUFFER], "vertex_density"
    );

    program.attribute_velocity_x = glGetAttribLocation(
        program.program[FRAMEBUFFER], "vertex_velocity_x"
    );

    program.attribute_velocity_y = glGetAttribLocation(
        program.program[FRAMEBUFFER], "vertex_velocity_y"
    );
    program.attribute_velocity_z = glGetAttribLocation(
      program.program[FRAMEBUFFER], "vertex_velocity_z"
    );

    program.uniform_view_matrix = glGetUniformLocation(
      program.program[FRAMEBUFFER], "view_matrix"
    );
    program.uniform_proj_matrix = glGetUniformLocation(
      program.program[FRAMEBUFFER], "proj_matrix"
    );
    program.uniform_model_matrix = glGetUniformLocation(
      program.program[FRAMEBUFFER], "model_matrix"
    );
    program.uniform_size = glGetUniformLocation(
        program.program[FRAMEBUFFER], "field_dimension"
    );

    program.uniform_color = glGetUniformLocation(
        program.program[FRAMEBUFFER], "color"
    );

    program.uniform_shading_option = glGetUniformLocation(
        program.program[FRAMEBUFFER], "color_option"
    );

    program.uniform_pixel_size = glGetUniformLocation(
        program.program[FRAMEBUFFER], "pixel_size"
    );

    /*glUseProgram(program.program[BLUR]);

    program.attribute_tex_vertex = glGetAttribLocation(
        program.program[BLUR], "vertex_position"
    );

    program.uniform_sampler = glGetUniformLocation(
        program.program[BLUR], "frame"
    );*/
}

char* GLSL::textFileRead(const char* fn) {
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

void GLSL::initArrays() {
    GLuint count = 0;

    for (int k = 0; k < FLUIDSIZE; k++) {
        for (int j = 0; j < FLUIDSIZE; j++ ) {
            for (int i = 0; i < FLUIDSIZE; i++, count++ ) {
                program.vertex_array[count*3] = (GLfloat)(((float)i/FLUIDSIZE)*2.f - 1.f);
                program.vertex_array[count*3+1] = (GLfloat)(((float)j/FLUIDSIZE)*2.f - 1.f);
                program.vertex_array[count*3+2] = (GLfloat)(((float)k/FLUIDSIZE)*2.f - 1.f);
                program.index_array[count] = count; //Simply references the current
                program.density_array[count] = 0.f;
                program.velocity_x_array[count] = 0.f;
                program.velocity_y_array[count] = 0.f;
                program.velocity_z_array[count] = 0.f;
                program.locked_index_array[count] = false;
            }
        }
    }
}

void GLSL::initVBO() {
    glUseProgram(program.program[FRAMEBUFFER]);

    // Initialize vertex array
    glGenBuffers( 1, &(program.vbo) );
    std::cout << "VBO Address: " << program.vbo << std::endl;
    glBindBuffer( GL_ARRAY_BUFFER, program.vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( program.vertex_array ),
     program.vertex_array, GL_STATIC_DRAW );

    // Initialize density array
    glGenBuffers( 1, &(program.dbo) );
    std::cout << "DBO Address: " << program.dbo << std::endl;
    glBindBuffer( GL_ARRAY_BUFFER, program.dbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( program.density_array ),
     program.density_array, GL_STATIC_DRAW );

    glGenBuffers( 1, &(program.velxbo) );
    std::cout << "VELBO Address: " << program.velxbo << std::endl;
    glBindBuffer( GL_ARRAY_BUFFER, program.velxbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( program.velocity_x_array ),
     program.velocity_x_array, GL_STATIC_DRAW );

    glGenBuffers( 1, &(program.velybo) );
    std::cout << "VELBO Address: " << program.velybo << std::endl;
    glBindBuffer( GL_ARRAY_BUFFER, program.velybo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( program.velocity_y_array ),
        program.velocity_y_array, GL_STATIC_DRAW );

    glGenBuffers( 1, &(program.velzbo) );
    std::cout << "VELBO Address: " << program.velzbo << std::endl;
    glBindBuffer( GL_ARRAY_BUFFER, program.velzbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( program.velocity_z_array ),
        program.velocity_z_array, GL_STATIC_DRAW );

     // Initialize index array
    glGenBuffers( 1, &(program.ibo) );
    std::cout << "IBO Address: " << program.ibo << std::endl;
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, program.ibo );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( program.index_array ),
     program.index_array, GL_STATIC_DRAW );

    assert( glGetError() == GLEW_OK );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

    glUseProgram(program.program[BLUR]);
    glGenVertexArrays(1, &program.quad_vertex_array_id);
    glBindVertexArray(program.quad_vertex_array_id);

    glGenBuffers(1, &program.quad_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, program.quad_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(program.quad_vertex_array),
     program.quad_vertex_array, GL_STATIC_DRAW);
}

void GLSL::bufferData(Fluid* field) {
    float* vx = field->getXVelocity();
    float* vy = field->getYVelocity();
    float* vz = field->getZVelocity();
    float* d = field->getDensity();
    int i, j, k, count = 0, currentIndex;
    float highestx = 0, highesty = 0, highestz = 0, highestDensity = 0;

    for (k = 1; k <= FLUIDSIZE; k++) {
        for ( j = 1; j <= FLUIDSIZE; j++ ) {
            for ( i = 1; i <= FLUIDSIZE; i++, count++ ) {
                currentIndex = IX3D(i,j,k);
                program.velocity_x_array[count] = vx[currentIndex];
                program.velocity_y_array[count] = vy[currentIndex];
                program.velocity_z_array[count] = vz[currentIndex];
                program.density_array[count] = d[currentIndex];
                /*if (vy[currentIndex] > highestDensity) {
                    highestDensity = vy[currentIndex];
                    fprintf(stderr, "%f\n", highestDensity);
                }*/
            }
        }
    }

    glUseProgram(program.program[FRAMEBUFFER]);
    glBindBuffer( GL_ARRAY_BUFFER, program.dbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( program.density_array ),
     program.density_array, GL_STATIC_DRAW );

    glBindBuffer( GL_ARRAY_BUFFER, program.velxbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( program.velocity_x_array ),
     program.velocity_x_array, GL_STATIC_DRAW );

    glBindBuffer( GL_ARRAY_BUFFER, program.velybo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( program.velocity_y_array ),
        program.velocity_y_array, GL_STATIC_DRAW );

    glBindBuffer( GL_ARRAY_BUFFER, program.velzbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( program.velocity_z_array ),
        program.velocity_z_array, GL_STATIC_DRAW );
}

#endif
