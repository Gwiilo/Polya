#ifndef VECTOR2_H
#define VECTOR2_H

#include <cmath>
#include "../funcs/MathFunctions.h"

class Vector2 {
public:
    float x, y;

    Vector2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

    // Magnitude
    float magnitude() const {
        return std::sqrt(x * x + y * y);
    }

    // Normalized
    Vector2 normalized() const {
        float mag = magnitude();
        return mag > 0 ? Vector2(x / mag, y / mag) : Vector2(0.0f, 0.0f);
    }

    // Subtraction operator
    Vector2 operator-(const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    }

    // Addition operator
    Vector2 operator+(const Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    }

    // Multiplication by scalar operator
    Vector2 operator*(float scalar) const {
        return Vector2(x * scalar, y * scalar);
    }

    // Division by scalar operator
    Vector2 operator/(float scalar) const {
        return Vector2(x / scalar, y / scalar);
    }

    Vector2 projectPoint(const Vector3& point, float fov, float aspectRatio, float nearPlane, float farPlane) {
    constexpr double PI = 3.141592653589793; // Define π as a constant
    float scale = 1 / std::tan(fov * 0.5 * PI / 180.0); // Convert FOV from degrees to radians and apply scaling
    float x = point.x * scale / point.z; // Divide by z for perspective
    float y = point.y * scale / (point.z * aspectRatio); // Adjust for aspect ratio
    return Vector2(x, y);
}
};


#endif
