#include "ball.hpp"
#include <cmath>

#include "matrices.h"


Ball::Ball(ObjectID id, float x, float z, float vx, float vz, float radius)
    : x(x), z(z), vx(vx), vz(vz), radius(radius), isPocketed(false),
      rotationAngle(0.0f), rotationAxis(1.0f, 0.0f, 0.0f, 0.0f), object_id(static_cast<int>(id)) {}

Ball::Ball(ObjectID id, float x, float z, float radius)
    : x(x), z(z), vx(0.0f), vz(0.0f), radius(radius), isPocketed(false),
      rotationAngle(0.0f), rotationAxis(1.0f, 0.0f, 0.0f, 0.0f), object_id(static_cast<int>(id)) {}

void Ball::update(float dt) 
{
    if (isPocketed){
        return;
    }

    if (isMoving()) {
        float speed = getBallSpeed();
        float distanceTraveled = speed * dt;
        float angleIncrement = distanceTraveled / radius;

        glm::vec4 velocityVector(vx, 0.0f, vz, 0.0f);
        
        if (speed > Ball::STABLE_ROTATION_THRESHOLD) {
            const glm::vec4 rotationVector = glm::vec4(-vz, 0.0f, vx, 0.0f);
            rotationAxis = rotationVector / norm(rotationVector);
        }

        rotationAngle += angleIncrement;
    }

    x += vx * dt;
    z += vz * dt;

    vx *= Ball::FRICTION;
    vz *= Ball::FRICTION;

    if (std::abs(vx) < 0.01f) vx = 0;
    if (std::abs(vz) < 0.01f) vz = 0;
}


bool Ball::isMoving() const 
{
    return vx != 0 || vz != 0;
}

void Ball::pocket()
{
    isPocketed = true;
    vx = 0.0f;
    vz = 0.0f;
}

float Ball::getBallSpeed()
{
    return sqrt(pow(this->vx,2) + pow(this->vz,2));
}

float Ball::getBallPositionX()
{
    return this->x;
}

float Ball::getBallPositionZ()
{
    return this->z;
}

float Ball::getRotationAngle() const
{
    return rotationAngle;
}

glm::vec4 Ball::getRotationAxis() const
{
    return rotationAxis;
}

int Ball::getObjectID() const
{
    return object_id;
}