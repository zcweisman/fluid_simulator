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

#define IX2D( x, y ) ( (x) + ((FLUIDSIZE+2) * (y)) )
#define IX3D( x, y, z ) ( (x) + (y) * (FLUIDSIZE+2) + (z) * (FLUIDSIZE+2) * (FLUIDSIZE+2) )

class Fluid {

    float *vx, *vy, *vx0, *vy0, *dens, *s;
    float diffusion, viscosity, dt;
    short fieldDimension, fieldArraySize;
    char iterations;

    void diffuse(int, float*, float*, float); //Done
    void advect(int, float*, float*, float*, float*); //Done
    void setBnd(int, float*); //Done
    void project( float*, float*, float*, float* );
    void linSolve( int, float*, float*, float, float );

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
    void    printDensityArray();
    void    printVelocityArray();
};

Fluid::Fluid(short field, short window) {
    fieldDimension = field;
    fieldArraySize = pow(fieldDimension+2, 2);

    s           = (float*)calloc( fieldArraySize, sizeof(GLfloat) );
    vx          = (float*)calloc( fieldArraySize, sizeof(GLfloat) );
    vy          = (float*)calloc( fieldArraySize, sizeof(GLfloat) );
    vx0         = (float*)calloc( fieldArraySize, sizeof(GLfloat) );
    vy0         = (float*)calloc( fieldArraySize, sizeof(GLfloat) );
    dens        = (float*)calloc( fieldArraySize, sizeof(GLfloat) );
}

Fluid::~Fluid() {
    free(s);
    free(vx);
    free(vy);
    free(vx0);
    free(vy0);
    free(dens);
}

void Fluid::update() {
    diffuse ( 1, vx0, vx, viscosity );
    diffuse ( 2, vy0, vy, viscosity );

    project ( vx0, vy0, vx, vy );

    advect ( 2, vx, vx0, vx0, vy0 );
    advect ( 1, vy, vy0, vx0, vy0 );

    project ( vx, vy, vx0, vy0 );

    diffuse ( 0, s, dens, diffusion );
    advect ( 0, dens, s, vx, vy );
}

void Fluid::diffuse( int b, float* x, float* x0, float c ) {
    float a = dt*c*fieldDimension*fieldDimension;
    linSolve( b, x, x0, a, 1+4*a );
}

void Fluid::advect( int b, float* d, float* d0, float* u, float* v ) {
    float i0, i1, j0, j1;
    float dtx = dt*fieldDimension;
    float dty = dt*fieldDimension;

    float s0, s1, t0, t1, u0, u1, tmp1, tmp2, x, y;
    float ifloat, jfloat, n = fieldDimension+2.0f;

    int i, j;

    for ( j = 1, jfloat = 1.0f; j <= fieldDimension; j++, jfloat++ ) {
        for ( i = 1, ifloat = 1.0f; i <= fieldDimension; i++, ifloat++ ) {
            tmp1 = dtx*u[IX2D(i,j)];
            tmp2 = dty*v[IX2D(i,j)];
            x = ifloat-tmp1;
            y = jfloat-tmp2;

            if ( x < 0.5f ) x = 0.5f; // Set x's floor to 0.5
            if ( x > n+0.5f ) x = n+0.5f; //Limit x to N+0.5
            i0 = floorf(x);
            i1 = i0+1.0f;

            if ( y < 0.5f ) y = 0.5f; // Set y's floor to 0.5
            if ( y > n+0.5f ) y = n+0.5f; // Limit y to N+0.5
            j0 = floorf(y);
            j1 = j0+1.0f;

            s1 = x-i0;
            s0 = 1.0f-s1;
            t1 = y-j0;
            t0 = 1.0f-t1;

            int i0i = i0;
            int i1i = i1;
            int j0i = j0;
            int j1i = j1;

            d[IX2D(i,j)] = s0*(t0*d0[IX2D(i0i,j0i)] +
             t1*d0[IX2D(i0i,j1i)]) + s1*(t0*d0[IX2D(i1i,j0i)] +
                            t1*d0[IX2D(i1i,j1i)]);
        }
    }

    setBnd(b,d);
}

void Fluid::setBnd(int b, float* x) {
    int i, n = fieldDimension;

    for ( i = 1; i <= fieldDimension; i++ ) {
        x[IX2D(0,i)] = b == 1 ? -x[IX2D(1,i)] : x[IX2D(1,i)];
        x[IX2D(n+1,i)] = b == 1 ? -x[IX2D(n,i)] : x[IX2D(n,i)];
    }

    for ( i = 1; i <= fieldDimension; i++ ) {
        x[IX2D(i,0)] = b == 2 ? -x[IX2D(i,1)] : x[IX2D(i,1)];
        x[IX2D(i,n+1)] = b == 2 ? -x[IX2D(i,n)] : x[IX2D(i,n)];
    }

    x[IX2D(0, 0)] = 0.5f*(x[IX2D(1, 0)] + x[IX2D(0, 1)]);
    x[IX2D(0, n+1)] = 0.5f*(x[IX2D(1, n+1)] + x[IX2D(0, n)]);
    x[IX2D(n+1, 0)] = 0.5f*(x[IX2D(n, 0)] + x[IX2D(n+1, 1)]);
    x[IX2D(n+1, n+1)] = 0.5f*(x[IX2D(n, n+1)] + x[IX2D(n+1, n)]);
}

void Fluid::project( float* u, float* v, float* p, float* div ) {
    int i, j, k;
    float n = fieldDimension+2;
    float h = 1.0f/fieldDimension;

    for ( j=1 ; j<=FLUIDSIZE ; j++ ) {
        for ( i=1 ; i<=FLUIDSIZE ; i++ ) {
            div[IX2D(i,j)] = -0.5*(
                u[IX2D(i+1,j)]-
                u[IX2D(i-1,j)]+
                v[IX2D(i,j+1)]-
                v[IX2D(i,j-1)]
            ) / n;

            p[IX2D(i,j)] = 0;
        }
    }

    setBnd ( 0, div );
    setBnd ( 0, p );
    linSolve( 0, p, div, 1, 4 );

    for ( j=1 ; j<=FLUIDSIZE; j++ ) {
        for ( i=1 ; i<=FLUIDSIZE; i++ ) {
            u[IX2D(i,j)] -= 0.5*(p[IX2D(i+1,j)]-p[IX2D(i-1,j)])*n;
            v[IX2D(i,j)] -= 0.5*(p[IX2D(i,j+1)]-p[IX2D(i,j-1)])*n;
        }
    }

    setBnd ( 1, u );
    setBnd ( 2, v );
}

void Fluid::linSolve( int b, float* x, float* x0, float a, float c ) {
    float cRecip = 1.0 / c;
    int i, j;
    for (int k = 0; k < iterations; k++) {
        for (int j = 1; j <= fieldDimension; j++) {
            for (int i = 1; i <= fieldDimension; i++) {
                x[IX2D(i, j)] = (x0[IX2D(i, j)] +
                 a*(x[IX2D(i+1, j)] + x[IX2D(i-1, j)] +
                    x[IX2D(i, j+1)] + x[IX2D(i, j-1)])
                ) * cRecip;
            }
        }

        setBnd(b, x);
    }
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
    int index = IX2D(x,y);
    dens[index]+amount <= 255.0 ? dens[index] += amount : dens[index] = 255.0;
}

void Fluid::addVelocity(float amountX, float amountY, int x, int y) {
    int index = IX2D(x, y);

    vx[index]+amountX <= 255.0 ? vx[index] += amountX : vx[index] = 255.0;
    vy[index]+amountY <= 255.0 ? vy[index] += amountY : vy[index] = 255.0;
}

void Fluid::printDensityArray() {
    int i, j;
    for ( j = 1; j <= fieldDimension; j++ ) {
        for ( i = 1; i <= fieldDimension; i++ ) {
            std::cout << dens[ IX2D(i,j) ] << " ";
        }
        std::cout << std::endl;
    }
}

void Fluid::printVelocityArray() {
    int i, j;
    for ( j = 1; j <= fieldDimension; j++ ) {
        for ( i = 1; i <= fieldDimension; i++ ) {
            std::cout << "(" << vx[ IX2D(i,j) ] << ","
                << vy[ IX2D(i,j) ] << ") ";
        }
        std::cout << std::endl;
    }
}

#endif
