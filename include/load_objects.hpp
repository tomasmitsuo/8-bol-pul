#pragma once
#include <vector>
#include "ball.hpp"
#include "cuestick.hpp"
#include "camera.hpp"

enum class ObjectID : int {
    WHITE_BALL,
    BALL1,
    BALL2,
    BALL3,
    BALL4,
    BALL5,
    BALL6,
    BALL7,
    BALL8,
    BALL9,
    BALL10,
    BALL11,
    BALL12,
    BALL13,
    BALL14,
    BALL15,
    TABLE,
    CUESTICK
};

void loadAllTextures(void);
void loadAllObjects(void);
void drawInitialScene(const std::vector<Ball>& vec_balls, const Cuestick& cuestick, const Camera& camera, bool isWhiteBallMoving);