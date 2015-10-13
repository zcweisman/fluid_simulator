/*
 * Zachary Weisman
 * simulator.hpp
 * Contains functions that control the Fluid environment
 */

#ifndef _SIMULATOR_HPP_
#define _SIMULATOR_HPP_

#include <iostream>
#include "fluid.hpp"
#include "GLSL.hpp"
#include "glm/glm.hpp"

static uint windowWidth;
static uint windowHeight;
static GLFWwindow* window;

class Simulator {
private:
    Fluid *field;
    double force_multiplier;
public:
    Simulator(short, short);
    void mouseMove(
    void update();
};

Simulator::Simulator(short size, short windowSize) {
    windowWidth = windowSize;
    windowHeight = windowSize;
    field = new Field(size, windowSize);
    field->setDiffuse(0.2);
    field->setTimeStep(0.1);
    field->setViscosity(0.7);
    field->setIterations(20);

    force_multiplier = 0.;
}

void Simulator::

void Simulator::update() {
    return;
}

#endif
