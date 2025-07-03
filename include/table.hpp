#pragma once

#include <vector>
#include <glm/vec2.hpp>
#include <string>

class Ball; // Forward declaration to avoid circular dependency

class Table {
public:
    static constexpr float HEIGHT = 1.5f;
    static constexpr float WIDTH = 3.1f * 2;
    static constexpr float LENGTH = 5.5f * 2;
    static constexpr float CENTER_X = 0.4f;
    static constexpr float CENTER_Z = 4.0f;
    static constexpr float HOLE_RADIUS = 0.25f;
    static constexpr float BOUNCINESS = 0.2f;

private:
    std::vector<glm::vec2> holePositions;
    float holeRadius;
    float centerX;
    float centerZ;
    float width;
    float length;

public:
    Table(float centerX, float centerZ, float width, float length, float holeRadius);

    std::vector<std::string> update(std::vector<Ball>& balls);

    float getCenterX() const;
    float getCenterZ() const;
    float getWidth() const;
    float getLength() const;
};