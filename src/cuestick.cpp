#include "cuestick.hpp"

#include <glm/vec2.hpp>
#include <glm/glm.hpp>

#include <iostream>

constexpr float Cuestick::MAX_PULL_BACK;
constexpr float Cuestick::MAX_HORIZONTAL_OFFSET;

Cuestick::Cuestick(ObjectID id, float x, float y, float z, float angleX, float angleY, float angleZ)
    :
    x(x), 
    y(y), 
    z(z), 
    angleY(angleY),
    angleX(angleX),
    angleZ(angleZ),
    object_id(static_cast<int>(id)),
    state(CueState::Aiming),
    pullBackDistance(0.0f),
    shotPower(0.0f),
    horizontalOffset(0.0f)
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

void Cuestick::addHorizontalOffset(float offset) {
    horizontalOffset += offset;
    // There is not std::clamp in the C++ version currently used
    horizontalOffset = std::max(-Cuestick::MAX_HORIZONTAL_OFFSET, std::min(Cuestick::MAX_HORIZONTAL_OFFSET, horizontalOffset));
}

void Cuestick::update(float deltaTime, Ball& white_ball)
{
    const float dir_x = std::sin(angleY);
    const float dir_z = std::cos(angleY);

    const float right_x = -dir_z;
    const float right_z = dir_x;

    switch (state) {
        case CueState::Aiming:
        {
            // In Aiming state, the cue just follows the white ball
            pullBackDistance = 0.0f;
            this->x = white_ball.x + dir_x * Cuestick::DISTANCE + right_x * horizontalOffset;
            this->y = Cuestick::HEIGHT;
            this->z = white_ball.z + dir_z * Cuestick::DISTANCE + right_z * horizontalOffset;
            break;
        }

        case CueState::Charging:
        {
            // Pull the cue stick back
            pullBackDistance = std::min(pullBackDistance + Cuestick::CHARGE_SPEED * deltaTime, Cuestick::MAX_PULL_BACK);
            this->x = white_ball.x + dir_x * (Cuestick::DISTANCE + pullBackDistance) + right_x * horizontalOffset;
            this->z = white_ball.z + dir_z * (Cuestick::DISTANCE + pullBackDistance) + right_z * horizontalOffset;
            break;
        }
        case CueState::Shooting:
        {
            const glm::vec2 dir_vec = glm::vec2(dir_x, dir_z);
            const glm::vec2 sidestep_vec = glm::vec2(-right_x, -right_z);
            calculateShooting(deltaTime, white_ball, dir_vec, sidestep_vec);
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

int Cuestick::getObjectID() const {
    return object_id;
}

void Cuestick::addAngleY(float value) {
    angleY += value;
}

void Cuestick::addAngleX(float value) {
    angleX += value;
}

void Cuestick::addAngleZ(float value) {
    angleZ += value;
}
