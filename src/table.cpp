#include "table.hpp"

#include "ball.hpp"

#include <cmath>
#include <iostream>

Table::Table(float centerX, float centerZ, float width, float length, float holeRadius)
    : holeRadius(holeRadius),
      centerX(centerX),
      centerZ(centerZ),
      width(width),
      length(length) {
    
    // Calculate the boundaries of the table
    const float halfWidth = width / 2.0f;
    const float halfLength = length / 2.0f;
    const float left = centerX - halfWidth;
    const float right = centerX + halfWidth;
    const float top = centerZ - halfLength;
    const float bottom = centerZ + halfLength;

    // Define the 6 hole positions
    // Corner holes
    holePositions.push_back(glm::vec2(left, top));
    holePositions.push_back(glm::vec2(right, top));
    holePositions.push_back(glm::vec2(left, bottom));
    holePositions.push_back(glm::vec2(right, bottom));

    const float middle_hole_offset = holeRadius * 0.40f;

    // Middle holes
    holePositions.push_back(glm::vec2(left - middle_hole_offset, centerZ));
    holePositions.push_back(glm::vec2(right + middle_hole_offset, centerZ));
}

float Table::getCenterX() const {
    return centerX;
}

float Table::getCenterZ() const {
    return centerZ;
}

float Table::getWidth() const {
    return width;
}

float Table::getLength() const {
    return length;
}
