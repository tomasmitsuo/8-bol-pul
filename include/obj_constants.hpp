#pragma once

#include <cmath>

constexpr float TABLE_HEIGHT = 1.5f;
constexpr float TABLE_WIDTH = 3.5f*2;
constexpr float TABLE_LENGTH = 5.5f*2;
constexpr float TABLE_CENTER_X = 0.4f;
constexpr float TABLE_CENTER_Z = 4.0f;

constexpr float BALL_RADIUS = 0.13f;
constexpr float BALL_DIAMITER = BALL_RADIUS * 2.0f;
const float ROW_SPACING = std::sqrt(3.0f) * BALL_RADIUS;

constexpr float TRIANGULE_CENTER_X = TABLE_CENTER_X;
constexpr float TRIANGULE_OFFSET_Z = 0.8f;
constexpr float TRIANGULE_CENTER_Z = TABLE_CENTER_Z - TABLE_CENTER_Z + TRIANGULE_OFFSET_Z;

constexpr float WHITE_BALL_X = TRIANGULE_CENTER_X;
constexpr float WHITE_BALL_Z = TABLE_CENTER_Z + TABLE_CENTER_Z;

constexpr float MAX_SHOT_STRENGTH = 5.0f;
constexpr float STRENGTH_INCREMENT = 2.5f;