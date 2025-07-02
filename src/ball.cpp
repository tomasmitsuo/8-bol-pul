#include "ball.hpp"
#include <cmath>

#include "matrices.h"


Ball::Ball(ObjectID id, float x, float z, float vx, float vz, float radius)
    : position(x, BALL_HEIGHT, z, 1.0f), velocity(vx, 0.0f, vz, 0.0f),
      radius(radius), pocketed(false),
      rotationAngle(0.0f), rotationAxis(1.0f, 0.0f, 0.0f, 0.0f), object_id(static_cast<int>(id)) {}

Ball::Ball(ObjectID id, float x, float z, float radius)
    : Ball(id, x, z, 0.0f, 0.0f, radius) {}

Ball::Ball(ObjectID id, glm::vec2 position, float radius)
    : Ball(id, position.x, position.y, 0.0f, 0.0f, radius) {}

void Ball::update(float dt) 
{
    if (pocketed){
        return;
    }

    if (isMoving()) {
        float speed = getBallSpeed();
        float distanceTraveled = speed * dt;
        float angleIncrement = distanceTraveled / radius;
        rotationAngle += angleIncrement;

        if (speed > Ball::STABLE_ROTATION_THRESHOLD) {
            const glm::vec4 rotationVector = glm::vec4(-velocity.z, 0.0f, velocity.x, 0.0f);
            rotationAxis = rotationVector / norm(rotationVector);
        }
    }

    position += velocity * dt;
    velocity *= Ball::FRICTION;

    if (std::abs(velocity.x) < 0.01f) velocity.x = 0;
    if (std::abs(velocity.y) < 0.01f) velocity.y = 0;
    if (std::abs(velocity.z) < 0.01f) velocity.z = 0;
}


bool Ball::isMoving() const 
{
    return velocity.x != 0 || velocity.y != 0 || velocity.z != 0;
}

void Ball::pocket()
{
    pocketed = true;
    velocity = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
}

void Ball::unpocket()
{
    pocketed = false;
    velocity = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
}

float Ball::getBallSpeed()
{
    return glm::length(velocity);
}

glm::vec4 Ball::getPosition() const
{
    return position;
}

glm::vec4 Ball::getVelocity() const
{
    return velocity;
}

float Ball::getRadius() const
{
    return radius;
}

void Ball::applyForce(const glm::vec4& force)
{
    velocity += force;
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

bool Ball::shouldBeDrawn() const
{
    return !pocketed;
}

bool Ball::isPocketed() const
{
    return pocketed;
}

void Ball::resetBallTo(const glm::vec4& position)
{
    this->position = position;
    velocity = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    pocketed = false;
}

void Ball::resetBallTo(const glm::vec2& position)
{
    this->position = glm::vec4(position.x, BALL_HEIGHT, position.y, 1.0f);
    velocity = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    pocketed = false;
}
