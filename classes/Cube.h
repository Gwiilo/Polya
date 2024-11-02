#ifndef CUBE_H
#define CUBE_H

#include <vector>
#include "Vector3.h"

class Cube {
public:
    std::vector<Vector3> vertices;
    std::vector<uint32_t> indices;

    Cube(bool useQuads = false) {
        // Define vertices
        vertices = {
            { -0.5f, -0.5f, -0.5f }, { 0.5f, -0.5f, -0.5f }, { 0.5f,  0.5f, -0.5f }, { -0.5f,  0.5f, -0.5f },
            { -0.5f, -0.5f,  0.5f }, { 0.5f, -0.5f,  0.5f }, { 0.5f,  0.5f,  0.5f }, { -0.5f,  0.5f,  0.5f }
        };

        if (useQuads) {
            // Define indices for quads
            indices = {
                0, 1, 2, 3,  // Front face
                4, 5, 6, 7,  // Back face
                0, 1, 5, 4,  // Bottom face
                2, 3, 7, 6,  // Top face
                0, 3, 7, 4,  // Left face
                1, 2, 6, 5   // Right face
            };
        } else {
            // Define indices for triangles
            indices = {
                0, 1, 2, 2, 3, 0,  // Front face
                4, 5, 6, 6, 7, 4,  // Back face
                0, 1, 5, 5, 4, 0,  // Bottom face
                2, 3, 7, 7, 6, 2,  // Top face
                0, 3, 7, 7, 4, 0,  // Left face
                1, 2, 6, 6, 5, 1   // Right face
            };
        }
    }
};

#endif
