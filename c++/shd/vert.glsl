#version 330

in vec2     vertex_position;
in float    vertex_density;
in vec2     vertex_velocity;

out vec2    vPos;
out vec3    vCol;

uniform int field_dimension;

float travel = 1.0/field_dimension;

void main() {
    //final_position = vertex_position;
    //final
    vPos = vertex_position;
    vCol = vec3( 0.5, 0.5, 0.5 );
    gl_Position = vec4( vertex_position, -0.5, 1.0 );
}
