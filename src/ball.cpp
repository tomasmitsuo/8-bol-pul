// TODO: CRIAR UMA CLASSE PARA A BOLA

#include "ball.hpp"
#include <cmath>

Ball::Ball(float x, float z, float vx, float vz, float radius)
    : x(x), z(z), vx(vx), vz(vz), radius(radius){}

Ball::Ball(float x, float z, float radius)
    : x(x), z(z), vx(0.0f), vz(0.0f), radius(radius){}

constexpr float FRICTION = 0.99f;

void Ball::update(float dt) 
{
    x += vx * dt;
    z += vz * dt;

    vx *= FRICTION;
    vz *= FRICTION;

    if (std::abs(vx) < 0.01f) vx = 0;
    if (std::abs(vz) < 0.01f) vz = 0;
}

void Ball::reflectOnWalls(float table_center_x, float table_center_z, float tableWidth, float tableHeight)
{
    if (x - radius < table_center_x - tableWidth / 2 || x + radius > table_center_x + tableWidth / 2) {
        vx *= -FRICTION;
    }
    if (z - radius < table_center_z - tableHeight / 2 || z + radius > table_center_z + tableHeight / 2) {
        vz *= -FRICTION;
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