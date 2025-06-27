#pragma once
#include <vector>
#include "ball.hpp"
#include "cuestick.hpp"

void loadAllTextures(void);
void loadAllObjects(void);
void drawInitialScene(const std::vector<Ball>& vec_balls, const Cuestick& cuestick);