uniform sampler2D frame;
out vec4 fragcolor;

vec2 poissonFilter[16] = vec2[](
   vec2( -0.94201624, -0.39906216 ),
   vec2( 0.94558609, -0.76890725 ),
   vec2( -0.094184101, -0.92938870 ),
   vec2( 0.34495938, 0.29387760 ),
   vec2( -0.91588581, 0.45771432 ),
   vec2( -0.81544232, -0.87912464 ),
   vec2( -0.38277543, 0.27676845 ),
   vec2( 0.97484398, 0.75648379 ),
   vec2( 0.44323325, -0.97511554 ),
   vec2( 0.53742981, -0.47373420 ),
   vec2( -0.26496911, -0.41893023 ),
   vec2( 0.79197514, 0.19090188 ),
   vec2( -0.24188840, 0.99706507 ),
   vec2( -0.81409955, 0.91437590 ),
   vec2( 0.19984126, 0.78641367 ),
   vec2( 0.14383161, -0.14100790 )
);

void main() {
    float lightFactor = 1.0;
    float bias = 0.005;
    for (int i = 0; i < 16; i++)
        lightFactor -= 0.2*(1.0-texture(frame, vec3(gl_FragCoord.xy + poissonFilter[i]/700.0, (gl_FragCoord.z-bias)/gl_FragCoord.w)));

    fragcolor = lightFactor*texture(frame, gl_FragCoord.xy);
}
