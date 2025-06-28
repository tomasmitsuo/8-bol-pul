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

bool Ball::isCollidingWith(const Ball& other) const
{
    float dx = x - other.x;
    float dz = z - other.z;
    float distance_squared = dx * dx + dz * dz;
    float radius_sum = radius + other.radius;
    return distance_squared < (radius_sum * radius_sum);
}

void Ball::handleCollision(Ball& other)
{
    float dx = other.x - x;
    float dz = other.z - z;

    float distance = std::sqrt(dx * dx + dz * dz);
    if (distance == 0.0f) {
        dx = 0.01f;
        distance = 0.01f;
    }

    const float nx = dx / distance;
    const float nz = dz / distance;

    const float overlap = (radius + other.radius) - distance;

    x -= overlap * nx * 0.5f;
    z -= overlap * nz * 0.5f;
    other.x += overlap * nx * 0.5f;
    other.z += overlap * nz * 0.5f;

    const float tx = -nz;
    const float tz = nx;

    const float v1n = vx * nx + vz * nz;
    const float v1t = vx * tx + vz * tz;
    const float v2n = other.vx * nx + other.vz * nz;
    const float v2t = other.vx * tx + other.vz * tz;

    const float v1n_final = v2n;
    const float v2n_final = v1n;

    vx = (v1n_final * nx) + (v1t * tx);
    vz = (v1n_final * nz) + (v1t * tz);
    other.vx = (v2n_final * nx) + (v2t * tx);
    other.vz = (v2n_final * nz) + (v2t * tz);
}

int Ball::getObjectID() const
{
    return object_id;
}