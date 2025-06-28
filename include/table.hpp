#pragma once

#include <vector>
#include <glm/vec2.hpp>
#include "ball.hpp"

class Table {
private:
    std::vector<glm::vec2> holePositions;
    float holeRadius;

public:
    Table(float centerX, float centerZ, float width, float length, float holeRadius);

    void update(std::vector<Ball>& balls);
};