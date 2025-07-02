// TODO: CRIAR UMA CLASSE PARA A CAMERA
#pragma once

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "table.hpp"
#include "ball.hpp"

class Camera {
public:
    static constexpr float SPEED = 2.5f; // Speed of the camera movement
    static constexpr float HEIGHT = 0.5f; // Height of the camera
    static constexpr float DISTANCE = 3.5f; // Distance of the camera from the target ball
    static constexpr float THETA = 0.0f; // Angle theta (in the ZX plane)
    static constexpr float PHI = 0.6f; // Angle phi (relative to the Y axis)
    static constexpr float NEAR_PLANE = -0.1f; // Position of the near plane
    static constexpr float FAR_PLANE = -100.0f; // Position of the far plane
    static constexpr float FIELD_OF_VIEW = 3.141592f / 3.0f; // Field of view in radians

private:
    glm::vec4 position;
    glm::vec4 lookAt;
    glm::vec4 upVector;
    glm::vec4 projection;
    glm::vec4 viewVector;

    float theta;
    float phi;
    float distance;

    float camera_height;
    float camera_speed;
    
    float nearPlane;
    float farPlane;
    float fieldOfView;

    bool using_perspective_projection;
    bool using_look_at_camera;

    const Ball& targetBall;

public:
    Camera(const Ball &targetBall, float camera_height, float camera_speed, float distance, float theta, float phi, float nearPlane, float farPlane, float fieldOfView);

    void control(float delta_time, bool go_front, bool go_back, bool go_left, bool go_right);

    glm::vec4 getPosition() const;
    glm::vec4 getLookAt() const;
    glm::vec4 getUpVector() const;
    glm::vec4 getViewVector() const;

    glm::mat4 getViewMatrix() const;

    glm::mat4 getProjectionMatrix(float screenRatio) const;

    bool isUsingLookAtCamera() const;

    void setUsingPerspectiveProjection(bool value);

    bool isUsingPerspectiveProjection();

    float getTheta() const;
    float getPhi() const;
    float getDistance() const;

    void setTheta(float newTheta);
    void setPhi(float newPhi);
    void setDistance(float newDistance);

    void addTheta(float deltaTheta);
    void addPhi(float deltaPhi);
    void addDistance(float deltaDistance);

    void setCameraType(bool use_look_at_camera);
};
