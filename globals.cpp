#include "globals.h"

// Depth buffer
float depthBuffer[screenWidth * screenHeight];

// Global camera instance
Camera globalCamera(
    Vector3(0.0f, 0.0f, 0.0f), 
    Quaternion(0.0f, 0.0f, 0.0f, 1.0f), 
    70.0f, 
    static_cast<float>(screenWidth) / screenHeight, 
    0.1f, 
    100.0f
);
