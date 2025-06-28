#include "table.hpp"

#include "ball.hpp"

#include <cmath>
#include <iostream>

Table::Table(float centerX, float centerZ, float width, float length, float holeRadius)
    : holeRadius(holeRadius) {
    
    // Calculate the boundaries of the table
    float halfWidth = width / 2.0f;
    float halfLength = length / 2.0f;
    float left = centerX - halfWidth;
    float right = centerX + halfWidth;
    float top = centerZ - halfLength;
    float bottom = centerZ + halfLength;

    // Define the 6 hole positions
    // Corner holes
    holePositions.push_back(glm::vec2(left, top));
    holePositions.push_back(glm::vec2(right, top));
    holePositions.push_back(glm::vec2(left, bottom));
    holePositions.push_back(glm::vec2(right, bottom));

    // Middle holes
    holePositions.push_back(glm::vec2(left, centerZ));
    holePositions.push_back(glm::vec2(right, centerZ));
}
