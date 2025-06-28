#pragma once

#include "ball.hpp"

class Cuestick {

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

    CueState state;
    float pullBackDistance;
    float shotPower;

public:
    Cuestick(float x, float y, float z, float angleX, float angleY, float angleZ);

    void update(float deltaTime, Ball& whiteBall);

    void startCharging();
    void shoot();

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
};