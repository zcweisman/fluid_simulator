/*
 * Zachary Weisman
 * GLSL.hpp
 * A namespace for GL-task related functions
 */

#ifndef _GLSL_HPP_
#define _GLSL_HPP_

#define IX2DNOBUF(x,y) ((x) + FLUIDSIZE*(y))

namespace GLSL {
    GLuint  loadShaders( const std::string&, const std::string& ); //Done
    void    initShaderVars();                           //Done
    char*   textFileRead( const char* );                //Done
    void    initVBO();
    void    bufferData(Fluid*);
    void    initArrays();
}

//Done - No GL Errors
GLuint GLSL::loadShaders( const std::string &vertShader, const std::string &fragShader ) {
    fprintf(stderr, "Installing shaders...\n");
    GLuint prog, vao;
    GLint rc, err;

    glGenVertexArrays( 1, &vao );
    assert( glGetError() == GLEW_OK );
    glBindVertexArray( vao );
    assert( glGetError() == GLEW_OK );

    fprintf( stderr, "Creating shader programs..." );
    GLuint VS = glCreateShader( GL_VERTEX_SHADER );
    GLuint FS = glCreateShader( GL_FRAGMENT_SHADER );
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

//Done - No GL Errors
void GLSL::initShaderVars() {
    glUseProgram( program.program );
    program.attribute_vertex = glGetAttribLocation(
        program.program, "vertex_position"
    );
    std::cout << "Vertex Attribute Shader Address: " << program.attribute_vertex
        << std::endl;

    program.attribute_density = glGetAttribLocation(
        program.program, "vertex_density"
    );
    std::cout << "Density Attribute Shader Address: " << program.attribute_density
        << std::endl;

    program.attribute_velocity_x = glGetAttribLocation(
        program.program, "vertex_velocity_x"
    );
    std::cout << "X Velocity Attribute Shader Address: " << program.attribute_velocity_x
        << std::endl;

    program.attribute_velocity_y = glGetAttribLocation(
        program.program, "vertex_velocity_y"
    );
    std::cout << "Y Velocity Attribute Shader Address: " << program.attribute_velocity_y
        << std::endl;
    program.uniform_size = glGetUniformLocation(
        program.program, "field_dimension"
    );
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

void GLSL::initArrays() {
    int i, j, k;
    GLuint count = 0;

    /*for ( k = 0.0f; k < FLUIDSIZE; k++ ) {
        ;
    }*/
    for ( j = 0.0f; j < FLUIDSIZE; j++ ) {
        for ( i = 0.0f; i < FLUIDSIZE; i++, count++ ) {
            program.vertex_array[count*3] = (GLfloat)((i+1.0f-0.5f)/FLUIDSIZE);
            program.vertex_array[count*3+1] = (GLfloat)((j+1.0f-0.5f)/FLUIDSIZE);
            program.vertex_array[count*3+2] = (GLfloat)0.0f;
            program.index_array[count] = count; //Simply references the current
            program.density_array[count] = 0.0f;
            program.velocity_x_array[count] = 0.0f;
            program.velocity_y_array[count] = 0.0f;
            program.velocity_z_array[count] = 0.0f;
            program.locked_index_array[count] = false;
        }
    }
}

void GLSL::initVBO() {
    glUseProgram( program.program );

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

     // Initialize index array
    glGenBuffers( 1, &(program.ibo) );
    std::cout << "IBO Address: " << program.ibo << std::endl;
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, program.ibo );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( program.index_array ),
     program.index_array, GL_STATIC_DRAW );

    assert( glGetError() == GLEW_OK );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

void GLSL::bufferData(Fluid* field) {
    float* vx = field->getXVelocity();
    float* vy = field->getYVelocity();
    float* d = field->getDensity();
    int i, j, count = 0;

    for ( j = 1; j <= FLUIDSIZE; j++ ) {
        for ( i = 1; i <= FLUIDSIZE; i++, count++ ) {
            program.velocity_x_array[count] = vx[IX2D(i,j)];
            program.velocity_y_array[count] = vy[IX2D(i,j)];
            program.density_array[count] = d[IX2D(i,j)];
        }
    }

    glBindBuffer( GL_ARRAY_BUFFER, program.dbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( program.density_array ),
     program.density_array, GL_STATIC_DRAW );

    glBindBuffer( GL_ARRAY_BUFFER, program.velxbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( program.velocity_x_array ),
     program.velocity_x_array, GL_STATIC_DRAW );

    glBindBuffer( GL_ARRAY_BUFFER, program.velybo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( program.velocity_y_array ),
        program.velocity_y_array, GL_STATIC_DRAW );
}

#endif
