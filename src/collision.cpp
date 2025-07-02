#include "ball.hpp"
#include "cuestick.hpp"
#include "table.hpp"
#include "matrices.h"


#include <cmath>
#include <iostream>
#include <glm/glm.hpp>


//! Start of Sphere-Wall(Plane) collision
void Ball::reflectOnWalls(const Table& table)
{
    const float table_center_x = table.getCenterX();
    const float halfTableWidth = table.getWidth() / 2;
    if (position.x - radius < table_center_x - halfTableWidth || position.x + radius > table_center_x + halfTableWidth) {
        velocity.x *= -FRICTION;
    }
    const float table_center_z = table.getCenterZ();
    const float halfTableLength = table.getLength() / 2;
    if (position.z - radius < table_center_z - halfTableLength || position.z + radius > table_center_z + halfTableLength) {
        velocity.z *= -FRICTION;
    }
}
//! End of Sphere-Wall(Plane) collision

//! Start of Sphere-Sphere collision
bool Ball::isCollidingWith(const Ball& other) const
{
    const glm::vec4 direction = position - other.position;
    float distance_squared = dotproduct(direction, direction);
    float radius_sum = radius + other.radius;
    return distance_squared < (radius_sum * radius_sum);
}

void Ball::handleCollision(Ball& other)
{
    glm::vec4 direction = other.position - position;

    float distance = glm::length(direction);
    if (distance == 0.0f) {
        direction += glm::vec4(0.01f, 0.0f, 0.01f, 0.0f);
        distance = glm::length(direction);
    }

    const glm::vec4 normal = direction / distance;

    const float overlap = (radius + other.radius) - distance;
    const glm::vec4 bump_direction = normal * (overlap * 0.5f);
    position -= bump_direction;
    other.position += bump_direction;

    const glm::vec4 v1_normal = normal * dotproduct(velocity, normal);
    const glm::vec4 v2_normal = normal * dotproduct(other.velocity, normal);

    const glm::vec4 v1_tanget = velocity - v1_normal;
    const glm::vec4 v2_tanget = other.velocity - v2_normal;

    velocity = v2_normal + v1_tanget;
    other.velocity = v1_normal + v2_tanget;
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

        force_vec = (force_vec / norm(force_vec)) * shotPower * Cuestick::SHOT_POWER_MULTIPLIER;

        // Apply force to the white ball
        white_ball.applyForce(glm::vec4(force_vec.x, 0.0f, force_vec.y, 0.0f));

        // Reset to Aiming state
        state = CueState::Aiming;
        horizontalOffset = 0.0f;
    }

    // Update position during the shot animation
    const glm::vec4 ball_position = white_ball.getPosition();
    this->x = ball_position.x + dir_vec.x * (Cuestick::DISTANCE + std::max(0.0f, pullBackDistance)) - sidestep_vec.x * horizontalOffset;
    this->z = ball_position.z + dir_vec.y * (Cuestick::DISTANCE + std::max(0.0f, pullBackDistance)) - sidestep_vec.y * horizontalOffset;
}

//! Start of Sphere-Circle collision
void Table::update(std::vector<Ball>& balls) {
    for (size_t i = 0; i < balls.size(); ++i) {
        Ball& ball = balls[i];

        if (ball.isPocketed()) {
            continue;
        }

        for (const auto& holePos : holePositions) {
            const glm::vec4 ball_position = ball.getPosition();
            float dx = ball_position.x - holePos.x;
            float dz = ball_position.z - holePos.y;
            float distanceSquared = dx * dx + dz * dz;

            if (distanceSquared < (this->holeRadius * this->holeRadius)) {
                if (i == 0) {
                    std::cout << "White ball scratched!" << std::endl;
                    ball.resetBallTo(glm::vec2(Ball::WHITE_BALL_X, Ball::WHITE_BALL_Z));
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