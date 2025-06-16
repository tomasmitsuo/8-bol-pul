// TODO: CRIAR UMA CLASSE PARA A BOLA

#include "ball.hpp"
#include <cmath>

Ball::Ball(float x, float z, float vx, float vz, float radius)
    : x(x), z(z), vx(vx), vz(vz), radius(radius){}

void Ball::update(float dt) 
{
    x += vx * dt;
    z += vz * dt;

    vx *= 0.99f;
    vz *= 0.99f;

    if (std::abs(vx) < 0.01f) vx = 0;
    if (std::abs(vz) < 0.01f) vz = 0;
}

void Ball::reflectOnWalls(float tableWidth, float tableHeight)
{
    if (x - radius < 0 || x + radius > tableWidth) {
        vx *= -1;
    }
    if (z - radius < 0 || z + radius > tableHeight) {
        vz *= -1;
    }
}

bool Ball::isMoving() const 
{
    return vx != 0 || vz != 0;
}

float Ball::getBallSpeed()
{
    return sqrt(pow(this->vz,2) + pow(this->vz,2));
}

float Ball::getBallPositionX()
{
    return this->x;
}

float Ball::getBallPositionZ()
{
    return this->z;
}