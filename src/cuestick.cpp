#include "cuestick.hpp"

#include "obj_constants.hpp"

#include <iostream>

Cuestick::Cuestick(float x, float y, float z, float angleX, float angleY, float angleZ)
    : x(x), 
    y(y), 
    z(z), 
    angleY(angleY),
    angleX(angleX),
    angleZ(angleZ),
    state(CueState::Aiming),
    pullBackDistance(0.0f),
    shotPower(0.0f)
    {}

void Cuestick::startCharging() {
    if (state == CueState::Aiming) {
        state = CueState::Charging;
        pullBackDistance = 0.0f;
        std::cout << "State: Charging" << std::endl;
    }
}

void Cuestick::shoot() {
    if (state == CueState::Charging) {
        state = CueState::Shooting;
        shotPower = pullBackDistance;
        std::cout << "State: Shooting with Power: " << shotPower << std::endl;
    }
}

void Cuestick::update(float deltaTime, Ball& white_ball)
{
    const float dir_x = std::sin(angleY);
    const float dir_z = std::cos(angleY);

     switch (state) {
        case CueState::Aiming:
        {
            // In Aiming state, the cue just follows the white ball
            pullBackDistance = 0.0f;
            this->x = white_ball.x + dir_x * CUESTICK_DISTANCE;
            this->y = CUESTICK_HEIGHT;
            this->z = white_ball.z + dir_z * CUESTICK_DISTANCE;
            break;
        }

        case CueState::Charging:
        {
            // Pull the cue stick back
            pullBackDistance = std::min(pullBackDistance + CUESTICK_CHARGE_SPEED * deltaTime, MAX_PULL_BACK);
            this->x = white_ball.x + dir_x * (CUESTICK_DISTANCE + pullBackDistance);
            this->z = white_ball.z + dir_z * (CUESTICK_DISTANCE + pullBackDistance);
            break;
        }
        case CueState::Shooting:
        {
            // Move the cue stick forward rapidly
            pullBackDistance -= CUESTICK_SHOOT_SPEED * deltaTime;

            // Check for collision
            if (pullBackDistance <= 0.0f) {
                std::cout << "Impact!" << std::endl;
                // Apply force to the white ball
                white_ball.vx = -dir_x * shotPower * SHOT_POWER_MULTIPLIER;
                white_ball.vz = -dir_z * shotPower * SHOT_POWER_MULTIPLIER;

                // Reset to Aiming state
                state = CueState::Aiming;
            }

            // Update position during the shot animation
            this->x = white_ball.x + dir_x * (CUESTICK_DISTANCE + std::max(0.0f, pullBackDistance));
            this->z = white_ball.z + dir_z * (CUESTICK_DISTANCE + std::max(0.0f, pullBackDistance));
            break;
        }
    }
}

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

Cuestick::CueState Cuestick::getState() const {
    return state;
}

bool Cuestick::isAiming() const {
    return state == CueState::Aiming;
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
