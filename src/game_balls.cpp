#include "game_balls.hpp"

#include "load_objects.hpp"

#include <iostream>

GameBalls::GameBalls(const float ball_radius, const glm::vec2 &white_ball_position, const glm::vec2 &triangle_center, const float row_spacing)
    : ball_radius(ball_radius), white_ball_position(white_ball_position), triangle_center(triangle_center), row_spacing(row_spacing)
{
    reset();
}

void GameBalls::reset()
{
    balls.clear();

    balls.reserve(GameBalls::TOTAL_BALLS);

    balls.emplace_back(ObjectID::WHITE_BALL, white_ball_position, ball_radius);
    int balls_created = 0; // Start with the white ball
    for (int row = 0; row < GameBalls::NUM_ROWS; ++row) {
        const float row_offset = -row * row_spacing;

        const float row_collumn_start = row * ball_radius;

        const int balls_in_row = row + 1;
        for (int col = 0; col < balls_in_row; ++col)
        {
            const float coll_offset = col * ball_radius * 2.0f - row_collumn_start;

            const glm::vec2 position = triangle_center + glm::vec2(coll_offset, row_offset);
            const ObjectID ball_id = static_cast<ObjectID>(static_cast<int>(ObjectID::BALL1) + balls_created++);
            balls.emplace_back(
                ball_id,
                position,
                ball_radius
            );
        }
    }
}

std::vector<Ball> &GameBalls::getBalls()
{
    return balls;
}

Ball &GameBalls::getWhiteBall()
{
    return balls[WHITE_BALL];
}

void GameBalls::update(float deltaTime, const Table &table)
{
    for (size_t i = 0; i < balls.size(); ++i) {
        Ball & curr_ball = balls[i];

        if (curr_ball.hasCollision())
        {
            for (size_t j = i + 1; j < balls.size(); ++j) {
                Ball & other_ball = balls[j];
                if (curr_ball.isCollidingWith(other_ball)) {
                    curr_ball.handleCollision(other_ball);
                }
            }

            if (curr_ball.isMoving())
            {
                curr_ball.reflectOnWalls(deltaTime, table);
            }
        }

        curr_ball.update(deltaTime);
    }
}