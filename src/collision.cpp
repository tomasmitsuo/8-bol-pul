#include "ball.hpp"
#include "cuestick.hpp"
#include "table.hpp"
#include "matrices.h"


#include <cmath>
#include <iostream>
#include <glm/glm.hpp>


//! Start of Sphere-Wall(Plane) collision
void Ball::reflectOnWalls(float table_center_x, float table_center_z, float tableWidth, float tableHeight)
{
    if (x - radius < table_center_x - tableWidth / 2 || x + radius > table_center_x + tableWidth / 2) {
        vx *= -FRICTION;
    }
    if (z - radius < table_center_z - tableHeight / 2 || z + radius > table_center_z + tableHeight / 2) {
        vz *= -FRICTION;
    }
}
//! End of Sphere-Wall(Plane) collision

//! Start of Sphere-Sphere collision
bool Ball::isCollidingWith(const Ball& other) const
{
    float dx = x - other.x;
    float dz = z - other.z;
    float distance_squared = dx * dx + dz * dz;
    float radius_sum = radius + other.radius;
    return distance_squared < (radius_sum * radius_sum);
}

void Ball::handleCollision(Ball& other)
{
    float dx = other.x - x;
    float dz = other.z - z;

    float distance = std::sqrt(dx * dx + dz * dz);
    if (distance == 0.0f) {
        dx = 0.01f;
        distance = 0.01f;
    }

    const float nx = dx / distance;
    const float nz = dz / distance;

    const float overlap = (radius + other.radius) - distance;

    x -= overlap * nx * 0.5f;
    z -= overlap * nz * 0.5f;
    other.x += overlap * nx * 0.5f;
    other.z += overlap * nz * 0.5f;

    const float tx = -nz;
    const float tz = nx;

    const float v1n = vx * nx + vz * nz;
    const float v1t = vx * tx + vz * tz;
    const float v2n = other.vx * nx + other.vz * nz;
    const float v2t = other.vx * tx + other.vz * tz;

    const float v1n_final = v2n;
    const float v2n_final = v1n;

    vx = (v1n_final * nx) + (v1t * tx);
    vz = (v1n_final * nz) + (v1t * tz);
    other.vx = (v2n_final * nx) + (v2t * tx);
    other.vz = (v2n_final * nz) + (v2t * tz);
}
//! End of Sphere-Sphere collision

void Cuestick::calculateShooting(float deltaTime, Ball& white_ball,const glm::vec2& dir_vec, const glm::vec2& sidestep_vec)
{
    // Move the cue stick forward rapidly
    pullBackDistance -= Cuestick::SHOOT_SPEED * deltaTime;

    // Check for collision
    if (pullBackDistance <= 0.0f) {
        std::cout << "Impact!" << std::endl;

        glm::vec2 force_vec = -dir_vec;

        force_vec += sidestep_vec * horizontalOffset * Cuestick::SIDESTEP_FACTOR;

        force_vec = force_vec / norm(force_vec);

        // Apply force to the white ball
        white_ball.vx = force_vec.x * shotPower * Cuestick::SHOT_POWER_MULTIPLIER;
        white_ball.vz = force_vec.y * shotPower * Cuestick::SHOT_POWER_MULTIPLIER;

        // Reset to Aiming state
        state = CueState::Aiming;
        horizontalOffset = 0.0f;
    }

    // Update position during the shot animation
    this->x = white_ball.x + dir_vec.x * (Cuestick::DISTANCE + std::max(0.0f, pullBackDistance)) - sidestep_vec.x * horizontalOffset;
    this->z = white_ball.z + dir_vec.y * (Cuestick::DISTANCE + std::max(0.0f, pullBackDistance)) - sidestep_vec.y * horizontalOffset;
}

//! Start of Sphere-Circle collision
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
                    ball.x = Ball::WHITE_BALL_X;
                    ball.z = Ball::WHITE_BALL_Z;
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
//! End of Sphere-Circle collision