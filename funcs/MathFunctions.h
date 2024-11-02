#ifndef MATH_FUNCTIONS_H
#define MATH_FUNCTIONS_H

#include <cstddef>  // For size_t
#include <vector>   // For std::vector
#include <iostream>
#include "../classes/Vector3.h"
#include "../classes/Vector2.h"
#include "../classes/Quaternion.h"

float lerp(float start, float end, float t);
Vector3 lerp(const Vector3& start, const Vector3& end, float t);
Vector2 lerp(const Vector2& start, const Vector2& end, float t);
Quaternion lerp(const Quaternion& start, const Quaternion& end, float t);

/*
float midpoint(float pos1, float pos2, float t);
Vector3 midpoint(const Vector3& pos1, const Vector3& pos2);
Vector2 midpoint(const Vector2& pos1, const Vector2& pos2);
*/

template <typename... Args>
Vector3 midpoint(const Vector3& first, const Args&... args) {
    Vector3 totalPositions = first;
    int totalPositionsNum = 1;

    auto addPositions = [&](const Vector3& pos) {
        totalPositions = totalPositions + pos;
        totalPositionsNum++;
    };

    (addPositions(args), ...);

    return totalPositions / static_cast<float>(totalPositionsNum);
}

template <typename... Args>
Vector2 midpoint(const Vector2& first, const Args&... args) {
    Vector2 totalPositions = first;
    int totalPositionsNum = 1;

    auto addPositions = [&](const Vector2& pos) {
        totalPositions = totalPositions + pos;
        totalPositionsNum++;
    };

    (addPositions(args), ...);

    return totalPositions / static_cast<float>(totalPositionsNum);
}

Vector3 midpoint(const std::vector<Vector3>& positions);
Vector2 midpoint(const std::vector<Vector2>& positions);

double pi(const int repetitions);

Vector3 getPointAlongRay(const Vector3& origin, const Vector3& direction, float t);
Vector3 rotateRayDirection(const Vector3& direction, const Quaternion& rotation);

#endif
