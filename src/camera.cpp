#include "camera.hpp"

#include <cmath>

#include "matrices.h"


Camera::Camera(const Ball &targetBall, float camera_height, float camera_speed, float distance, float theta, float phi, float nearPlane, float farPlane, float fieldOfView)
    : theta(theta), phi(phi), distance(distance),
        camera_height(camera_height), camera_speed(camera_speed),
      nearPlane(nearPlane), farPlane(farPlane), fieldOfView(fieldOfView),
      using_perspective_projection(true), using_look_at_camera(true),
      targetBall(targetBall) {
    updatePosition(0.0f, false, false, false, false);
}

void Camera::updatePosition(float delta_time, bool go_front, bool go_back, bool go_left, bool go_right) {
    upVector = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

    if (using_look_at_camera){
        float y = distance * std::sin(phi);
        float x = targetBall.x + distance * std::sin(theta) * std::cos(phi);
        float z = targetBall.z + distance * std::cos(theta) * std::cos(phi);
        position = glm::vec4(x, y, z, 1.0f);
        viewVector = lookAt - position;
        lookAt = glm::vec4(targetBall.x, camera_height, targetBall.z, 1.0f);
    }
    else {
        float y = std::sin(phi);
        float x = std::sin(theta) * std::cos(phi);
        float z = std::cos(theta) * std::cos(phi);
        viewVector = -glm::vec4(x, y, z, 0.0f);

        glm::vec4 u_vector = -crossproduct(viewVector, upVector);
        const bool is_u_null = (u_vector.x == 0.0f && u_vector.y == 0.0f && u_vector.z == 0.0f);
        u_vector = is_u_null ? u_vector : u_vector / norm(u_vector); // Normaliza o vetor u, evitando movimento não unitário
        
        const float speed = camera_speed * delta_time;
        if (go_front) {
            position += speed * viewVector;
        }
        if (go_back) {
            position -= speed * viewVector;
        }
        if (go_right) {
            position += speed * u_vector;
        }
        if (go_left) {
            position -= speed * u_vector;
        }
    }
}

glm::vec4 Camera::getPosition() const {
    return position;
}

glm::vec4 Camera::getLookAt() const {
    return lookAt;
}

glm::vec4 Camera::getUpVector() const {
    return upVector;
}

glm::vec4 Camera::getViewVector() const {
    return viewVector;
}

glm::mat4 Camera::getViewMatrix() const {
    const glm::vec4 view_vector = getViewVector();
    return Matrix_Camera_View(position, view_vector, upVector);
}

glm::mat4 Camera::getProjectionMatrix(float screenRatio) const {
    if (using_perspective_projection)
    {
        return Matrix_Perspective(fieldOfView, screenRatio, nearPlane, farPlane);
    }
    else
    {
        float t = 1.5f * distance / 2.5f;
        float b = -t;
        float r = t * screenRatio;
        float l = -r;
        return Matrix_Orthographic(l, r, b, t, nearPlane, farPlane);
    }
}

bool Camera::isUsingLookAtCamera() const {
    return using_look_at_camera;
}

void Camera::setUsingPerspectiveProjection(bool value) {
    using_perspective_projection = value;
}

bool Camera::isUsingPerspectiveProjection() {
    return using_perspective_projection;
}

float Camera::getTheta() const {
    return theta;
}

float Camera::getPhi() const {
    return phi;
}

float Camera::getDistance() const {
    return distance;
}

void Camera::setTheta(float newTheta) {
    theta = newTheta;
}

void Camera::setPhi(float newPhi) {
    phi = newPhi;
}

void Camera::setDistance(float newDistance) {
    distance = newDistance;
}

void Camera::addTheta(float deltaTheta) {
    theta += deltaTheta;
}

void Camera::addPhi(float deltaPhi) {
    phi += deltaPhi;
}

void Camera::addDistance(float deltaDistance) {
    distance += deltaDistance;
}

void Camera::setCameraType(bool use_look_at_camera) {
    using_look_at_camera = use_look_at_camera;
}