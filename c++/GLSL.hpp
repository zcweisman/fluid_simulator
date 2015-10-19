/*
 * Zachary Weisman
 * GLSL.hpp
 * A namespace for GL-task related functions
 */

#ifndef _GLSL_HPP_
#define _GLSL_HPP_

namespace GLSL {
    GLuint  loadShaders( const std::string&, const std::string& ); //Done
    void  initShaderVars();                           //Done
    char*   textFileRead( const char* );                //Done
    void    initVBO();
    void    draw();
}

//Done
GLuint GLSL::loadShaders( const std::string &vertShader, const std::string &fragShader ) {
    fprintf(stderr, "Installing shaders...\n");
    GLuint prog, vao;
    GLint rc;

    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

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
    if ( !rc ) {
        printf("Error compiling vertex shader %s\n", vertShader.c_str());
        /*GLint logsize = 0;
        glGetShaderiv(VS, GL_INFO_LOG_LENGTH, &logsize);
        if ( logsize > 1 ) {
            GLchar* log_string = new char[logsize + 1];
            glGetShaderInfoLog(VS, logsize, 0, log_string);
            fprintf(stderr, "Log found:\n%s", log_string );

            delete log_string;
        }*/
        return false;
    }

    glCompileShader(FS);
    glGetShaderiv(FS, GL_COMPILE_STATUS, &rc);

    if ( !rc ) {
        printf("Error compiling fragment shader %s\n", fragShader.c_str());
        /*GLint logsize = 0;
        glGetShaderiv(FS, GL_INFO_LOG_LENGTH, &logsize);
        if ( logsize > 1 ) {
            GLchar* log_string = new char[logsize + 1];
            glGetShaderInfoLog(FS, logsize, 0, log_string);
            fprintf(stderr, "Log found:\n%s", log_string );

            delete log_string;
        }*/
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

    program.program = prog;

    fprintf(stderr, "Successfully installed shaders\n");
    return prog;
}

//Done
void GLSL::initShaderVars() {
    program.attribute_vertex = glGetAttribLocation(
        program.program, "vertex_position"
    );
    /*std::cout << "GL ERROR: " << glGetError() << "\n";
    program.attribute_density = glGetAttribLocation(
        program.program, "vertex_density"
    );
    std::cout << "GL ERROR: " << glGetError() << "\n";*/
    program.attribute_velocity = glGetAttribLocation(
        program.program, "vertex_velocity"
    );
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

void GLSL::initVBO() {
    assert(glGetError() == GL_NO_ERROR);
    int i, j, count = 0;

    for ( j = 0; j < FLUIDSIZE; j++ ) {
        for ( i = 0; i < FLUIDSIZE; i++, count++ ) {
                program.vertex_array[count].x = (i+1-0.5)/FLUIDSIZE;
                program.vertex_array[count].y = (j+1-0.5)/FLUIDSIZE;
                program.index_array[count] = (GLuint)count; //Simply references the current
                program.density_array[count] = 0.0f;
                program.velocity_array[count].x = 0.0f;
                program.velocity_array[count].y = 0.0f;
                                                    //index to handle in shaders
        }
    }

    glGenBuffers( 1, &(program.vbo) );
    glBindBuffer( GL_ARRAY_BUFFER, program.vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( Vector2D )*FLUIDSIZE*FLUIDSIZE,
     program.vertex_array, GL_STATIC_DRAW );
assert(glGetError() == GL_NO_ERROR);
    glGenBuffers( 1, &(program.velbo) );
    glBindBuffer( GL_ARRAY_BUFFER, program.velbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( Vector2D )*FLUIDSIZE*FLUIDSIZE,
     program.velocity_array, GL_STATIC_DRAW );
assert(glGetError() == GL_NO_ERROR);

    glGenBuffers( 1, &(program.dbo) );
    glBindBuffer( GL_ARRAY_BUFFER, program.dbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( GLfloat )*FLUIDSIZE*FLUIDSIZE,
     program.density_array, GL_STATIC_DRAW );
assert(glGetError() == GL_NO_ERROR);

    glGenBuffers( 1, &(program.ibo) );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, program.ibo );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, FLUIDSIZE*FLUIDSIZE*sizeof(GLuint),
     program.index_array, GL_STATIC_DRAW );
    assert(glGetError() == GL_NO_ERROR);
    std::cout << "GL ERROR 1: " << glGetError() << "\n";
}

void GLSL::draw() {
    assert(glGetError() == GL_NO_ERROR);
    glEnableVertexAttribArray( program.attribute_vertex );
    glBindBuffer( GL_ARRAY_BUFFER, program.vbo );
    glVertexAttribPointer( program.attribute_vertex, 2, GL_FLOAT, GL_FALSE, 0, 0 );

    glEnableVertexAttribArray( program.attribute_density );
    glBindBuffer( GL_ARRAY_BUFFER, program.dbo );
    glVertexAttribPointer( program.attribute_density, 1, GL_FLOAT, GL_FALSE, 0, 0 );

    glEnableVertexAttribArray( program.attribute_velocity );
    glBindBuffer( GL_ARRAY_BUFFER, program.velbo );
    std::cout << "GL ERROR 2: " << glGetError() << "\n";
    glVertexAttribPointer( program.attribute_velocity, 2, GL_FLOAT, GL_FALSE, 0, 0 );
    std::cout << "GL ERROR 3: " << glGetError() << "\n";

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, program.ibo );
    std::cout << "GL ERROR 4: " << glGetError() << "\n";
    assert(glGetError() == GL_NO_ERROR);

    glUniform1i( program.uniform_size, FLUIDSIZE );
    //Transformations here

    assert(glGetError() == GL_NO_ERROR);
    glDrawElements( GL_POINTS, FLUIDSIZE*FLUIDSIZE, GL_UNSIGNED_INT, 0 );
    std::cout << "GL ERROR 5: " << glGetError() << "\n";
    assert(glGetError() == GL_NO_ERROR);

    glDisableVertexAttribArray( program.attribute_vertex );
    glDisableVertexAttribArray( program.attribute_density );
    glDisableVertexAttribArray( program.attribute_velocity );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    std::cout << "GL ERROR 6: " << glGetError() << "\n";
}

#endif
