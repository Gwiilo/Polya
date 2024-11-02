#ifndef QUATERNION_H
#define QUATERNION_H

#include <cmath>
#include <iostream>
#include "../classes/Vector3.h"

class Quaternion {
public:
    float x, y, z, w;

    Quaternion(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f) : x(x), y(y), z(z), w(w) {}

    // Subtraction operator
    Quaternion operator-(const Quaternion& other) const {
        return Quaternion(x - other.x, y - other.y, z - other.z, w - other.w);
    }

    // Addition operator
    Quaternion operator+(const Quaternion& other) const {
        return Quaternion(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    // Multiplication by scalar operator
    Quaternion operator*(float scalar) const {
        return Quaternion(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    // Division by scalar operator
    Quaternion operator/(float scalar) const {
        return Quaternion(x / scalar, y / scalar, z / scalar, w / scalar);
    }

    // Dot product
    float dot(const Quaternion& other) const {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    // Normalize the quaternion
    Quaternion normalize() const {
        try {
            float magnitude = std::sqrt(x * x + y * y + z * z + w * w);
            if (magnitude == 0) {
                std::cerr << "Warning: magnitude is zero in Quaternion::normalize" << std::endl;
            }
            return *this / magnitude;
        } catch (const std::exception& e) {
            std::cerr << "Exception in Quaternion::normalize: " << e.what() << std::endl;
            throw;
        }
    }

    // Quaternion-Quaternion multiplication (for rotation chaining)
    Quaternion operator*(const Quaternion& other) const {
        return Quaternion(
            w * other.x + x * other.w + y * other.z - z * other.y,
            w * other.y - x * other.z + y * other.w + z * other.x,
            w * other.z + x * other.y - y * other.x + z * other.w,
            w * other.w - x * other.x - y * other.y - z * other.z
        );
    }

    // Rotate a Vector3 by this quaternion
    Vector3 rotate(const Vector3& vec) const {
        Quaternion vecQuat(vec.x, vec.y, vec.z, 0.0f);
        Quaternion result = (*this * vecQuat) * conjugate();
        return Vector3(result.x, result.y, result.z);
    }

    static Quaternion fromAxisAngle(const Vector3& axis, float angle) {
        float halfAngle = angle / 2.0f;
        float sinHalfAngle = std::sin(halfAngle);
        return Quaternion(
            axis.x * sinHalfAngle,
            axis.y * sinHalfAngle,
            axis.z * sinHalfAngle,
            std::cos(halfAngle)
        );
    }

private:
    // Conjugate of the quaternion (needed for vector rotation)
    Quaternion conjugate() const {
        return Quaternion(-x, -y, -z, w);
    }
};

#endif
