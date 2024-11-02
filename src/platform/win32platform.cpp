#include <windows.h>
#include <../globals.h>
#include <renderer/vkRenderer.cpp>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <vulkan/vulkan.h>
#include <../classes/Camera.h>
#include <../globals.h>

static bool running = true;
LRESULT CALLBACK platformWindowCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;

    switch (message) {
        case WM_CLOSE: {
            DestroyWindow(window);
            running = false;
        } break;
        case WM_DESTROY: {
            PostQuitMessage(0);
        } break;
        default: {
            result = DefWindowProc(window, message, wParam, lParam);
        } break;
    }

    return result;
}

bool platformCreateWindow(HWND& window) {
    HINSTANCE instance = GetModuleHandleA(0);

    WNDCLASSW wc = {};
    wc.lpfnWndProc = platformWindowCallback;
    wc.hInstance = instance;
    wc.lpszClassName = L"Polygin";
    //wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);

    if (!RegisterClassW(&wc)) {
        MessageBoxW(window, L"Failed to register window class!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return false;
    }

    window = CreateWindowExW(
        WS_EX_APPWINDOW,
        L"Polygin", // Window class name
        L"Polya",   // Window title
        WS_OVERLAPPEDWINDOW, // Window style
        100, 100, screenWidth, screenHeight,
        nullptr, nullptr, instance, nullptr
    );

    if(window) {
        ShowWindow(window, SW_SHOW);
        return true;
    } else {
        MessageBoxW(window, L"Failed to create window!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return false;
    }
}

void platformUpdateWindow(HWND window) {
    MSG message;
    while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}

int main() {
    VkContext vkcontext = {};

    HWND window = 0;
    if (!platformCreateWindow(window)) {
        return -1;
    }

    if(!vkInit(&vkcontext)) {
        return -1;
    }

    while (running) {
        platformUpdateWindow(window);
        // Perform other per-frame updates here if needed
    }

    //return EXIT_SUCCESS;
    return 0;
}