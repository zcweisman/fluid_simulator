#version 330

in vec3     vVel;
in float    vDens;

uniform vec3 color;
uniform int color_option;

out vec4    fragcolor;

void main () {
    vec3 x;

    vec3 tmp = normalize(abs(vVel));
    //vec3(vDens/25.0, (1-tmp.y-(vDens/255.0)), (tmp.y-(vDens/255.0)) );
    vec2 r = normalize(vec2(vDens, vVel.x));
    vec2 g = normalize(vec2(vDens, vVel.y));
    vec2 b = normalize(vec2(vDens, vVel.z));
    if (color_option == 1)
        x = color;
    if (color_option == 2)
        x = vec3(tmp.x, tmp.y, tmp.z);
    if (color_option == 3)
        x = vec3(1.0) - tmp;

    if (vDens/25.0 > 0.25) fragcolor = vec4(x, vDens/25.0);
    //fragcolor = vec4( x, vDens/255.0 );
    else discard;
}   
