#version 330

in vec3     vertex_position;
in float    vertex_density;
in float     vertex_velocity_x;
in float     vertex_velocity_y;

out vec3    vPos;
out vec3    vCol;
out float   vDens;
out vec2    vVel;

//uniform int field_dimension;

//float travel = 1.0/field_dimension;

void main() {
    vPos.x = vertex_position.x*2.0-1.0;
    vPos.y = vertex_position.y*2.0-1.0;

    vCol = vec3( 1.0, 0.0, 0.0 );
    /*vCol.r = 0.0;
    vCol.g = 0.0;
    vCol.b = vertex_density;*/
    vDens = vertex_density;
    vVel = vec2(vertex_velocity_x, vertex_velocity_y);

    gl_Position.x = vertex_position.x*2.0-1.0;
    gl_Position.y = vertex_position.y*2.0-1.0;
    gl_Position.z = vertex_position.z+1;
    //gl_Position.w = vertex_velocity.x/vertex_velocity.x + 1.0;
    gl_Position.w = 1.0;
    gl_PointSize = 3;
}
