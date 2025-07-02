#pragma once

#include "table.hpp"
#include "ball.hpp"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

enum class ObjectID : int;

class Cuestick {
public:
    static constexpr float ANGLE = -100.0f * 3.14159265359 / 180.0f;
    static constexpr float HEIGHT = Table::HEIGHT - 0.4f;
    static constexpr float DISTANCE = Ball::DIAMETER;

    static constexpr float CHARGE_SPEED = 5.0f;
    static constexpr float MAX_PULL_BACK = 3.0f;
    static constexpr float SHOOT_SPEED = 25.0f;
    static constexpr float SHOT_POWER_MULTIPLIER = 5.0f;

    static constexpr float HORIZONTAL_OFFSET_SPEED = 0.25f;
    static constexpr float MAX_HORIZONTAL_OFFSET = 0.1f;
    static constexpr float SIDESTEP_FACTOR = 1.9f;

    static constexpr float ROTATION_SPEED = 0.5f;

public:
    enum class CueState
    {
        Aiming,
        Charging,
        Shooting
    };

private:
    glm::vec4 position;
    glm::vec3 angles;

    int object_id;

    CueState state;
    float pullBackDistance;
    float shotPower;
    float horizontalOffset;

public:
    Cuestick(ObjectID id, const glm::vec3 &position, const glm::vec3 &angles);
    Cuestick(ObjectID id, float x, float y, float z, float angleX, float angleY, float angleZ);

    void update(float deltaTime, Ball& whiteBall);

    void calculateShooting(float deltaTime, Ball &white_ball, const glm::vec2 &dir_vec, const glm::vec2 &sidestep_vec);

    void startCharging();
    void shoot();

    void addHorizontalOffset(float offset);

    glm::vec4 getPosition() const;
    glm::vec3 getAngles() const;
    CueState getState() const;
    bool isAiming() const;

    void setPosition(const glm::vec4 &newPosition);
    void setPosition(float x, float y, float z);
    void setAngles(const glm::vec3 &newAngles);
    void setAngles(float angleX, float angleY, float angleZ);

    void addAngleY(float value);
    void addAngleX(float value);
    void addAngleZ(float value);

    int getObjectID() const;
};