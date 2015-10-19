#version 330

in vec2 vPos;
in vec3 vCol;

uniform int field_dimension;

out vec4 fragcolor;

void main () {
    /*if ( distance( gl_FragCoord.xy, vPos.xy ) < 0.05 ) {
        gl_FragColor = vec4( 118.0/255.0, 169.0/255.0, 1.0 );
    } else {
        gl_FragColor = vec4( 1.0, 156.0/255.0, 84.0/255.0 );
    }*/
    fragcolor = vec4( vCol.x, 156.0/255.0, 84.0/255.0, vPos.x );
    //fragcolor = vec4( vPos.x/vPos.x, 0.0, 0.0. 0.0/vCol.x );
}
