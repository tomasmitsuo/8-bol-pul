#include "ball.hpp"
#include <cmath>

#include "matrices.h"
#include "load_objects.hpp"


Ball::Ball(ObjectID id, float x, float z, float vx, float vz, float radius)
    : position(x, BALL_HEIGHT, z, 1.0f), velocity(vx, 0.0f, vz, 0.0f),
      radius(radius), pocketed(false),
      animating(false), animationProgress(0.0f), animationSpeed(ANIMATION_SPEED),
      anim_p0(0.0f, 0.0f, 0.0f, 1.0f), anim_p1(0.0f, 0.0f, 0.0f, 1.0f), anim_p2(0.0f, 0.0f, 0.0f, 1.0f),
      rotationAngle(0.0f), rotationAxis(1.0f, 0.0f, 0.0f, 0.0f),
      object_id(static_cast<int>(id)) {}

Ball::Ball(ObjectID id, float x, float z, float radius)
    : Ball(id, x, z, 0.0f, 0.0f, radius) {}

Ball::Ball(ObjectID id, glm::vec2 position, float radius)
    : Ball(id, position.x, position.y, 0.0f, 0.0f, radius) {}

void Ball::update(float dt) 
{
    if (pocketed){
        return;
    }

    if (animating) {
        updatePocketAnimation(dt);
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
    velocity.x *= Ball::FRICTION;
    velocity.z *= Ball::FRICTION;

    if (std::abs(velocity.x) < Ball::STOP_TRAVEL_THRESHOLD) velocity.x = 0;
    if (std::abs(velocity.y) < Ball::STOP_TRAVEL_THRESHOLD) velocity.y = 0;
    if (std::abs(velocity.z) < Ball::STOP_TRAVEL_THRESHOLD) velocity.z = 0;
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
    animating = false;
    animationProgress = 0.0f;
    pocketed = false;
    velocity = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
}

float Ball::getBallSpeed()
{
    return norm(velocity);
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

bool Ball::hasCollision() const
{
    return !pocketed && !animating;
}

bool Ball::isAnimating() const
{
    return animating;
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

glm::vec4 calculateBezierPoint(float t, const glm::vec4& p0, const glm::vec4& p1, const glm::vec4& p2) {
    float u = 1.0f - t;
    float uu = u * u;
    glm::vec4 p = uu * p0;
    p += 2.0f * u * t * p1;
    float tt = t * t;
    p += tt * p2;
    return p;
}

void Ball::startPocketAnimation(const glm::vec2& holePosition)
{
    if (animating || pocketed) {
        return;
    }

    animating = true;
    animationProgress = 0.0f;
    anim_p0 = position;
    anim_p2 = glm::vec4(holePosition.x, position.y - radius * 2.0f, holePosition.y, 1.0f);

    anim_p1 = glm::vec4(anim_p2.x, anim_p0.y, anim_p2.z, 1.0f);

    velocity = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
}

void Ball::updatePocketAnimation(float dt)
{
    if (!animating) {
        return;
    }

    animationProgress += dt * animationSpeed;

    if (animationProgress >= 1.0f) {
        animationProgress = 1.0f;
        animating = false;
        pocketed = true;
        if (this->object_id == static_cast<int>(ObjectID::WHITE_BALL)) {
            // Reset the white ball position after pocketing
            resetBallTo(glm::vec2(WHITE_BALL_X, WHITE_BALL_Z));
        }
        return;
    }

    glm::vec4 bezierPoint = calculateBezierPoint(animationProgress, anim_p0, anim_p1, anim_p2);
    position = glm::vec4(bezierPoint.x, bezierPoint.y, bezierPoint.z, 1.0f);
    velocity = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f); // Stop the ball during animation
}