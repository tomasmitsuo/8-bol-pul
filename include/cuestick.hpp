#pragma once

#include "table.hpp"
#include "ball.hpp"

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

public:
    enum class CueState
    {
        Aiming,
        Charging,
        Shooting
    };

private:
    float x;        // Posição X do taco
    float y;        // Posição Y do taco
    float z;        // Posição Z do taco
    float angleY;   // Ângulo de rotação em torno do eixo Y
    float angleX;   // Ângulo de rotação em torno do eixo X
    float angleZ;   // Ângulo de rotação em torno do eixo Z

    int object_id;

    CueState state;
    float pullBackDistance;
    float shotPower;
    float horizontalOffset;

public:
    Cuestick(ObjectID id, float x, float y, float z, float angleX, float angleY, float angleZ);

    void update(float deltaTime, Ball& whiteBall);

    void calculateShooting(float deltaTime, Ball &white_ball, const glm::vec2 &dir_vec, const glm::vec2 &sidestep_vec);

    void startCharging();
    void shoot();

    void addHorizontalOffset(float offset);

    float getX() const;
    float getZ() const;
    float getY() const;
    float getAngleY() const;
    float getAngleX() const;
    float getAngleZ() const;
    CueState getState() const;
    bool isAiming() const;

    void setX(float newX);
    void setY(float newY);
    void setZ(float newZ);
    void setAngleY(float newAngleY);
    void setAngleX(float newAngleX);
    void setAngleZ(float newAngleZ);

    int getObjectID() const;
};