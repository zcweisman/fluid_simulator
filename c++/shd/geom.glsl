#version 330

layout(points) in;
layout(points, max_vertices = 6) out;

in vec3 velocity[];
in float density[];

uniform mat4 proj_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

out vec3 vVel;
out float vDens;

const float PI = 3.1415926;

void main() {
    vec4 tempPos;

    vVel = velocity[0];
    vDens = density[0];
    gl_PointSize = 4;
//------ old constant was 0.000001 ------
    tempPos = gl_in[0].gl_Position + vec4(0.0000000001, 0.0, 0.0, 1.0);
    gl_Position = proj_matrix * view_matrix * model_matrix * tempPos;
    EmitVertex();

    tempPos = gl_in[0].gl_Position + vec4(-0.0000000001, 0.0, 0.0, 1.0);
    gl_Position = proj_matrix * view_matrix * model_matrix * tempPos;
    EmitVertex();

    tempPos = gl_in[0].gl_Position + vec4(0.0, 0.0000000001, 0.0, 1.0);
    gl_Position = proj_matrix * view_matrix * model_matrix * tempPos;
    EmitVertex();

    tempPos = gl_in[0].gl_Position + vec4(0.0, -0.0000000001, 0.0, 1.0);
    gl_Position = proj_matrix * view_matrix * model_matrix * tempPos;
    EmitVertex();

    tempPos = gl_in[0].gl_Position + vec4(0.0, 0.0, 0.0000000001, 1.0);
    gl_Position = proj_matrix * view_matrix * model_matrix * tempPos;
    EmitVertex();

    tempPos = gl_in[0].gl_Position + vec4(0.0, 0.0, -0.00000001, 1.0);
    gl_Position = proj_matrix * view_matrix * model_matrix * tempPos;
    EmitVertex();

    EndPrimitive();
}
