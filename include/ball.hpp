#pragma once

#include <string>

class Ball {
public:
    float x, z;
    float vx, vz;
    float radius;


    Ball(float x, float z, float vx, float vz, float radius);

    void update(float dt);
    void reflectOnWalls(float tableWidth, float tableHeight);
    bool isMoving() const;

    float getBallSpeed();
    float getBallPositionX();
    float getBallPositionZ();
};
