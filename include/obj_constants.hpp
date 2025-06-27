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

constexpr float CAMERA_SPEED = 1.0f; // Speed of the camera movement
constexpr float CAMERA_HEIGHT = TABLE_HEIGHT + 0.1f; // Height of the camera
constexpr float CAMERA_DISTANCE = 3.5f; // Distance of the camera from the target ball
constexpr float CAMERA_THETA = 0.0f; // Angle theta (in the ZX plane)
constexpr float CAMERA_PHI = 0.6f; // Angle phi (relative to the Y axis)
constexpr float NEAR_PLANE = -0.1f; // Position of the near plane
constexpr float FAR_PLANE = -100.0f; // Position of the far plane
constexpr float FIELD_OF_VIEW = 3.141592f / 3.0f;