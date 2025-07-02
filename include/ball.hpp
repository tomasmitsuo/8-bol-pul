#pragma once

#include <string>
#include <cmath>

#include <glm/vec4.hpp>

#include "table.hpp"

enum class ObjectID : int;

class Ball {
public:
    static constexpr float RADIUS = 0.13f;
    static constexpr float DIAMETER = RADIUS * 2.0f;
    static constexpr float ROW_SPACING = 1.7320508076f * RADIUS; // std::sqrt(3.0f) * RADIUS;

    static constexpr float TRIANGLE_CENTER_X = Table::CENTER_X;
    static constexpr float TRIANGLE_OFFSET_Z = 0.8f;
    static constexpr float TRIANGLE_CENTER_Z = Table::CENTER_Z - Table::LENGTH / 3 + TRIANGLE_OFFSET_Z;

    static constexpr float WHITE_BALL_X = TRIANGLE_CENTER_X;
    static constexpr float WHITE_BALL_Z = Table::CENTER_Z + Table::LENGTH / 3 - TRIANGLE_OFFSET_Z;

    static constexpr float FRICTION = 0.99f;
    static constexpr float STABLE_ROTATION_THRESHOLD = 0.05f;

public:
    float x, z;
    float vx, vz;
    float radius;
    bool isPocketed;
    
    float rotationAngle;
    glm::vec4 rotationAxis;
    int object_id;

    Ball(ObjectID id, float x, float z, float vx, float vz, float radius);
    Ball(ObjectID id, float x, float z, float radius);
    Ball(ObjectID id, glm::vec2 position, float radius);

    void update(float dt);
    void reflectOnWalls(const Table& table);
    bool isMoving() const;
    void pocket();
    void unpocket();

    float getBallSpeed();
    float getBallPositionX();
    float getBallPositionZ();

    float getRotationAngle() const;
    glm::vec4 getRotationAxis() const;

    bool isCollidingWith(const Ball& other) const;
    void handleCollision(Ball& other);

    int getObjectID() const;
};
