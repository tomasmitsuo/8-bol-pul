#pragma once

#include "table.hpp"
#include "ball.hpp"
#include "camera.hpp"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

enum class ObjectID : int;

class Cuestick {
public:
    static constexpr float ANGLE = -100.0f * 3.14159265359 / 180.0f;
    static constexpr float HEIGHT = Table::HEIGHT - 0.4f;

    static constexpr float CHARGE_SPEED = 5.0f;
    static constexpr float MAX_PULL_BACK = 3.0f;
    static constexpr float SHOOT_SPEED = 25.0f;
    static constexpr float SHOT_POWER_MULTIPLIER = 5.0f;

    static constexpr float STRAFE_SPEED = 0.25f;

    static constexpr float ROTATION_SPEED = 0.5f;

    static constexpr float HORIZONTAL_EFFECT_SCALE = 1.0f;
    static constexpr float VERTICAL_UP_EFFECT_SCALE = 1.0f;
    static constexpr float VERTICAL_DOWN_EFFECT_SCALE = 0.3f;

public:
    enum class CueState
    {
        Aiming,
        Charging,
        Shooting
    };

private:
    glm::vec4 position;
    glm::vec3 angles;

    int object_id;

    CueState state;
    float pullBackDistance;
    float prev_pullBackDistance;
    float shotPower;
    float horizontalOffset;
    float verticalOffset;
    const float followRadius;

    bool last_right_button;

    void clampAimingOffsets();

public:
    Cuestick(ObjectID id, const glm::vec3 &position, const glm::vec3 &angles, const float followRadius);
    Cuestick(ObjectID id, float x, float y, float z, float angleX, float angleY, float angleZ, float followRadius);

    void update(float deltaTime, Ball& whiteBall, const Camera& camera);
    void control(float delta_time, bool right_mouse_button, bool middle_mouse_button, bool go_front, bool go_back, bool go_left, bool go_right, bool strafe_left, bool strafe_right, const Camera &camera, const Ball &white_ball);

    void calculateShooting(Ball &white_ball);

    void startCharging();
    void shoot();

    void strafeHorizontal(float distance);
    void strafeVertical(float distance);
    void resetAim();

    glm::vec4 getPosition() const;
    glm::vec3 getAngles() const;
    CueState getState() const;
    bool isAiming() const;

    void setPosition(const glm::vec4 &newPosition);
    void setPosition(float x, float y, float z);
    void setAngles(const glm::vec3 &newAngles);
    void setAngles(float angleX, float angleY, float angleZ);

    void addAngleY(float value);
    void addAngleX(float value);
    void addAngleZ(float value);

    int getObjectID() const;
};