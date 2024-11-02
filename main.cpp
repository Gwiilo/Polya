#include <iostream>
#include <stdexcept>

// ...existing includes...

int main() {
    try {
        // ...existing code...
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown exception occurred" << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
