#include "collision.hpp"

#include <cmath>
#include <iostream>

bool balls_are_colliding(const Ball& ball1, const Ball& ball2) {
    float dx = ball1.x - ball2.x;
    float dz = ball1.z - ball2.z;
    float distance_squared = dx * dx + dz * dz;
    float radius_sum = ball1.radius + ball2.radius;
    return distance_squared < (radius_sum * radius_sum);
}

void handle_ball_collisions(std::vector<Ball>& balls,
                            const std::pair<float, float> & tableCenter,
                            const float tableWidth,
                            const float tableLength) {
    for (size_t i = 0; i < balls.size(); ++i) {
        auto & main_ball = balls[i];
        for (size_t j = i + 1; j < balls.size(); ++j) {
            // continue;
            auto & secondary_ball = balls[j];
            if (balls_are_colliding(main_ball, secondary_ball)) {
                // std::cout << "Collision detected between ball " << i << " and ball " << j << std::endl;

                float dx = secondary_ball.x - main_ball.x;
                float dz = secondary_ball.z - main_ball.z;

                float distance = std::sqrt(dx * dx + dz * dz);
                if (distance == 0.0f) {
                    dx = 0.01f;
                    distance = 0.01f;
                }

                const float nx = dx / distance;
                const float nz = dz / distance;

                const float overlap = (main_ball.radius + secondary_ball.radius) - distance;

                main_ball.x -= overlap * nx * 0.5f;
                main_ball.z -= overlap * nz * 0.5f;
                secondary_ball.x += overlap * nx * 0.5f;
                secondary_ball.z += overlap * nz * 0.5f;


                const float tx = -nz;
                const float tz = nx;

                const float v1n = main_ball.vx * nx + main_ball.vz * nz;
                const float v1t = main_ball.vx * tx + main_ball.vz * tz;
                const float v2n = secondary_ball.vx * nx + secondary_ball.vz * nz;
                const float v2t = secondary_ball.vx * tx + secondary_ball.vz * tz;

                const float v1n_final = v2n;
                const float v2n_final = v1n;


                main_ball.vx = (v1n_final * nx) + (v1t * tx);
                main_ball.vz = (v1n_final * nz) + (v1t * tz);
                secondary_ball.vx = (v2n_final * nx) + (v2t * tx);
                secondary_ball.vz = (v2n_final * nz) + (v2t * tz);
            }
        }

        // Reflect balls on table walls
        main_ball.reflectOnWalls(tableCenter.first, tableCenter.second, tableWidth, tableLength);
    }
}