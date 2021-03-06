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
#define IX3D( x, y, z ) ( (x) + ((y) * (FLUIDSIZE+2)) + ((z) * (FLUIDSIZE+2) * (FLUIDSIZE+2)) )

class Fluid {
    float *vx, *vy, *vz, *vx0, *vy0, *vz0, *dens, *s;
    float diffusion, viscosity, dt, pf;
    unsigned int fieldDimension, fieldArraySize;
    char iterations;

    void diffuse(int, float*, float*, float);
    void linSolve( int, float*, float*, float, float );
    void linSolve3D(int, float*, float*, float, float);
    void advect(int, float*, float*, float*, float*);
    void advect3D(int, float*, float*, float*, float*, float*);
    void project( float*, float*, float*, float* );
    void project3D(float*, float*, float*, float*, float*);
    void setBnd(int, float*);
    void setBnd3D(int, float*);

public:
    Fluid( short, short );
    ~Fluid();

    void    update();

    void    setViscosity(float);
    void    setDiffuse(float);
    void    setTimeStep(float);
    void    setIterations(char);
    void    setPermeability(float);

    void    addDensity(float, int, int, int);
    void    addVelocity(float, float, float, int, int, int);

    float*  getDensity();
    float*  getXVelocity();
    float*  getYVelocity();
    float*  getZVelocity();
};

Fluid::Fluid(short field, short window) {
    fieldDimension = field;
    fieldArraySize = pow(fieldDimension+2, 3);
    pf = 1.0f;

    s           = (float*)calloc( fieldArraySize, sizeof(GLfloat) );
    vx          = (float*)calloc( fieldArraySize, sizeof(GLfloat) );
    vy          = (float*)calloc( fieldArraySize, sizeof(GLfloat) );
    vz          = (float*)calloc( fieldArraySize, sizeof(GLfloat) );
    vx0         = (float*)calloc( fieldArraySize, sizeof(GLfloat) );
    vy0         = (float*)calloc( fieldArraySize, sizeof(GLfloat) );
    vz0         = (float*)calloc( fieldArraySize, sizeof(GLfloat) );
    dens        = (float*)calloc( fieldArraySize, sizeof(GLfloat) );
}

Fluid::~Fluid() {
    free(s);
    free(vx);
    free(vy);
    free(vz);
    free(vx0);
    free(vy0);
    free(vz0);
    free(dens);
}

void Fluid::update() {
    //fprintf(stderr, "\n--- Diffuse 1 ---\n");
    /*diffuse ( 1, vx0, vx, viscosity );
    //fprintf(stderr, "\n--- Diffuse 2 ---\n");
    diffuse ( 2, vy0, vy, viscosity );

    //fprintf(stderr, "\n--- Project 1 ---\n");
    project ( vx0, vy0, vx, vy );

    //fprintf(stderr, "\n--- Advect 1 ---\n");
    advect ( 1, vx, vx0, vx0, vy0 );
    //fprintf(stderr, "\n--- Advect 2 ---\n");
    advect ( 2, vy, vy0, vx0, vy0 );

    //fprintf(stderr, "\n--- Project 2 ---\n");
    project ( vx, vy, vx0, vy0 );

    //fprintf(stderr, "\n--- Diffuse 1 ---\n");
    diffuse ( 0, s, dens, diffusion );
    advect ( 0, dens, s, vx, vy );*/

    std::thread d1(&Fluid::diffuse, this, 1, vx0, vx, viscosity);
    std::thread d2(&Fluid::diffuse, this, 2, vy0, vy, viscosity);
    std::thread d3(&Fluid::diffuse, this, 3, vz0, vz, viscosity);
    //diffuse(1, vx0, vx, viscosity);
    //diffuse(2, vy0, vy, viscosity);
    //diffuse(3, vz0, vz, viscosity);
    d1.join();
    d2.join();
    d3.join();

    project3D (vx0, vy0, vz0, vx, vy);

    std::thread a1(&Fluid::advect3D, this, 1, vx, vx0, vx0, vy0, vz0);
    std::thread a2(&Fluid::advect3D, this, 2, vy, vy0, vx0, vy0, vz0);
    std::thread a3(&Fluid::advect3D, this, 3, vz, vz0, vx0, vy0, vz0);
    a1.join();
    a2.join();
    a3.join();
    /*advect3D(1, vx, vx0, vx0, vy0, vz0);
    advect3D(2, vy, vy0, vx0, vy0, vz0);
    advect3D(3, vz, vz0, vx0, vy0, vz0);*/

    project3D(vx, vy, vz, vx0, vy0);

    diffuse(0, s, dens, diffusion);
    advect3D(0, dens, s, vx, vy, vz);
}

void Fluid::diffuse( int b, float* x, float* x0, float c ) {
    float a = dt*c*fieldDimension*fieldDimension;
    //linSolve( b, x, x0, a, 1+4*a );
    linSolve3D(b, x, x0, a, 1+6*a);
}

void Fluid::linSolve( int b, float* x, float* x0, float a, float c ) {
    float cRecip = 1.0 / c;
    int i, j, currentIndex;
    for (int k = 0; k < iterations; k++) {
        for (int j = 1; j <= fieldDimension; j++) {
            for (int i = 1; i <= fieldDimension; i++) {
                currentIndex = IX3D(i,j,k);
                x[currentIndex] = (x0[currentIndex] +
                 a*(x[IX3D(i+1, j, 1)] + x[IX3D(i-1, j, 1)] +
                    x[IX3D(i, j+1, 1)] + x[IX3D(i, j-1, 1)])
                ) * cRecip;
            }
        }
        setBnd(b, x);
    }
}

void Fluid::linSolve3D(int b, float* x, float* x0, float a, float c) {
    float cRecip = 1.f / c;
    unsigned int currentIndex;
    static int count = 0;
    static float highest = 0.0f;

    for (int m = 0; m < iterations; m++) {
        for (int k = 1; k <= fieldDimension; k++) {
            for (int j = 1; j <= fieldDimension; j++) {
                for (int i = 1; i <= fieldDimension; i++) {
                    currentIndex = IX3D(i,j,k);
                        x[currentIndex] = (x0[currentIndex] +
                          a*(x[IX3D(i+1, j, k)]
                            + x[IX3D(i-1, j, k)]
                            + x[IX3D(i, j+1, k)]
                            + x[IX3D(i, j-1, k)]
                            + x[IX3D(i, j, k+1)]
                            + x[IX3D(i,j,k-1)])
                        ) * cRecip;
                    if (x[currentIndex] > highest) highest = x[currentIndex];
                }
            }
        }
        setBnd3D(b, x);
    }
}

void Fluid::advect( int b, float* d, float* d0, float* u, float* v ) {
    float i0, i1, j0, j1;
    float dtx = dt*fieldDimension;
    float dty = dt*fieldDimension;

    float s0, s1, t0, t1, u0, u1, tmp1, tmp2, x, y;
    float ifloat, jfloat, n = fieldDimension+2.0f;

    int i, j, currentIndex;

    for ( j = 1, jfloat = 1.0f; j <= fieldDimension; j++, jfloat++ ) {
        for ( i = 1, ifloat = 1.0f; i <= fieldDimension; i++, ifloat++ ) {
            currentIndex = IX3D(i,j,1);
            tmp1 = dtx*u[currentIndex];
            tmp2 = dty*v[currentIndex];
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

            if (i0i < 0) i0i = 0;
            if (i0i > FLUIDSIZE) i0i = FLUIDSIZE;
            if (i1i < 0) i1i = 0;
            if (i1i > FLUIDSIZE) i1i = FLUIDSIZE;

            if (j0i < 0) j0i = 0;
            if (j0i > FLUIDSIZE) j0i = FLUIDSIZE;
            if (j1i < 0) j1i = 0;
            if (j1i > FLUIDSIZE) j1i = FLUIDSIZE;

            d[currentIndex] = s0*(t0*d0[IX3D(i0i,j0i,1)] +
             t1*d0[IX3D(i0i,j1i,1)]) + s1*(t0*d0[IX3D(i1i,j0i,1)] +
                            t1*d0[IX3D(i1i,j1i,1)]);
        }
    }

    setBnd(b,d);
}

void Fluid::advect3D(int b, float* d, float* d0, float* u, float* v, float* w) {
    //fprintf(stderr, "Starting advect3D\n");
  float i0, i1, j0, j1, k0, k1;

  float dtx = dt * fieldDimension;
  float dty = dt * fieldDimension;
  float dtz = dt * fieldDimension;

  float s0, s1, t0, t1, u0, u1;
  float tmp1, tmp2, tmp3, x, y, z;

  float Nfloat = FLUIDSIZE+2.0f;
  float ifloat, jfloat, kfloat;
  int i, j, k, n = fieldDimension, currentIndex;

  for(k = 1, kfloat = 1; k <= n; k++, kfloat++) {
      for(j = 1, jfloat = 1; j <= n; j++, jfloat++) {
          for(i = 1, ifloat = 1; i <= n; i++, ifloat++) {
                currentIndex = IX3D(i,j,k);
                tmp1 = dtx * u[currentIndex];
                tmp2 = dty * v[currentIndex];
                tmp3 = dtz * w[currentIndex];
                x    = ifloat - tmp1;
                y    = jfloat - tmp2;
                z    = kfloat - tmp3;

                if(x < 0.5f)
                    x = 0.5f;
                if(x > Nfloat + 0.5f)
                    x = Nfloat + 0.5f;
                i0 = floorf(x);
                i1 = i0 + 1.0f;

                if(y < 0.5f)
                    y = 0.5f;
                if(y > Nfloat + 0.5f)
                    y = Nfloat + 0.5f;
                j0 = floorf(y);
                j1 = j0 + 1.0f;

                if(z < 0.5f)
                    z = 0.5f;
                if(z > Nfloat + 0.5f)
                    z = Nfloat + 0.5f;
                k0 = floorf(z);
                k1 = k0 + 1.0f;

                s1 = x - i0;
                s0 = 1.0f - s1;
                t1 = y - j0;
                t0 = 1.0f - t1;
                u1 = z - k0;
                u0 = 1.0f - u1;

                int i0i = i0;
                int i1i = i1;
                int j0i = j0;
                int j1i = j1;
                int k0i = k0;
                int k1i = k1;

                d[currentIndex] =
                    s0 * ( t0 * (u0 * d0[IX3D(i0i, j0i, k0i)]
                    +u1 * d0[IX3D(i0i, j0i, k1i)])
                    +( t1 * (u0 * d0[IX3D(i0i, j1i, k0i)]
                    +u1 * d0[IX3D(i0i, j1i, k1i)])))
                    +s1 * ( t0 * (u0 * d0[IX3D(i1i, j0i, k0i)]
                    +u1 * d0[IX3D(i1i, j0i, k1i)])
                    +( t1 * (u0 * d0[IX3D(i1i, j1i, k0i)]
                    +u1 * d0[IX3D(i1i, j1i, k1i)])));
            }
        }
    }
    setBnd3D(b, d);
}

void Fluid::project(float* u, float* v, float* p, float* div) {
    int i, j, k, currentIndex;
    float n = fieldDimension+2;
    float h = 1.0f/fieldDimension;

    for (j = 1; j <= FLUIDSIZE; j++) {
        for (i = 1 ;i <= FLUIDSIZE; i++) {
            currentIndex = IX3D(i,j,1);
            div[currentIndex] = -0.5*(
                u[IX3D(i+1,j,1)]-
                u[IX3D(i-1,j,1)]+
                v[IX3D(i,j+1,1)]-
                v[IX3D(i,j-1,1)]
            ) / n;

            p[currentIndex] = 0;
        }
    }

    setBnd ( 0, div );
    setBnd ( 0, p );
    linSolve( 0, p, div, 1, 4 );

    for ( j=1 ; j<=FLUIDSIZE; j++ ) {
        for ( i=1 ; i<=FLUIDSIZE; i++ ) {
            u[IX3D(i,j,1)] -= 0.5*(p[IX3D(i+1,j,1)]-p[IX3D(i-1,j,1)])*n;
            v[IX3D(i,j,1)] -= 0.5*(p[IX3D(i,j+1,1)]-p[IX3D(i,j-1,1)])*n;
        }
    }

    setBnd ( 1, u );
    setBnd ( 2, v );
}

void Fluid::project3D(float* u, float* v, float* w, float* p, float* div) {
    int currentIndex, n = fieldDimension;
    for (int k = 1; k <= n; k++) {
        for (int j = 1; j <= n; j++) {
            for (int i = 1; i <= n; i++) {
                currentIndex = IX3D(i,j,k);
                div[currentIndex] = -0.5f*(
                    u[IX3D(i+1, j, k)]
                    -u[IX3D(i-1, j, k)]
                    +v[IX3D(i, j+1, k)]
                    -v[IX3D(i, j-1, k)]
                    +w[IX3D(i, j, k+1)]
                    -w[IX3D(i, j, k-1)]
                )/(n+2);
                p[currentIndex] = 0;
            }
        }
    }

    std::thread s1(&Fluid::setBnd3D, this, 0, div);
    std::thread s2(&Fluid::setBnd3D, this, 0, p);
    s1.join();
    s2.join();
    //setBnd3D(0, div);
    //setBnd3D(0, p);
    linSolve3D(0, p, div, 1, 6);

    for (int k = 1; k <= n; k++) {
        for (int j = 1; j <= n; j++) {
            for (int i = 1; i <= n; i++) {
                currentIndex = IX3D(i,j,k);

                u[currentIndex] -= 0.5f * (  p[IX3D(i+1, j, k)]
                -p[IX3D(i-1, j, k)]) * (n+2);
                v[currentIndex] -= 0.5f * (  p[IX3D(i, j+1, k)]
                -p[IX3D(i, j-1, k)]) * (n+2);
                w[currentIndex] -= 0.5f * (  p[IX3D(i, j, k+1)]
                -p[IX3D(i, j, k-1)]) * (n+2);
            }
        }
    }

    std::thread s3(&Fluid::setBnd3D, this, 1, u);
    std::thread s4(&Fluid::setBnd3D, this, 2, v);
    std::thread s5(&Fluid::setBnd3D, this, 3, w);
    s3.join();
    s4.join();
    s5.join();
    //setBnd3D(1, u);
    //setBnd3D(2, v);
    //setBnd3D(3, w);
}

void Fluid::setBnd(int b, float* x) {
    int i, n = fieldDimension;

    for ( i = 1; i <= fieldDimension; i++ ) {
        x[IX3D(0,i,1)] = b == 1 ? -x[IX3D(1,i,1)] : x[IX3D(1,i,1)];
        x[IX3D(n+1,i,1)] = b == 1 ? -x[IX3D(n,i,1)] : x[IX3D(n,i,1)];
    }

    for ( i = 1; i <= fieldDimension; i++ ) {
        x[IX3D(i,0,1)] = b == 2 ? -x[IX3D(i,1,1)] : x[IX3D(i,1,1)];
        x[IX3D(i,n+1,1)] = b == 2 ? -x[IX3D(i,n,1)] : x[IX3D(i,n,1)];
    }

    x[IX3D(0, 0,1)] = 0.5f*(x[IX3D(1, 0,1)] + x[IX3D(0, 1,1)]);
    x[IX3D(0, n+1,1)] = 0.5f*(x[IX3D(1, n+1,1)] + x[IX3D(0, n,1)]);
    x[IX3D(n+1, 0,1)] = 0.5f*(x[IX3D(n, 0,1)] + x[IX3D(n+1, 1,1)]);
    x[IX3D(n+1, n+1,1)] = 0.5f*(x[IX3D(n, n+1,1)] + x[IX3D(n+1, n,1)]);
}

void Fluid::setBnd3D(int b, float* x) {
    int n = fieldDimension+2;

    for(int k = 1; k <= n-2; k++) {
        for(int j = 1; j <= n-2; j++) {
            x[IX3D(0, j, k)] = b == 1 ? pf*-x[IX3D(1, j, k)] : pf*x[IX3D(1, j, k)];
            x[IX3D(n-1, j, k)] = b == 1 ? pf*-x[IX3D(n-2, j, k)] : pf*x[IX3D(n-2, j, k)];
        }
    }

    for(int k = 1; k <= n-2; k++) {
        for(int i = 1; i <= n-2; i++) {
            x[IX3D(i, 0  , k)] = b == 2 ? pf*-x[IX3D(i, 1  , k)] : pf*x[IX3D(i, 1  , k)];
            x[IX3D(i, n-1, k)] = b == 2 ? pf*-x[IX3D(i, n-2, k)] : pf*x[IX3D(i, n-2, k)];
        }
    }

    for(int j = 1; j <= n-2; j++) {
        for(int i = 1; i <= n-2; i++) {
            x[IX3D(i, j, 0  )] = b == 3 ? pf*-x[IX3D(i, j, 1  )] : pf*x[IX3D(i, j, 1  )];
            x[IX3D(i, j, n-1)] = b == 3 ? pf*-x[IX3D(i, j, n-2)] : pf*x[IX3D(i, j, n-2)];
        }
    }
    //fprintf(stderr, "Starting corner sets....");
    x[IX3D(0, 0, 0)]       = 0.33f * (x[IX3D(1, 0, 0)]
                                  + x[IX3D(0, 1, 0)]
                                  + x[IX3D(0, 0, 1)]);

    x[IX3D(0, n-1, 0)]     = 0.33f * (x[IX3D(1, n-1, 0)]
                                  + x[IX3D(0, n-2, 0)]
                                  + x[IX3D(0, n-1, 1)]);

    x[IX3D(0, 0, n-1)]     = 0.33f * (x[IX3D(1, 0, n-1)]
                                  + x[IX3D(0, 1, n-1)]
                                  + x[IX3D(0, 0, n-2)]);

    x[IX3D(0, n-1, n-1)]   = 0.33f * (x[IX3D(1, n-1, n-1)]
                                  + x[IX3D(0, n-2, n-1)]
                                  + x[IX3D(0, n-1, n-2)]);

    x[IX3D(n-1, 0, 0)]     = 0.33f * (x[IX3D(n-2, 0, 0)]
                                  + x[IX3D(n-1, 1, 0)]
                                  + x[IX3D(n-1, 0, 1)]);

    x[IX3D(n-1, n-1, 0)]   = 0.33f * (x[IX3D(n-2, n-1, 0)]
                                  + x[IX3D(n-1, n-2, 0)]
                                  + x[IX3D(n-1, n-1, 1)]);

    x[IX3D(n-1, 0, n-1)]   = 0.33f * (x[IX3D(n-2, 0, n-1)]
                                  + x[IX3D(n-1, 1, n-1)]
                                  + x[IX3D(n-1, 0, n-2)]);

    x[IX3D(n-1, n-1, n-1)] = 0.33f * (x[IX3D(n-2, n-1, n-1)]
                                  + x[IX3D(n-1, n-2, n-1)]
                                  + x[IX3D(n-1, n-1, n-2)]);
    //fprintf(stderr, "Completed\n");
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

void Fluid::setPermeability(float p) {
    pf = p;
}

void Fluid::addDensity(float amount, int x, int y, int z) {
    int index = IX3D(x,y,z);
    dens[index]+amount <= 255.0 ? dens[index] += amount : dens[index] = 255.0;
}

void Fluid::addVelocity(float amountX, float amountY, float amountZ, int x, int y, int z) {
    int index = IX3D(x, y, z);

    vx[index]+amountX <= 255.0 ? vx[index] += amountX : vx[index] = 255.0;
    vy[index]+amountY <= 255.0 ? vy[index] += amountY : vy[index] = 255.0;
    vz[index]+amountZ <= 255.0 ? vz[index] += amountZ : vz[index] = 255.0;
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

float* Fluid::getZVelocity() {
    return vz;
}

#endif
