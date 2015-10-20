#version 330

in vec3     vertex_position;
in float    vertex_density;
in vec2     vertex_velocity;

out vec3    vPos;
out vec3    vCol;
out float   vDens;

//uniform int field_dimension;

//float travel = 1.0/field_dimension;

void main() {
    vPos.x = vertex_position.x*2.0-1.0;
    vPos.y = vertex_position.y*2.0-1.0;

    vCol = vertex_position;
    /*vCol.r = 0.0;
    vCol.g = 0.0;
    vCol.b = vertex_density;*/
    vDens = vertex_density;

    gl_Position.x = vertex_position.x*2.0-1.0;
    gl_Position.y = vertex_position.y*2.0-1.0;
    gl_Position.z = vertex_position.z+1;
    gl_Position.w = vertex_velocity.x/vertex_velocity.x;
}
