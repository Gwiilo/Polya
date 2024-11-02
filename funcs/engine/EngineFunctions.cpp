#include <iostream>
#include <vector>
#include <fstream>
#include <cstdint>
#include <cmath>
#include <limits>
#include <algorithm>

#include "../MathFunctions.h"
#include "../../classes/Vector2.h"
#include "../../classes/Vector3.h"
#include "../../classes/Color.h"
#include "../../classes/Matrix4.h"
#include "../../classes/Camera.h"
#include "../../globals.h"
#include "../../globals.cpp"

std::vector<std::vector<Color>> frameBuffer(screenHeight, std::vector<Color>(screenWidth));

inline void clearFrameBuffer(const Color& color) {
//void clearFrameBuffer(const Color& color = Color(0, 0, 0)) {
    for (int y = 0; y < screenHeight; ++y) {
        for (int x = 0; x < screenWidth; ++x) {
            frameBuffer[y][x] = color;
        }
    }
}

void setPixel(int x, int y, const Color& color) {
    if (x >= 0 && x < screenWidth && y >= 0 && y < screenHeight) {
        frameBuffer[y][x] = color;
    }
}

void writeFrameBufferToFile(const std::string& filename) {
    std::ofstream outFile(filename, std::ios::out | std::ios::binary);
    outFile << "P6\n" << screenWidth << " " << screenHeight << "\n255\n";
    for (int y = 0; y < screenHeight; ++y) {
        for (int x = 0; x < screenWidth; ++x) {
            Color& color = frameBuffer[y][x];
            outFile << static_cast<char>(color.r)
                    << static_cast<char>(color.g)
                    << static_cast<char>(color.b);
        }
    }
    outFile.close();
}

void clearDepthBuffer() {
    std::fill_n(depthBuffer, screenWidth * screenHeight, std::numeric_limits<float>::infinity());
}


/* // Before I realized that I have already implemented Vector2s
void drawLine(int x0, int y0, int x1, int y1, const Color& color) {
    int dx = std::abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -std::abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (true) {
        setPixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void drawGrid(int x0, int y0, int x1, int y1, int gridSpacing, const Color& color) {
    for (int x = x0; x <= x1; x += gridSpacing) {
        drawLine(x, y0, x, y1, color);
    }

    for (int y = y0; y <= y1; y += gridSpacing) {
        drawLine(x0, y, x1, y, color);
    }
}
*/

void drawLine(const Vector2& from, const Vector2& to, const Color& color) {
    int x0 = static_cast<int>(from.x);
    int y0 = static_cast<int>(from.y);
    int x1 = static_cast<int>(to.x);
    int y1 = static_cast<int>(to.y);

    int dx = std::abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -std::abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (true) {
        setPixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void drawGrid(const Vector2& from, const Vector2& to, int gridSpacing, const Color& color) {
    int x0 = static_cast<int>(from.x);
    int y0 = static_cast<int>(from.y);
    int x1 = static_cast<int>(to.x);
    int y1 = static_cast<int>(to.y);

    for (int x = x0; x <= x1; x += gridSpacing) {
        drawLine(Vector2(x, y0), Vector2(x, y1), color);
    }

    for (int y = y0; y <= y1; y += gridSpacing) {
        drawLine(Vector2(x0, y), Vector2(x1, y), color);
    }
}

void drawRect(const Vector2& from, const Vector2& to, const Color& color) {
    int x0 = static_cast<int>(std::min(from.x, to.x));
    int y0 = static_cast<int>(std::min(from.y, to.y));
    int x1 = static_cast<int>(std::max(from.x, to.x));
    int y1 = static_cast<int>(std::max(from.y, to.y));

    for (int y = y0; y <= y1; ++y) {
        for (int x = x0; x <= x1; ++x) {
            setPixel(x, y, color);
        }
    }
}

Vector2 projectPoint(const Vector3& point, float fov, float aspectRatio, float nearPlane, float farPlane) {
    float scale = 1 / std::tan(fov * 0.5 * 3.14159265359 / 180.0); // Convert FOV from degrees to radians and apply scaling
    float x = point.x * scale / point.z; // Divide by z for perspective
    float y = point.y * scale / (point.z * aspectRatio); // Adjust for aspect ratio
    return Vector2(x, y);
}

Vector3 barycentric(const Vector2& p, const Vector2& a, const Vector2& b, const Vector2& c) {
    Vector3 s0 = {c.x - a.x, b.x - a.x, a.x - p.x};
    Vector3 s1 = {c.y - a.y, b.y - a.y, a.y - p.y};
    Vector3 u = s0.cross(s1);

    if (std::abs(u.z) < 1) return Vector3(-1, 1, 1); // Avoid division by zero

    return Vector3(1.0f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
}

void drawTriangle(const Vector2& p0, const Vector2& p1, const Vector2& p2, const Color& color, float z0, float z1, float z2) {
    // Determine the bounding box of the triangle
    int minX = std::max(0, (int)std::floor(std::min({p0.x, p1.x, p2.x})));
    int minY = std::max(0, (int)std::floor(std::min({p0.y, p1.y, p2.y})));
    int maxX = std::min(screenWidth - 1, (int)std::ceil(std::max({p0.x, p1.x, p2.x})));
    int maxY = std::min(screenHeight - 1, (int)std::ceil(std::max({p0.y, p1.y, p2.y})));

    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {
            Vector2 p(x + 0.5f, y + 0.5f); // Pixel center

            Vector3 bary = barycentric(p, p0, p1, p2);
            if (bary.x < 0 || bary.y < 0 || bary.z < 0) continue; // Outside the triangle

            float z = bary.x * z0 + bary.y * z1 + bary.z * z2;

            int index = y * screenWidth + x; // 1D index in depth buffer
            if (z < depthBuffer[index]) { // Depth test
                depthBuffer[index] = z;   // Update depth buffer
                setPixel(x, y, color);    // Draw pixel
            }
        }
    }
}


void drawTriangle3D(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Color& color,
    float fov, float aspectRatio, float nearPlane, float farPlane) {

    // Project the 3D vertices to 2D screen coordinates
    Vector2 p0 = projectPoint(v0, fov, aspectRatio, nearPlane, farPlane);
    Vector2 p1 = projectPoint(v1, fov, aspectRatio, nearPlane, farPlane);
    Vector2 p2 = projectPoint(v2, fov, aspectRatio, nearPlane, farPlane);

    // Convert normalized device coordinates to screen coordinates
    p0.x = (p0.x + 1) * screenWidth / 2;
    p0.y = (1 - p0.y) * screenHeight / 2;
    p1.x = (p1.x + 1) * screenWidth / 2;
    p1.y = (1 - p1.y) * screenHeight / 2;
    p2.x = (p2.x + 1) * screenWidth / 2;
    p2.y = (1 - p2.y) * screenHeight / 2;

    // Pass the projected 2D points and the z-values of the original 3D points to drawTriangle
    drawTriangle(p0, p1, p2, color, v0.z, v1.z, v2.z);
}

std::vector<Vector3> generateSpherePoints(const Vector3& position, float radius, int segments) {
    std::vector<Vector3> points;
    for (int i = 0; i <= segments; ++i) {
        float theta = i * 3.14159265359 / segments; // latitude angle
        for (int j = 0; j <= segments * 2; ++j) {
            float phi = j * 2 * 3.14159265359 / (segments * 2); // longitude angle
            float x = position.x + radius * std::sin(theta) * std::cos(phi);
            float y = position.y + radius * std::cos(theta);
            float z = position.z + radius * std::sin(theta) * std::sin(phi);
            points.push_back(Vector3(x, y, z));
        }
    }
    return points;
}

void drawSphere(const Vector3& position, float size) { 
    int segments = 10;
    auto points = generateSpherePoints(position, size, segments);

    std::vector<Vector2> projectedPoints;
    std::vector<float> zValues; // Store the z-values of each point for depth testing

    for (const auto& point : points) {
        Vector2 projectedPoint = projectPoint(
            point, 
            globalCamera.getFOV(), 
            globalCamera.getAspectRatio(), 
            globalCamera.getNearPlane(), 
            globalCamera.getFarPlane()
        );
        
        projectedPoints.push_back(projectedPoint);
        zValues.push_back(point.z); // Store the original z-value for depth testing
    }

    for (int i = 0; i < segments; ++i) {
        for (int j = 0; j < segments * 2; ++j) {
            int p0 = i * (segments * 2 + 1) + j;
            int p1 = p0 + 1;
            int p2 = p0 + (segments * 2 + 1);
            int p3 = p2 + 1;

            drawTriangle(projectedPoints[p0], projectedPoints[p2], projectedPoints[p1], Color(255, 255, 255), zValues[p0], zValues[p2], zValues[p1]);
            drawTriangle(projectedPoints[p1], projectedPoints[p2], projectedPoints[p3], Color(255, 255, 255), zValues[p1], zValues[p2], zValues[p3]);
        }
    }
}

void drawIcosahedron(const Vector3& position, float size) {
    float t = (1.0 + std::sqrt(5.0)) / 2.0;

    std::vector<Vector3> vertices = {
        Vector3(-1,  t,  0), Vector3( 1,  t,  0), Vector3(-1, -t,  0), Vector3( 1, -t,  0),
        Vector3( 0, -1,  t), Vector3( 0,  1,  t), Vector3( 0, -1, -t), Vector3( 0,  1, -t),
        Vector3( t,  0, -1), Vector3( t,  0,  1), Vector3(-t,  0, -1), Vector3(-t,  0,  1)
    };

    for (auto& vertex : vertices) {
        vertex = position + vertex.normalized() * size;
    }

    int faces[20][3] = {
        {0, 11, 5}, {0, 5, 1}, {0, 1, 7}, {0, 7, 10}, {0, 10, 11},
        {1, 5, 9}, {5, 11, 4}, {11, 10, 2}, {10, 7, 6}, {7, 1, 8},
        {3, 9, 4}, {3, 4, 2}, {3, 2, 6}, {3, 6, 8}, {3, 8, 9},
        {4, 9, 5}, {2, 4, 11}, {6, 2, 10}, {8, 6, 7}, {9, 8, 1}
    };

    std::vector<Vector2> projectedVertices;
    std::vector<float> zValues; // Store the z-values of each point for depth testing

    for (const auto& vertex : vertices) {
        projectedVertices.push_back(projectPoint(
            vertex, 
            globalCamera.getFOV(), 
            globalCamera.getAspectRatio(), 
            globalCamera.getNearPlane(), 
            globalCamera.getFarPlane()
        ));
        zValues.push_back(vertex.z); // Store the z-value
    }

    for (int i = 0; i < 20; ++i) {
        drawTriangle(
            projectedVertices[faces[i][0]], 
            projectedVertices[faces[i][1]], 
            projectedVertices[faces[i][2]], 
            Color(255, 255, 255),
            zValues[faces[i][0]], zValues[faces[i][1]], zValues[faces[i][2]]
        );
    }
}

void drawShape(const std::vector<Vector3>& points) {
    std::vector<Vector2> projectedPoints;
    std::vector<float> zValues; // Store the z-values for depth testing

    for (const auto& point : points) {
        projectedPoints.push_back(projectPoint(
            point, 
            globalCamera.getFOV(), 
            globalCamera.getAspectRatio(), 
            globalCamera.getNearPlane(), 
            globalCamera.getFarPlane()
        ));
        zValues.push_back(point.z); // Store the z-value
    }

    for (size_t i = 0; i + 2 < projectedPoints.size(); i += 3) {
        drawTriangle(
            projectedPoints[i], 
            projectedPoints[i + 1], 
            projectedPoints[i + 2], 
            Color(255, 255, 255),
            zValues[i], zValues[i + 1], zValues[i + 2]
        );
    }
}

void drawCube(const Vector3& position, const Vector3& size) {
    std::vector<Vector3> vertices = {
        position + Vector3(-size.x, -size.y, -size.z),
        position + Vector3( size.x, -size.y, -size.z),
        position + Vector3( size.x,  size.y, -size.z),
        position + Vector3(-size.x,  size.y, -size.z),
        position + Vector3(-size.x, -size.y,  size.z),
        position + Vector3( size.x, -size.y,  size.z),
        position + Vector3( size.x,  size.y,  size.z),
        position + Vector3(-size.x,  size.y,  size.z)
    };

    int faces[12][3] = {
        {0, 1, 2}, {0, 2, 3}, // Back face
        {4, 5, 6}, {4, 6, 7}, // Front face
        {0, 1, 5}, {0, 5, 4}, // Bottom face
        {2, 3, 7}, {2, 7, 6}, // Top face
        {0, 3, 7}, {0, 7, 4}, // Left face
        {1, 2, 6}, {1, 6, 5}  // Right face
    };

    std::vector<Vector2> projectedVertices;
    std::vector<float> zValues; // Store the z-values for depth testing

    for (const auto& vertex : vertices) {
        projectedVertices.push_back(projectPoint(
            vertex, 
            globalCamera.getFOV(), 
            globalCamera.getAspectRatio(), 
            globalCamera.getNearPlane(), 
            globalCamera.getFarPlane()
        ));
        zValues.push_back(vertex.z); // Store the z-value
    }

    for (int i = 0; i < 12; ++i) {
        drawTriangle(
            projectedVertices[faces[i][0]], 
            projectedVertices[faces[i][1]], 
            projectedVertices[faces[i][2]], 
            Color(255, 255, 255),
            zValues[faces[i][0]], zValues[faces[i][1]], zValues[faces[i][2]]
        );
    }
}


