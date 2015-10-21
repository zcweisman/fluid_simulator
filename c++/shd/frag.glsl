#version 330

in vec3 vPos;
in vec3 vCol;
in float vDens;
in vec2 vVel;

//uniform int field_dimension;

out vec4 fragcolor;

void main () {
    /*if ( distance( gl_FragCoord.xy, vPos.xy ) < 0.05 ) {
        gl_FragColor = vec4( 118.0/255.0, 169.0/255.0, 1.0 );
    } else {
        gl_FragColor = vec4( 1.0, 156.0/255.0, 84.0/255.0 );
    }*/
    //if ( gl_FragCoord.x > 100 ) fragcolor = vec4( 1, 0.5, 0.2, 1.0 );

    //vec3 x = vec3( vCol.x+vDens, vCol.y+vDens, vCol.z+vDens );
    vec3 tempPos = vec3( vPos.x/2.0 + 0.5, vPos.y/2.0+0.5, vPos.z/2.0+0.5 );
    vec3 x;// = (vDens/255.0 + 0/vCol.y + 0/vVel.x)/3;//vDens/vCol;
    if ( vDens < -100 ) {
        x.x = ( vDens/255.0 + vCol.x + vVel.x/255.0 + tempPos.x )/4.0;
        x.y = ( vDens/255.0 + vCol.y + vVel.y/255.0 + tempPos.y )/4.0;
        x.z = ( vDens/255.0 + vCol.z + tempPos.z )/3.0;
    } else {
        x = vec3( vDens/10, 0.0, 0.0 );
    }

    //if ( distance( gl_FragCoord.xy, vPos.xy ) < 80 )
    fragcolor = vec4( x, 1.0 );
    //else fragcolor = vec4( 0.1, 0.3, 0.4, 1.0 );
}
