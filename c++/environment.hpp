#ifndef _ENVIRONMENT_HPP_
#define _ENVIRONMENT_HPP_

namespace Environment {
    void addObstacle();
}

void Environment::addObstacle() {
    for ( int j = 32; j < 40; j++ ) {
            for ( int i = 32; i < 40; i++ ) {
                program.locked_index_array[IX3D(i,j,0)] = true;
            }
    }
}

#endif
