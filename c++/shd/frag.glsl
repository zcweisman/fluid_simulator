#version 330

in vec3 vPos;
in vec3 vCol;
in float vDens;

//uniform int field_dimension;

out vec4 fragcolor;

void main () {
    /*if ( distance( gl_FragCoord.xy, vPos.xy ) < 0.05 ) {
        gl_FragColor = vec4( 118.0/255.0, 169.0/255.0, 1.0 );
    } else {
        gl_FragColor = vec4( 1.0, 156.0/255.0, 84.0/255.0 );
    }*/
    //if ( gl_FragCoord.x > 100 ) fragcolor = vec4( 1, 0.5, 0.2, 1.0 );

    vec3 x = vec3( vCol.x+vDens, vCol.y+vDens, vCol.z+vDens );

    //if ( distance( gl_FragCoord.xy, vPos.xy ) < 80 )
    fragcolor = vec4( x, 1.0 );
    //else fragcolor = vec4( 0.1, 0.3, 0.4, 1.0 );
}
