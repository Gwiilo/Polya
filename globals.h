#ifndef GLOBALS_H
#define GLOBALS_H

#include "classes/Camera.h"
#include <limits>

// Screen dimensions
const int screenWidth = 800;
const int screenHeight = 600;
const int maxFramerate = 60;

extern Camera globalCamera;
extern float depthBuffer[screenWidth * screenHeight];

#endif
