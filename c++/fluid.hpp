/*
 * Zachary Weisman
 * fluid.hpp
 * Contains functions for manipulating the fluid space
 * Credit to Jos Stam for the algorithm
 * Credit: http://www.dgp.toronto.edu/people/stam/reality/Research/pdf/GDC03.pdf
 */

#ifndef _FLUID_HPP_
#define _FLUID_HPP_

#include <math.h>
#define N 8

#define SWAP( x0, x ) { float* tmp=x0; x0=x; x=tmp; }
#define IX2D( x, y ) ( (x) + (N+2) * y )
#define IX3D( x, y, z ) ( (x) + (y) * (N+2) + (z) * (N+2) * (N+2) )

class Fluid {

    float *vx, *vy, *vx0, *vy0, *dens, *densPrev;
    float diffusion, viscosity, dt;
    short fieldDimension, fieldArraySize, windowWidth, windowHeight;
    char iterations;

    void updateVelocity(float*, float*, float*, float*); //Done
    void updateDensity(float*, float*, float*, float*); //Done
    void diffuse(int, float*, float*); //Done
    void advect(int, float*, float*, float*, float*); //Done
    void project(float*, float*, float*);
    void setBnd(int, float*); //Done
    void addSource(float*, float*); //Done
    void project( float*, float*, float*, float* );

public:
    Fluid( short, short );  //Constructor
    ~Fluid();               //Destructor
    void    update();
    void    setViscosity(float); //Done
    void    setDiffuse(float);  //Done
    void    setTimeStep(float); //Done
    float*  getDensity(); //Done
    float*  getXVelocity(); //Done
    float*  getYVelocity(); //Done
    void    addDensity(float, int, int); //Done
    void    addVelocity(float, float, int, int); //Done
    void    setIterations(char);
};

Fluid::Fluid(short field, short window) {
    fieldDimension = field;
    fieldArraySize = pow(fieldDimension+2, 2);
    windowWidth = windowHeight = window;

    vx          = (float*)calloc( fieldArraySize, sizeof(float) );
    vy          = (float*)calloc( fieldArraySize, sizeof(float) );
    vx0         = (float*)calloc( fieldArraySize, sizeof(float) );
    vy0         = (float*)calloc( fieldArraySize, sizeof(float) );
    dens        = (float*)calloc( fieldArraySize, sizeof(float) );
    densPrev    = (float*)calloc( fieldArraySize, sizeof(float) );
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
    updateVelocity(vx, vy, vx0, vy0);
    updateDensity(dens, densPrev, vx, vy);
}

void Fluid::updateDensity(float* x, float* x0, float* u, float* v) {
    addSource( x, x0 );
    SWAP( x0, x );
    diffuse( 0, x, x0 );
    SWAP( x0, x );
    advect( 0, x, x0, u, v );
}

void Fluid::updateVelocity(float* u, float* v, float* u0, float* v0) {
    addSource(u, u0);
    addSource(v, v0);
    SWAP(u0, u);
    diffuse(1, u, u0);
    SWAP(v0, v);
    diffuse(2, v, v0);
    project(u, v, u0, v0);
    SWAP(u0, u);
    SWAP(v0, v);
    advect(1, u, u0, u0, v0);
    advect(2, v, v0, u0, v0);
    project(u, v, u0, v0);
}

void Fluid::diffuse(int b, float* x, float* x0) {
    unsigned int i, j, k;
    float a = dt*diffusion*fieldDimension*fieldDimension;
    for ( k = 0; k < iterations; k++ ) {
        for ( j = 1; j <= fieldDimension; j++ ) {
            for ( i = 1; i <= fieldDimension; i++ )
                x[IX2D(i,j)] = (x0[IX2D(i,j)] + a * (
                    x[IX2D(i-1,j)] +
                    x[IX2D(i+1,j)] +
                    x[IX2D(i,j-1)] +
                    x[IX2D(i,j+1)])
                )/(1+4*a);
        }
        setBnd(b,x);
    }
}

void Fluid::advect(int b, float* d, float* d0, float* u, float* v) {
    int i, j, i0, j0, i1, j1;
    float x, y, s0, t0, s1, t1, dt0;

    dt0 = dt*fieldDimension;
    for ( i = 1; i <= fieldDimension; i++) {
        for ( j = 1; j <= fieldDimension; j++ ) {
            x = i-dt0*u[IX2D(i,j)];
            y = j-dt0*v[IX2D(i,j)];

            if ( x < 0.5 ) x = 0.5; // Set x's floor to 0.5
            if ( x > fieldDimension+0.5 ) x = fieldDimension+0.5; //Limit x to N+0.5
            i0 = (int)x;
            i1 = i0+1;

            if ( y < 0.5 ) y = 0.5; // Set y's floor to 0.5
            if ( y > fieldDimension+0.5 ) y = fieldDimension+0.5; // Limit y to N+0.5
            j0 = (int)y;
            j1 = j0+1;

            s1 = x-i0;
            s0 = 1-s1;
            t1 = y-j0;
            t0 = 1-t1;

            d[IX2D(i,j)] = s0*(t0*d0[IX2D(i0,j0)] +
                            t1*d0[IX2D(i0,j1)]) +
                            s1*(t0*d0[IX2D(i1,j0)] +
                            t1*d0[IX2D(i1,j1)]);
        }
    }

    setBnd(b,d);
}

void Fluid::setBnd(int b, float* x) {
    int i;

    for ( i = 1; i <= fieldDimension; i++ ) {
        x[IX2D(0, i)] = b == 1 ? -x[IX2D(1, i)] : x[IX2D(1, i)];
        x[IX2D(fieldDimension+1, i)] = b ==1 ? -x[IX2D(fieldDimension, i)] :
            x[IX2D(fieldDimension, i)];
        x[IX2D(i, 0)] = b == 2 ? -x[IX2D(i, 1)] : x[IX2D(i, 1)];
        x[IX2D(i, fieldDimension+1)] = b == 2 ? -x[IX2D(i, fieldDimension)] :
            x[IX2D(i, fieldDimension)];
    }

    x[IX2D(0, 0)] = 0.5*(x[IX2D(1, 0)] + x[IX2D(0, 1)]);
    x[IX2D(0, fieldDimension+1)] = 0.5*(x[IX2D(1, fieldDimension+1)] +
        x[IX2D(0, fieldDimension)]);
    x[IX2D(fieldDimension+1, 0)] = 0.5*(x[IX2D(fieldDimension, 0)] +
        x[IX2D(fieldDimension+1, 1)]);
    x[IX2D(fieldDimension+1, fieldDimension+1)] = 0.5*(x[fieldDimension, fieldDimension+1] +
        x[IX2D(fieldDimension+1, fieldDimension)]);
}

void Fluid::addSource(float* a, float* b) {
    unsigned int i;
    for ( i = 0; i < fieldArraySize; i++ ) a[i] += dt * b[i];
}

void Fluid::project( float* u, float* v, float* p, float* div ) {
    int i, j, k;
    float h;
    h = 1.0/N;

    for ( i=1 ; i<=N ; i++ ) {
        for ( j=1 ; j<=N ; j++ ) {
            div[IX2D(i,j)] = -0.5*h*(u[IX2D(i+1,j)]-u[IX2D(i-1,j)]+
                v[IX2D(i,j+1)]-v[IX2D(i,j-1)]);
            p[IX2D(i,j)] = 0;
        }
    }

    setBnd ( 0, div );
    setBnd ( 0, p );

    for ( k=0 ; k<iterations ; k++ ) {
        for ( i=1 ; i<=N ; i++ ) {
            for ( j=1 ; j<=N ; j++ ) {
                p[IX2D(i,j)] = (div[IX2D(i,j)]+p[IX2D(i-1,j)]+p[IX2D(i+1,j)]+
                    p[IX2D(i,j-1)]+p[IX2D(i,j+1)])/4;
            }
        }
        setBnd ( 0, p );
    }

    for ( i=1 ; i<=N ; i++ ) {
        for ( j=1 ; j<=N ; j++ ) {
            u[IX2D(i,j)] -= 0.5*(p[IX2D(i+1,j)]-p[IX2D(i-1,j)])/h;
            v[IX2D(i,j)] -= 0.5*(p[IX2D(i,j+1)]-p[IX2D(i,j-1)])/h;
        }
    }

    setBnd ( 1, u );
    setBnd ( 2, v );
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
    diffusion = d;
}

void Fluid::setTimeStep( float t ) {
    dt = t;
}

void Fluid::setIterations( char i ) {
    iterations = i;
}

void Fluid::addDensity(float amount, int x, int y) {
    densPrev[IX2D(x, y)] += amount;
}

void Fluid::addVelocity(float amountX, float amountY, int x, int y) {
    int index = IX2D(x, y);
    vx0[index] += amountX;
    vy0[index] += amountY;
}

#endif
