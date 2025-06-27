#include "cuestick.hpp"

Cuestick::Cuestick(float x, float y, float z, float angleX, float angleY, float angleZ)
    : x(x), 
    y(y), 
    z(z), 
    angleY(angleY),
    angleX(angleX),
    angleZ(angleZ) {}

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

void Cuestick::setX(float newX) {
    x = newX;
}

void Cuestick::setY(float newY) {
    y = newY;
}

void Cuestick::setZ(float newZ) {
    z = newZ;
}

void Cuestick::setAngleY(float newAngleY) {
    angleY = newAngleY;
}

void Cuestick::setAngleX(float newAngleX) {
    angleX = newAngleX;
}

void Cuestick::setAngleZ(float newAngleZ) {
    angleZ = newAngleZ;
}
