#ifndef MATRIX4_H
#define MATRIX4_H

#include <cmath>
#include "../classes/Vector3.h"
#include "../funcs/MathFunctions.h" // For pi()

class Matrix4 {
public:
    float m[4][4] = {0}; // 4x4 matrix initialized to zero

    // Identity matrix by default
    Matrix4() {
        for (int i = 0; i < 4; ++i)
            m[i][i] = 1.0f;
    }

    Matrix4 operator*(const Matrix4& other) const {
        Matrix4 result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result.m[i][j] = 0.0f;
                for (int k = 0; k < 4; ++k) {
                    result.m[i][j] += m[i][k] * other.m[k][j];
                }
            }
        }
        return result;
    }

    Vector3 operator*(const Vector3& vec) const {
        float x = m[0][0] * vec.x + m[0][1] * vec.y + m[0][2] * vec.z + m[0][3];
        float y = m[1][0] * vec.x + m[1][1] * vec.y + m[1][2] * vec.z + m[1][3];
        float z = m[2][0] * vec.x + m[2][1] * vec.y + m[2][2] * vec.z + m[2][3];
        return Vector3(x, y, z);
    }

    static Matrix4 createTranslation(const Vector3& translation) {
        Matrix4 result;
        result.m[0][3] = translation.x;
        result.m[1][3] = translation.y;
        result.m[2][3] = translation.z;
        return result;
    }

    static Matrix4 createScaling(const Vector3& scale) {
        Matrix4 result;
        result.m[0][0] = scale.x;
        result.m[1][1] = scale.y;
        result.m[2][2] = scale.z;
        return result;
    }

    static Matrix4 createRotationZ(float angle) {
        Matrix4 result;
        double piVal = pi(10);
        float radians = angle * piVal / 180.0f;
        result.m[0][0] = std::cos(radians);
        result.m[0][1] = -std::sin(radians);
        result.m[1][0] = std::sin(radians);
        result.m[1][1] = std::cos(radians);
        return result;
    }

    // Create a view matrix from camera position and direction
    static Matrix4 createView(const Vector3& cameraPos) {
        Matrix4 translation = createTranslation(Vector3(-cameraPos.x, -cameraPos.y, -cameraPos.z));
        return translation; // Assuming camera faces down -z by default; for more flexibility, add rotation
    }

    // Create a perspective projection matrix
    static Matrix4 createPerspective(float fov, float aspectRatio, float nearPlane, float farPlane) {
        Matrix4 result;
        double piVal = pi(10);
        float tanHalfFov = std::tan(fov * 0.5 * piVal / 180.0f);
        result.m[0][0] = 1.0f / (aspectRatio * tanHalfFov);
        result.m[1][1] = 1.0f / tanHalfFov;
        result.m[2][2] = -(farPlane + nearPlane) / (farPlane - nearPlane);
        result.m[2][3] = -(2.0f * farPlane * nearPlane) / (farPlane - nearPlane);
        result.m[3][2] = -1.0f;
        return result;
    }

    static Matrix4 createFromQuaternion(const Quaternion& q) {
        Matrix4 result;
        float xx = q.x * q.x;
        float yy = q.y * q.y;
        float zz = q.z * q.z;
        float xy = q.x * q.y;
        float xz = q.x * q.z;
        float yz = q.y * q.z;
        float wx = q.w * q.x;
        float wy = q.w * q.y;
        float wz = q.w * q.z;

        result.m[0][0] = 1.0f - 2.0f * (yy + zz);
        result.m[0][1] = 2.0f * (xy - wz);
        result.m[0][2] = 2.0f * (xz + wy);
        result.m[0][3] = 0.0f;

        result.m[1][0] = 2.0f * (xy + wz);
        result.m[1][1] = 1.0f - 2.0f * (xx + zz);
        result.m[1][2] = 2.0f * (yz - wx);
        result.m[1][3] = 0.0f;

        result.m[2][0] = 2.0f * (xz - wy);
        result.m[2][1] = 2.0f * (yz + wx);
        result.m[2][2] = 1.0f - 2.0f * (xx + yy);
        result.m[2][3] = 0.0f;

        result.m[3][0] = 0.0f;
        result.m[3][1] = 0.0f;
        result.m[3][2] = 0.0f;
        result.m[3][3] = 1.0f;

        return result;
    }
};

#endif
