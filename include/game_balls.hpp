#pragma once

#include <vector>
#include "ball.hpp"
#include <glm/vec2.hpp>

class GameBalls {
public:
    static constexpr int RACK_BALLS = 15;
    static constexpr int TOTAL_BALLS = RACK_BALLS + 1;
    static constexpr int NUM_ROWS = 5;

private:
    std::vector<Ball> balls;
    static constexpr int WHITE_BALL = 0;
    const float ball_radius;
    const glm::vec2 white_ball_position;
    const glm::vec2 triangle_center;
    const float row_spacing;

public:
    GameBalls(const float ball_radius, const glm::vec2 &white_ball_position, const glm::vec2 &triangle_center, const float row_spacing);

    void reset();

    std::vector<Ball> &getBalls();

    Ball &getWhiteBall();

    void update(float deltaTime, const Table &table);
};