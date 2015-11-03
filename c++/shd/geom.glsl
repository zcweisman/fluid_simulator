#version 330

layout(points) in;
layout(points, max_vertices = 2) out;

in vec3 velocity[];
in float density[];

out vec3 vVel;
out float vDens;

void main() {
    vVel = velocity[0];
    vDens = density[0];

    gl_Position = gl_in[0].gl_Position + vec4(0.000001, 0.0, 0.0, 1.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(-0.000001, 0.0, 0.0, 1.0);
    EmitVertex();

    EndPrimitive();
}
