#pragma once

#include <string>

#include <glm/vec4.hpp>

enum class ObjectID : int;

class Ball {
public:
    float x, z;
    float vx, vz;
    float radius;
    int object_id;

    float rotationAngle;
    glm::vec4 rotationAxis;

    Ball(ObjectID id, float x, float z, float vx, float vz, float radius);
    Ball(ObjectID id, float x, float z, float radius);

    void update(float dt);
    void reflectOnWalls(float table_center_x, float table_center_z, float tableWidth, float tableHeight);
    bool isMoving() const;

    float getBallSpeed();
    float getBallPositionX();
    float getBallPositionZ();

    float getRotationAngle() const;
    glm::vec4 getRotationAxis() const;

    bool isCollidingWith(const Ball& other) const;
    void handleCollision(Ball& other);

    int getObjectID() const;
};
