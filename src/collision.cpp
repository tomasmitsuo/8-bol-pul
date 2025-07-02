#include "ball.hpp"
#include "cuestick.hpp"
#include "table.hpp"
#include "matrices.h"
#include "game_balls.hpp"
#include "load_objects.hpp"


#include <cmath>
#include <iostream>
#include <glm/glm.hpp>


//! Start of Sphere-Wall(Plane/Box) collision
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
//! End of Sphere-Wall(Plane/Box) collision

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

    float distance = norm(direction);
    if (distance == 0.0f) {
        direction += glm::vec4(0.01f, 0.0f, 0.01f, 0.0f);
        distance = norm(direction);
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

//! Start of Sphere-Ray collision
void Cuestick::calculateShooting(Ball& white_ball)
{
    const glm::vec4 ball_center = white_ball.getPosition();
    const float ball_radius = white_ball.getRadius();
    const float yaw = angles.y;
    const float pitch = angles.x;

    const glm::vec4 dir = glm::vec4(
        std::cos(pitch) * std::sin(yaw),
        std::sin(pitch),
        std::cos(pitch) * std::cos(yaw),
        0.0f
    );

    const glm::vec4 right = glm::vec4(-std::cos(yaw), 0.0f, std::sin(yaw), 0.0f);
    const glm::vec4 up = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

    const float backwardDistance = followRadius + std::max(0.0f, pullBackDistance);
    const glm::vec4 centered_handle_pos = ball_center + glm::vec4(std::sin(yaw), 0.0f, std::cos(yaw), 0.0f) * backwardDistance;
    
    const glm::vec4 offset_vector = this->position - centered_handle_pos;
    const float effective_horizontal_offset = dotproduct(offset_vector, right);
    const float effective_vertical_offset = this->position.y - Cuestick::HEIGHT;

    // Different vertical scales for up and down effects
    const float vertical_scale = (effective_vertical_offset > 0) ? Cuestick::VERTICAL_UP_EFFECT_SCALE : Cuestick::VERTICAL_DOWN_EFFECT_SCALE;
    // Sum of all offsets, offset subtracted to get the direction the ball should go (opposite of hit)
    const glm::vec4 aim_target = ball_center
                                - right * effective_horizontal_offset * Cuestick::HORIZONTAL_EFFECT_SCALE
                                - up * effective_vertical_offset * vertical_scale;

    const glm::vec4 tip_start_pos = aim_target - dir * (ball_radius + prev_pullBackDistance);
    const glm::vec4 tip_end_pos = aim_target - dir * (ball_radius + pullBackDistance);
    const glm::vec4 ray_direction = tip_end_pos - tip_start_pos;

    // Fix for preventing the cue from crossing the ball in one frame
    // Solve the quadratic equation for the intersection of the ray with the sphere
    const glm::vec4 ball_to_tip = tip_start_pos - ball_center;
    const float quad_a = dotproduct(ray_direction, ray_direction);
    const float quad_b = 2.0f * dotproduct(ray_direction, ball_to_tip);
    const float quad_c = dotproduct(ball_to_tip, ball_to_tip) - (ball_radius * ball_radius);
    const float delta = quad_b * quad_b - 4.0f * quad_a * quad_c;

    // Check for collision
    if (delta >= 0.0f)
    {
        const float t = (-quad_b - std::sqrt(delta)) / (2.0f * quad_a);
        if (t >= 0.0f && t <= 1.0f)
        {
            std::cout << "Impact!" << std::endl;

            const glm::vec4 impact_point = tip_start_pos + ray_direction * t;

            glm::vec4 force_direction = impact_point - ball_center;

            if (norm(force_direction) > 0.0f) {
                glm::vec4 final_force = (force_direction / norm(force_direction)) * shotPower * Cuestick::SHOT_POWER_MULTIPLIER;
                white_ball.applyForce(final_force);
            }

            resetAim();
        }
    }
    else
    {
        std::cout << "Missed!" << std::endl;
        resetAim();
    }
}
//! End of Sphere-Ray collision

//! Start of Sphere-Circle collision
void Table::update(std::vector<Ball>& balls) {
    for (Ball& ball : balls) {
        if (ball.isPocketed()) {
            continue;
        }

        for (const auto& holePos : holePositions) {
            if (ball.isAnimating())
            {
                continue;
            }
            const glm::vec4 ball_position = ball.getPosition();
            float dx = ball_position.x - holePos.x;
            float dz = ball_position.z - holePos.y;
            float distanceSquared = dx * dx + dz * dz;

            if (distanceSquared < (this->holeRadius * this->holeRadius)) {
                if (ball.getObjectID() == static_cast<int>(ObjectID::WHITE_BALL)) {
                    std::cout << "White ball scratched!" << std::endl;
                } else {
                    std::cout << "Ball " << ball.getObjectID() << " pocketed!" << std::endl;
                }
                ball.startPocketAnimation(holePos);
                break; // Ball is handled, move to the next ball
            }
        }
    }
}
//! End of Sphere-Circle collision