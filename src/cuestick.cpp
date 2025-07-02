#include "cuestick.hpp"

#include <glm/vec2.hpp>
#include <glm/glm.hpp>

#include <iostream>

constexpr float Cuestick::MAX_PULL_BACK;
constexpr float Cuestick::MAX_HORIZONTAL_OFFSET;
constexpr float Cuestick::DISTANCE;

Cuestick::Cuestick(ObjectID id, const glm::vec3 &position, const glm::vec3 &angles)
    :
    position(glm::vec4(position, 1.0f)),
    angles(angles),
    object_id(static_cast<int>(id)),
    state(CueState::Aiming),
    pullBackDistance(0.0f),
    shotPower(0.0f),
    horizontalOffset(0.0f)
    {}

Cuestick::Cuestick(ObjectID id, float x, float y, float z, float angleX, float angleY, float angleZ)
    : Cuestick(id, glm::vec3(x, y, z), glm::vec3(angleX, angleY, angleZ))
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
    const glm::vec4 ball_position = white_ball.getPosition();
    const glm::vec4 dir = glm::vec4(std::sin(angles.y), 0.0f, std::cos(angles.y), 0.0f);
    const glm::vec4 right = glm::vec4(-dir.z, 0.0f, dir.x, 0.0f);

    switch (state) {
        case CueState::Aiming:
        {
            // In Aiming state, the cue just follows the white ball
            pullBackDistance = 0.0f;
            position = ball_position + dir * Cuestick::DISTANCE + right * horizontalOffset;
            break;
        }

        case CueState::Charging:
        {
            // Pull the cue stick back
            pullBackDistance = std::min(pullBackDistance + Cuestick::CHARGE_SPEED * deltaTime, Cuestick::MAX_PULL_BACK);
            position = ball_position + dir * (Cuestick::DISTANCE + pullBackDistance) + right * horizontalOffset;
            break;
        }
        case CueState::Shooting:
        {
            const glm::vec2 dir_vec = glm::vec2(dir.x, dir.z);
            const glm::vec2 sidestep_vec = glm::vec2(-right.x, -right.z);
            calculateShooting(deltaTime, white_ball, dir_vec, sidestep_vec);
            break;
        }
    }
}

glm::vec4 Cuestick::getPosition() const {
    return position;
}

glm::vec3 Cuestick::getAngles() const {
    return angles;
}

Cuestick::CueState Cuestick::getState() const {
    return state;
}

bool Cuestick::isAiming() const {
    return state == CueState::Aiming;
}

void Cuestick::setPosition(const glm::vec4 &newPosition) {
    position = newPosition;
}

void Cuestick::setPosition(float x, float y, float z) {
    position = glm::vec4(x, y, z, 1.0f);
}

void Cuestick::setAngles(const glm::vec3 &newAngles) {
    angles = newAngles;
}

void Cuestick::setAngles(float angleX, float angleY, float angleZ) {
    angles = glm::vec3(angleX, angleY, angleZ);
}

int Cuestick::getObjectID() const {
    return object_id;
}

void Cuestick::addAngleY(float value) {
    angles.y += value;
}

void Cuestick::addAngleX(float value) {
    angles.x += value;
}

void Cuestick::addAngleZ(float value) {
    angles.z += value;
}
