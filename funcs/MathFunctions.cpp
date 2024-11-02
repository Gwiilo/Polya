#include "MathFunctions.h"
#include <cstddef> // For size_t
#include <vector> // For std::vector

#include "../classes/Vector3.h"
#include "../classes/Quaternion.h"

#include <iostream>
#include <cmath>

// Lerp function - linear interpoliation - start position, end position, amount (0 to 1)

float lerp(float start, float end, float t) {
    return start + t * (end - start);
}

Vector3 lerp(const Vector3& start, const Vector3& end, float t) {
    return start + (end - start) * t;
}

Vector2 lerp(const Vector2& start, const Vector2& end, float t) {
    return start + (end - start) * t;
}

Quaternion lerp(const Quaternion& start, const Quaternion& end, float t) {
    return start + (end - start) * t;
}

/* // Old midpoint function that only works with 2 positions (lame)

float midpoint(float pos1, float pos2) {
    return (pos1 + pos2) / 2; //pos1 + 0.5 * (pos2 - pos1);
}

Vector3 midpoint(const Vector3& pos1, const Vector3& pos2) {
    return (pos1 + pos2) / 2;
}

Vector2 midpoint(const Vector2& pos1, const Vector2& pos2) {
    return (pos1 + pos2) / 2;
}

*/

// Midpoint function - gets the center of an array of positions
/*

Example usage:

    std::vector<Vector2> positions = {
        Vector2(0.0f, 0.0f),
        Vector2(10.0f, 10.0f),
        Vector2(20.0f, 20.0f),
        Vector2(30.0f, 30.0f)
    };

    Vector2 result2midpoint = midpoint(positions);

*/

Vector3 midpoint(const std::vector<Vector3>& positions) {
    if (positions.empty()) return Vector3(0.0f, 0.0f, 0.0f);

    Vector3 totalPositions = positions[0];
    for (size_t i = 1; i < positions.size(); ++i) {
        totalPositions = totalPositions + positions[i];
    }

    return totalPositions / static_cast<float>(positions.size());
}

Vector2 midpoint(const std::vector<Vector2>& positions) {
    if (positions.empty()) return Vector2(0.0f, 0.0f);

    Vector2 totalPositions = positions[0];
    for (size_t i = 1; i < positions.size(); ++i) {
        totalPositions = totalPositions + positions[i];
    }

    return totalPositions / static_cast<float>(positions.size());
}

/*
double pi(const int repetitions) { // Leibniz formula, quite slow
    double pi = 0.0;
    int denominator = 1;
    int sign = 1;
    for (int i = 0; i < repetitions; i++) {
        pi += sign * 4.0 / denominator;
        denominator += 2;
        sign = -sign;
    }
    return pi;
}
*/

double pi(const int repetitions) { // Gauss-Legendre method - no need to use more than 10 repetitions
    double a = 1.0;
    double b = 1.0 / std::sqrt(2.0);
    double t = 0.25;
    double p = 1.0;

    for (int i = 0; i < repetitions; ++i) {
        double next_a = (a + b) / 2.0;
        b = std::sqrt(a * b);
        t -= p * (a - next_a) * (a - next_a);
        a = next_a;
        p *= 2;
    }

    return (a + b) * (a + b) / (4 * t);
}

Vector3 getPointAlongRay(const Vector3& origin, const Vector3& direction, float t) {
    return origin + direction.normalized() * t;
}

Vector3 rotateRayDirection(const Vector3& direction, const Quaternion& rotation) {
    return rotation.rotate(direction).normalized();
}