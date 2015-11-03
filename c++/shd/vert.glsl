#version 330

in vec3     vertex_position;
in float    vertex_density;
in float    vertex_velocity_x;
in float    vertex_velocity_y;
in float    vertex_velocity_z;

out vec3    velocity;
out float   density;

uniform int field_dimension;
uniform mat4 proj_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

float norm2D(float x, float y) {
    return sqrt(x*x + y*y);
}

float norm3D(float x, float y, float z) {
  return sqrt(x*x + y*y + z*z);
}

void main() {
    float mag = norm3D(vertex_velocity_x, vertex_velocity_y, vertex_velocity_z);
    float x = (vertex_position.x*2.0-1.0)+((vertex_velocity_x/mag)/field_dimension)*2;
    float y = (vertex_position.y*2.0-1.0)+((vertex_velocity_y/mag)/field_dimension)*2;
    float z = (vertex_position.z*2.0-1.0)+((vertex_velocity_z/mag)/field_dimension)*2;

    vec4 vector = vec4(x, y, z, 1.0);
    vec4 pos = proj_matrix*view_matrix*model_matrix*vector;

    density = vertex_density;
    velocity = vec3(vertex_velocity_x, vertex_velocity_y, vertex_velocity_z);

    gl_Position = vec4(pos.xyz, 1.0);
}
