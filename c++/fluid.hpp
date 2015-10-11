#ifndef _FLUID_HPP_
#define _FLUID_HPP_

#define SWAP( x0, x ) { float* tmp=x0; x0=x; x=tmp; }
#define IX2D( x, y ) ( (x) + (N+2) * y )
#define IX3D( x, y, z ) ( (x) + (y) * (N+2) + (z) * (N+2) * (N+2) )

class Fluid {
private:
    float *vx, *vy, *vx0, *vy0, *dens, *densPrev;
    float diffusion, viscosity, dt;
    short fieldDimension, fieldArraySize, windowWidth, windowHeight;
    char iterations;
    void updateVelocity();
    void updateDensity();
    void diffuse(int, float*, float*);
    void advect(int, float*, float*, float*, float*);
    void project();
    void setBnd(int, float*);
    void linSolve();
    void addSource(float*, float*);

public:
    Fluid( short, short );
    ~Fluid();
    void    update();
    void    setViscosity(float);
    void    setDiffuse(float);
    void    setTimeStep(float);
    float*  getDensity();
    float*  getXVelocity();
    float*  getYVelocity();
    void    addDensity(float, int, int);
    void    addVelocity(float, int, int);
};

Fluid::Fluid(short field, short window) {
    fieldDimension = field;
    fieldArraySize = Math.pow(fieldDimension+2, 2);
    windowWidth = windowHeight = window;

    vx          = calloc( fieldArraySize*sizeof(float) );
    vy          = calloc( fieldArraySize*sizeof(float) );
    vx0         = calloc( fieldArraySize*sizeof(float) );
    vy0         = calloc( fieldArraySize*sizeof(float) );
    dens        = calloc( fieldArraySize*sizeof(float) );
    densPrev    = calloc( fieldArraySize*sizeof(float) );
}

Fluid::~Fluid() {
    free(vx);
    free(vy);
    free(vx0);
    free(vy0);
    free(dens);
    free(densPrev);
}

void Fluid::update() {
    updateDensity();
    updateVelocity();
}

void Fluid::updateDensity() {

}

void Fluid::updateVelocity() {

}

void Field::diffuse(int b, float* x, float* x0) {
    unsigned int i, j, k;
    float a = dt*diffusion*fieldDimension*fieldDimension;
    for ( k = 0; k < iterations; k++ ) {
        for ( i = 1; i < fieldDimension; i++ ) {

        }
    }
}

void Fluid::addSource(float* a, float* b) {
    unsigned int i;
    for ( i = 0; i < fieldArraySize; i++ ) a[i] += dt * b[i];
}

float* Fluid::getDensity() {
    return dens;
}

float* Fluid::getXVelocity() {
    return vx;
}

float* Fluid::getYVelocity() {
    return vy;
}

void Fluid::setViscosity( float v ) {
    viscosity = v;
}

void Fluid::setDiffuse( float d ) {
    density = d;
}

void Fluid::setTimeStep( float t ) {
    dt = t;
}

void Fluid::setIterations( char i ) {
    iterations = i;
}

#endif
