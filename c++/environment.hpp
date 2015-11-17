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
    int midPoint = FLUIDSIZE/2;
    field->setIterations(object.iterations);
    field->setDiffuse(object.diffusion);
    field->setViscosity(object.viscosity);
    field->setTimeStep(object.dt);
    field->setPermeability(object.permeability);

    field->addVelocity( object.velocityXAmount, object.velocityYAmount, object.velocityZAmount,
     object.velocityXPos, object.velocityYPos, object.velocityZPos );
    field->addDensity( object.densityAmount, object.densityXPos,
     object.densityYPos, object.densityZPos );
    field->addDensity(255, midPoint+object.sourceXOffset,
        midPoint+object.sourceYOffset, midPoint);
    field->addDensity(255, midPoint+object.sourceXOffset,
        midPoint+object.sourceYOffset, midPoint+1);
    field->addDensity(255, midPoint+object.sourceXOffset,
        midPoint+1+object.sourceYOffset, midPoint);
    field->addDensity(255, midPoint+object.sourceXOffset,
        midPoint+1+object.sourceYOffset, midPoint+1);

    field->addVelocity(255, 0, 0, midPoint-1+object.sourceXOffset,
        midPoint-1+object.sourceYOffset, midPoint-1);
    field->addVelocity(255, 0, 0, midPoint-1+object.sourceXOffset,
        midPoint-1+object.sourceYOffset, midPoint+2);
    field->addVelocity(255, 0, 0, midPoint-1+object.sourceXOffset,
        midPoint+2+object.sourceYOffset, midPoint-1);
    field->addVelocity(255, 0, 0, midPoint-1+object.sourceXOffset,
        midPoint+2+object.sourceYOffset, midPoint+2);
    field->update();
}

#endif
