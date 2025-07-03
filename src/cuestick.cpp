#include "cuestick.hpp"

#include <glm/vec2.hpp>
#include <glm/glm.hpp>

#include <iostream>

constexpr float Cuestick::MAX_PULL_BACK;
constexpr float Cuestick::SHOT_POWER_MULTIPLIER;
constexpr float Cuestick::HORIZONTAL_EFFECT_SCALE;
constexpr float Cuestick::VERTICAL_UP_EFFECT_SCALE;
constexpr float Cuestick::VERTICAL_DOWN_EFFECT_SCALE;

Cuestick::Cuestick(ObjectID id, const glm::vec3 &position, const glm::vec3 &angles, const float followRadius)
    :
    position(glm::vec4(position, 1.0f)),
    angles(angles),
    object_id(static_cast<int>(id)),
    state(CueState::Aiming),
    pullBackDistance(0.0f),
    prev_pullBackDistance(0.0f),
    shotPower(0.0f),
    horizontalOffset(0.0f),
    verticalOffset(0.0f),
    followRadius(followRadius),
    last_right_button(false),
    should_track_ball(true),
    last_pullBackDistance(0.0f)
    {}

Cuestick::Cuestick(ObjectID id, float x, float y, float z, float angleX, float angleY, float angleZ, float followRadius)
    : Cuestick(id, glm::vec3(x, y, z), glm::vec3(angleX, angleY, angleZ), followRadius)
    {}

void Cuestick::startCharging() {
    if (state == CueState::Aiming) {
        state = CueState::Charging;
        pullBackDistance = 0.0f;
    }
}

void Cuestick::shoot() {
    if (state == CueState::Charging) {
        state = CueState::Shooting;
        shotPower = pullBackDistance;
    }
}

void Cuestick::strafeHorizontal(float distance)
{
    horizontalOffset += distance;
    clampAimingOffsets();
}

void Cuestick::strafeVertical(float distance)
{
    verticalOffset += distance;
    clampAimingOffsets();
}

void Cuestick::resetAim()
{
    state = CueState::Aiming;
    horizontalOffset = 0.0f;
    verticalOffset = 0.0f;
    last_pullBackDistance = 0.0f;
}

void Cuestick::clampAimingOffsets()
{
    const float max_offset = followRadius * 0.95f;
    const float total_offset_squared = horizontalOffset * horizontalOffset + verticalOffset * verticalOffset;

    if (total_offset_squared > (max_offset * max_offset))
    {
        const float total_offset = std::sqrt(total_offset_squared);
        horizontalOffset = (horizontalOffset / total_offset) * max_offset;
        verticalOffset = (verticalOffset / total_offset) * max_offset;
    }
}

std::string Cuestick::update(float deltaTime, Ball& white_ball, const Camera& camera)
{
    should_track_ball = !(white_ball.isMoving() || white_ball.isAnimating());
    // should_track_ball = true; // Fun for gameplay testing
    switch (state)
    {
    case CueState::Aiming:
        pullBackDistance = 0.0f;
        last_pullBackDistance = 0.0f;
        break;
    case CueState::Charging:
        last_pullBackDistance = pullBackDistance;
        pullBackDistance = std::min(pullBackDistance + Cuestick::CHARGE_SPEED * deltaTime, Cuestick::MAX_PULL_BACK);
        break;
    case CueState::Shooting:
        pullBackDistance -= Cuestick::SHOOT_SPEED * deltaTime;
        break;
    case CueState::Shot:
        if (should_track_ball)
            resetAim();
        break;
    }

    const glm::vec4 ball_position = white_ball.getPosition();
    const glm::vec4 dir = glm::vec4(std::sin(angles.y), 0.0f, std::cos(angles.y), 0.0f);
    const glm::vec4 right = glm::vec4(-dir.z, 0.0f, dir.x, 0.0f);
    
    const float backwardDistance = followRadius + std::max(0.0f, pullBackDistance);
    
    position = ball_position + dir * backwardDistance + right * horizontalOffset;
    position.y = Cuestick::HEIGHT + verticalOffset;
    if (camera.isUsingLookAtCamera())
    {
        setAngles(0.0f, camera.getTheta(), 0.0f);
    }
    
    std::string outcome = "";
    if (state == CueState::Shooting)
    {
        outcome = calculateShooting(white_ball);
    }

    prev_pullBackDistance = pullBackDistance;
    return outcome;
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

float Cuestick::getLastPullBackDistance() const {
    return last_pullBackDistance;
}

bool Cuestick::isAiming() const {
    return state == CueState::Aiming;
}

bool Cuestick::shouldBeDrawn() const {
    return should_track_ball;
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


void Cuestick::control(float delta_time, bool right_mouse_button, bool middle_mouse_button, bool go_front, bool go_back, bool go_left, bool go_right, bool strafe_left, bool strafe_right, const Camera &camera, const Ball &white_ball)
{
    if (shouldBeDrawn())
    {
        // Check for right-click PRESS event
        if (right_mouse_button && !last_right_button) {
            startCharging();
        }

        // Check for right-click RELEASE event
        if (!right_mouse_button && last_right_button) {
            shoot();
        }

        if (middle_mouse_button)
        {
            resetAim();
        }

        if (isAiming()) {
            if (camera.isUsingLookAtCamera())
            {
                if (go_left)
                {
                    strafeHorizontal(Cuestick::STRAFE_SPEED * delta_time);
                }

                if (go_right)
                {
                    strafeHorizontal(-Cuestick::STRAFE_SPEED * delta_time);
                }

                if (go_front)
                {
                    strafeVertical(Cuestick::STRAFE_SPEED * delta_time);
                }

                if (go_back)
                {
                    strafeVertical(-Cuestick::STRAFE_SPEED * delta_time);
                }
            }
            else
            {
                // If using free camera, we can change the angles of the cuestick with Q and E keys
                if (strafe_left)
                {
                    addAngleY(-Cuestick::ROTATION_SPEED * delta_time);
                }
                if (strafe_right)
                {
                    addAngleY(Cuestick::ROTATION_SPEED * delta_time);
                }
            }
        }
    }

    last_right_button = right_mouse_button;
}