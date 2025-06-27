#pragma once

#include "ball.hpp"
#include <vector>
#include <tuple>


bool balls_are_colliding(const Ball& ball1, const Ball& ball2);

void handle_ball_collisions(std::vector<Ball>& balls,
                            const std::pair<float, float> & tableCenter,
                            const float tableWidth,
                            const float tableLength);