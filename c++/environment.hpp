#ifndef _ENVIRONMENT_HPP_
#define _ENVIRONMENT_HPP_

namespace Environment {
    void setParams(Fluid*);
    void addObstacle();
}

void Environment::addObstacle() {
    for ( int j = 32; j < 40; j++ ) {
            for ( int i = 32; i < 40; i++ ) {
                program.locked_index_array[IX3D(i,j,0)] = true;
            }
    }
}

void Environment::setParams(Fluid* field) {
    int sourcePoint1 = FLUIDSIZE/2;
    int sourcePoint2 = FLUIDSIZE/2 + 5;
    field->setIterations(object.iterations);
    field->setDiffuse(object.diffusion);
    field->setViscosity(object.viscosity);
    field->setTimeStep(object.dt);
    field->setPermeability(object.permeability);

    // Add density and velocity to the position indicated by the mouse
    field->addVelocity( object.velocityXAmount, object.velocityYAmount, object.velocityZAmount,
     object.velocityXPos, object.velocityYPos, object.velocityZPos );
    field->addDensity( object.densityAmount, object.densityXPos,
     object.densityYPos, object.densityZPos );

    if (object.densityLocation) {
        // Add density and velocity starting at FLUIDSIZE/2
        field->addDensity(255, sourcePoint1+object.sourceXOffset,
            sourcePoint1+object.sourceYOffset, sourcePoint1);
        field->addDensity(255, sourcePoint1+object.sourceXOffset,
            sourcePoint1+object.sourceYOffset, sourcePoint1+1);
        field->addDensity(255, sourcePoint1+object.sourceXOffset,
            sourcePoint1+1+object.sourceYOffset, sourcePoint1);
        field->addDensity(255, sourcePoint1+object.sourceXOffset,
            sourcePoint1+1+object.sourceYOffset, sourcePoint1+1);
    }

    if (object.addVelocity) {
        field->addVelocity(255, 0, 0, sourcePoint1-1+object.sourceXOffset,
            sourcePoint1-1+object.sourceYOffset, sourcePoint1-1);
        field->addVelocity(255, 0, 0, sourcePoint1-1+object.sourceXOffset,
            sourcePoint1-1+object.sourceYOffset, sourcePoint1+2);
        field->addVelocity(255, 0, 0, sourcePoint1-1+object.sourceXOffset,
            sourcePoint1+2+object.sourceYOffset, sourcePoint1-1);
        field->addVelocity(255, 0, 0, sourcePoint1-1+object.sourceXOffset,
            sourcePoint1+2+object.sourceYOffset, sourcePoint1+2);
    }
    // Opposing force
    if (object.densityLocation) {
        field->addDensity(255, sourcePoint1+object.sourceXOffset+15,
            sourcePoint1+object.sourceYOffset, sourcePoint1);
        field->addDensity(255, sourcePoint1+object.sourceXOffset+15,
            sourcePoint1+object.sourceYOffset, sourcePoint1+1);
        field->addDensity(255, sourcePoint1+object.sourceXOffset+15,
            sourcePoint1+1+object.sourceYOffset, sourcePoint1);
        field->addDensity(255, sourcePoint1+object.sourceXOffset+15,
            sourcePoint1+1+object.sourceYOffset, sourcePoint1+1);
    }
    if (object.addVelocity) {
        field->addVelocity(-255, 0, 0, sourcePoint1+object.sourceXOffset+16,
            sourcePoint1-1+object.sourceYOffset, sourcePoint1-1);
        field->addVelocity(-255, 0, 0, sourcePoint1+object.sourceXOffset+16,
            sourcePoint1-1+object.sourceYOffset, sourcePoint1+2);
        field->addVelocity(-255, 0, 0, sourcePoint1+object.sourceXOffset+16,
            sourcePoint1+2+object.sourceYOffset, sourcePoint1-1);
        field->addVelocity(-255, 0, 0, sourcePoint1+object.sourceXOffset+16,
            sourcePoint1+2+object.sourceYOffset, sourcePoint1+2);
    }
    // Add density and velocity at FLUIDSIZE/2 + 5
    /*field->addDensity(255, sourcePoint1+object.sourceXOffset+10,
        sourcePoint1+object.sourceYOffset-10, sourcePoint1);
    field->addDensity(255, sourcePoint1+object.sourceXOffset+11,
        sourcePoint1+object.sourceYOffset-10, sourcePoint1);
    field->addDensity(255, sourcePoint1+object.sourceXOffset+10,
        sourcePoint1+object.sourceYOffset-10, sourcePoint1+1);
    field->addDensity(255, sourcePoint1+object.sourceXOffset+11,
        sourcePoint1+object.sourceYOffset-10, sourcePoint1+1);

    field->addVelocity(0, 255, 0, sourcePoint1+object.sourceXOffset+9,
        sourcePoint1+object.sourceYOffset-11, sourcePoint1-1);
    field->addVelocity(0, 255, 0, sourcePoint1-1+object.sourceXOffset+9,
        sourcePoint1+object.sourceYOffset-11, sourcePoint1-1);
    field->addVelocity(0, 255, 0, sourcePoint1-1+object.sourceXOffset+12,
        sourcePoint1+object.sourceYOffset-11, sourcePoint1+2);
    field->addVelocity(0, 255, 0, sourcePoint1-1+object.sourceXOffset+12,
        sourcePoint1+object.sourceYOffset-11, sourcePoint1+2);*/

    //if (!object.densityLocation) {
        field->addDensity(255, 5, 5, 5);
        field->addDensity(255, 5, 55, 5);
        field->addDensity(255, 55, 5, 5);
        field->addDensity(255, 55, 55, 5);

        field->addVelocity(255, 0, 0, 4, 5, 5);
        field->addVelocity(0, 0, 255, 5, 5, 4);

        field->addVelocity(0, 255, 0, 55, 4, 5);
        field->addVelocity(0, 0, 255, 55, 5, 4);

        field->addVelocity(-255, 0, 0, 56, 55, 5);
        field->addVelocity(0, 0, 255, 55, 55, 4);

        field->addVelocity(0, -255, 0, 5, 56, 5);
        field->addVelocity(0, 0, 255, 5, 55, 4);
    //}

    field->update();
}

#endif
