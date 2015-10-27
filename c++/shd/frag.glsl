#version 330

in vec3     vPos;
in vec3     vCol;
in vec2     vVel;
in float    vDens;

uniform int field_dimension;

out vec4    fragcolor;

void main () {
    vec3 tempPos = vec3( vPos.x/2.0 + 0.5, vPos.y/2.0+0.5, vPos.z/2.0+0.5 );
    vec3 x;
    bool flip = true;

    if (flip) {
        vec2 tmp = normalize(vVel);
        x = vec3( vDens/255.0, (1-tmp.y-(vDens/255.0)), (tmp.y-(vDens/255.0)) );
    } else {
        if ( vDens < -100000 ) {
            x.x = ( vDens/255.0 + vCol.x + vVel.x/255.0 + tempPos.x )/4.0;
            x.y = ( vDens/255.0 + vCol.y + vVel.y/255.0 + tempPos.y )/4.0;
            x.z = ( vDens/255.0 + vCol.z + tempPos.z )/3.0;
        } else {
            x = vec3( vDens/10, 0.0, 0.0 );
        }
    }

    fragcolor = vec4( x, 1.0 );
}
