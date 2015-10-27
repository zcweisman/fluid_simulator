#version 330

in vec3     vertex_position;
in float    vertex_density;
in float    vertex_velocity_x;
in float    vertex_velocity_y;

out vec3    vPos;
out vec3    vCol;
out vec2    vVel;
out float   vDens;

uniform int field_dimension;

float norm(float x, float y) {
    float a = x*x;
    float b = y*y;
    return sqrt(a+b);
}

void main() {
    vPos.x = vertex_position.x*2.0-1.0;
    vPos.y = vertex_position.y*2.0-1.0;

    vCol = vec3( 1.0, 0.0, 0.0 );
    vDens = vertex_density;
    vVel = vec2(vertex_velocity_x, vertex_velocity_y);

    float mag = norm(vertex_velocity_x, vertex_velocity_y);

    gl_Position.x = (vertex_position.x*2.0-1.0)+((vertex_velocity_x/mag)/field_dimension)*2;
    gl_Position.y = (vertex_position.y*2.0-1.0)+((vertex_velocity_y/mag)/field_dimension)*2;
    gl_Position.z = vertex_position.z;
    gl_Position.w = 1.0;
    gl_PointSize = 4;
}
