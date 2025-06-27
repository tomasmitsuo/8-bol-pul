#pragma once


class Cuestick {
private:
    float x;        // Posição X do taco
    float y;        // Posição Y do taco
    float z;        // Posição Z do taco
    float angleY;   // Ângulo de rotação em torno do eixo Y
    float angleX;   // Ângulo de rotação em torno do eixo X
    float angleZ;   // Ângulo de rotação em torno do eixo Z

public:
    Cuestick();

    float getX() const;

    float getZ() const;

    float getY() const;

    float getAngleY() const;

    float getAngleX() const;

    float getAngleZ() const;
};