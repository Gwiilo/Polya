#include <windows.h>
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

bool platformCreateWindow(HWND window) {
    HINSTANCE instance = GetModuleHandleA(0);

    WNDCLASS wc = {};
    wc.lpfnWndProc = platformWindowCallback;
    wc.hInstance = instance;
    wc.lpszClassName = "vulkanengineclass";

    if (!RegisterClass(&wc)) {
        MessageBoxW(window, L"Failed to register window class!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return false;
    }

    window = CreateWindowExW(
        WS_EX_APPWINDOW,
        L"Polygin",
        L"Polya",
        0, 100, 100, screenWidth, screenHeight, 0, 0, instance, 0
    );
    if(window) {
        ShowWindow(window, SW_SHOW);
        return true;
    } else {
        MessageBoxW(window, L"Failed to create window!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return false;
    }
    
}

int main() {
    HWND window = 0;
    if(platformCreateWindow(window)) {
        return -1;
    }

    return EXIT_SUCCESS;
}