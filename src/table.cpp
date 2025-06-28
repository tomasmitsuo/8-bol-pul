#include "table.hpp"
#include "obj_constants.hpp"
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

void Table::update(std::vector<Ball>& balls) {
    for (size_t i = 0; i < balls.size(); ++i) {
        Ball& ball = balls[i];

        if (ball.isPocketed) {
            continue;
        }

        for (const auto& holePos : holePositions) {
            float dx = ball.x - holePos.x;
            float dz = ball.z - holePos.y;
            float distanceSquared = dx * dx + dz * dz;

            if (distanceSquared < (this->holeRadius * this->holeRadius)) {
                if (i == 0) {
                    std::cout << "White ball scratched!" << std::endl;
                    ball.x = WHITE_BALL_X;
                    ball.z = WHITE_BALL_Z;
                    ball.vx = 0.0f;
                    ball.vz = 0.0f;
                } else {
                    std::cout << "Ball " << i << " pocketed!" << std::endl;
                    ball.pocket();
                }
                
                break; // Ball is handled, move to the next ball
            }
        }
    }
}