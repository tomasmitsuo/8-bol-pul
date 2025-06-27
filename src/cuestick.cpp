#include "cuestick.hpp"

Cuestick::Cuestick()
    :   x(0.0f), 
        y(0.0f), 
        z(0.0f), 
        angleY(0.0f), 
        angleX(0.0f), 
        angleZ(0.0f) {}

float Cuestick::getX() const {
    return x;
}

float Cuestick::getY() const {
    return y;
}

float Cuestick::getZ() const {
    return z;
}

float Cuestick::getAngleX() const {
    return angleX;
}

float Cuestick::getAngleY() const {
    return angleY;
}

float Cuestick::getAngleZ() const {
    return angleZ;
}
