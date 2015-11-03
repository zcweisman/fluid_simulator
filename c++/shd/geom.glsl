#version 330

layout(points) in;
layout(points, max_vertices = 6) out;

in vec3 velocity[];
in float density[];

out vec3 vVel;
out float vDens;

const float PI = 3.1415926;

void main() {
    vVel = velocity[0];
    vDens = density[0];
    gl_PointSize = 2;

    gl_Position = gl_in[0].gl_Position + vec4(0.000001, 0.0, 0.0, 1.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(-0.000001, 0.0, 0.0, 1.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(0.0, 0.000001, 0.0, 1.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(0.0, -0.000001, 0.0, 1.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(0.0, 0.0, 0.000001, 1.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(0.0, 0.0, -0.000001, 1.0);
    EmitVertex();
    /*for (int i = 0; i <= 10; i++) {
        // Angle between each side in radians
        float ang = PI * 2.0 / 10.0 * i;

        // Offset from center of point (0.3 to accomodate for aspect ratio)
        vec4 offset = vec4(cos(ang) * 0.0003, -sin(ang) * 0.0004, 0.0, 0.0);
        gl_Position = gl_in[0].gl_Position + offset;

        EmitVertex();
    }*/

    //EndPrimitive();

    EndPrimitive();
}
