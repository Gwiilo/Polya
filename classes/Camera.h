#ifndef CAMERA_H
#define CAMERA_H

#include "../classes/Vector3.h"
#include "../classes/Quaternion.h"
#include "../classes/Matrix4.h"

class Camera {
public:
    Vector3 position;
    Quaternion orientation;
    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;

    Camera(const Vector3& pos = Vector3(0.0f, 0.0f, 0.0f),
           const Quaternion& orient = Quaternion(0.0f, 0.0f, 0.0f, 1.0f),
           float fov = 70.0f, float aspectRatio = 1.0f,
           float nearPlane = 0.1f, float farPlane = 100.0f)
        : position(pos), orientation(orient), fov(fov), aspectRatio(aspectRatio),
          nearPlane(nearPlane), farPlane(farPlane) {}

    // Getters for configuration values
    float getFOV() const { return fov; }
    float getAspectRatio() const { return aspectRatio; }
    float getNearPlane() const { return nearPlane; }
    float getFarPlane() const { return farPlane; }

    Matrix4 getViewMatrix() const {
        Matrix4 rotation = Matrix4::createFromQuaternion(orientation);
        Matrix4 translation = Matrix4::createTranslation(Vector3(-position.x, -position.y, -position.z));
        return rotation * translation;
    }

    Matrix4 getProjectionMatrix() const {
        return Matrix4::createPerspective(fov, aspectRatio, nearPlane, farPlane);
    }

    void rotateYawPitch(float yaw, float pitch) {
        Quaternion yawRotation = Quaternion::fromAxisAngle(Vector3(0, 1, 0), yaw);
        Quaternion pitchRotation = Quaternion::fromAxisAngle(Vector3(1, 0, 0), pitch);
        orientation = (yawRotation * orientation * pitchRotation).normalize();
    }

    Vector3 getForward() const {
        return orientation.rotate(Vector3(0, 0, -1));
    }

    Vector3 getRight() const {
        return orientation.rotate(Vector3(1, 0, 0));
    }
};

#endif
