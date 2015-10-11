#ifndef _SIMULATOR_HPP_
#define _SIMULATOR_HPP_

#include <iostream>

static uint windowWidth;
static uint windowHeight;
static GLFWwindow* window;

namespace Simulator {
    class Simulator {
    private:
        Fluid *field;
    public:
        Simulator(short, short);

    };

    Simulator::Simulator(short width, short height) {
        windowWidth = width;
        windowHeight = height;
        field = new Field(8, width);
        field->setDiffuse(0.2);
        field->setTimeStep(0.1);
        field->setViscosity(0.7);
        field->setIterations(20);
    }

    void Simulator::
}

#endif
