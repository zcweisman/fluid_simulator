#version 330

in vec3     vVel;
in float    vDens;

out vec4    fragcolor;

void main () {
    vec3 x;

    vec3 tmp = normalize(vVel);
    x = vec3(tmp.x, tmp.y, tmp.z*5);//vec3(vDens/25.0, (1-tmp.y-(vDens/255.0)), (tmp.y-(vDens/255.0)) );
    vec2 r = normalize(vec2(vDens, vVel.x));
    vec2 g = normalize(vec2(vDens, vVel.y));
    vec2 b = normalize(vec2(vDens, vVel.z));
    //x = vec3(r.y, g.y, b.y);

    if (vDens/25.0 > 0.05) fragcolor = vec4( x, 1.0 );
    //fragcolor = vec4( x, vDens/255.0 );
    else discard;
}
