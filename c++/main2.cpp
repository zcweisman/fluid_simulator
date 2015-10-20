#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define FLUIDSIZE 8
static GLFWwindow* window;

#include "structs.h"

GLuint loadShaders( const std::string&, const std::string& );
char* textFileRead( const char* );
GLuint program;

int main( void ) {
	printf("Everything's ok!\n");

	if ( !glfwInit() ) exit(EXIT_FAILURE);

    //glfwWindowHint( GLFW_SAMPLES, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	window          = glfwCreateWindow(512, 512, "Test Window", NULL, NULL);

	if ( !window ) { glfwTerminate(); exit(EXIT_FAILURE); }
	glfwMakeContextCurrent( window );
	glfwSwapInterval( 1 );

	glewExperimental = GL_TRUE;
	if ( glewInit() != GLEW_OK ) {
		fprintf( stderr, "GLEW failed to initialize" );
	}

	program = loadShaders( "shd/vert.glsl", "shd/frag.glsl" );


	glClearColor( 0.5, 0.1, 0.2, 1.0 );
	while ( !glfwWindowShouldClose( window ) ) {
		//do stuff
	}

	return 0;
}

GLuint loadShaders( const std::string &vertShader, const std::string &fragShader ) {
    fprintf(stderr, "Installing shaders...\n");
    GLuint prog, vao;
    GLint rc, err;

    glGenVertexArrays( 1, &vao );
    std::cout << "GL ERROR: " << glGetError() << "\n";
    glBindVertexArray( vao );
    std::cout << "GL ERROR: " << glGetError() << "\n";

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

    glCompileShader( VS );
    glGetShaderiv( VS, GL_COMPILE_STATUS, &rc );
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

    if ( !rc ) {
        printf("Error compiling fragment shader %s\n", fragShader.c_str());
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

char* textFileRead( const char* fn ) {
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
