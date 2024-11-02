#ifndef RAY_H
#define RAY_H

#include <cmath>
#include "../classes/Vector3.h"
#include "../classes/Quaternion.h"

class Ray {
public:
    Vector3 origin;
    Vector3 direction;

    // Constructor
    Ray(const Vector3& origin = Vector3(0.0f, 0.0f, 0.0f), const Vector3& direction = Vector3(0.0f, 0.0f, 1.0f))
        : origin(origin), direction(direction.normalized()) {}

    void setDirection(const Vector3& newDirection) {
        direction = newDirection.normalized();
    }

    Vector3 getPoint(float t) const {
        return origin + direction * t;
    }

    void rotate(const Quaternion& rotation) {
        direction = rotation.rotate(direction).normalized();
    }
};

#endif
