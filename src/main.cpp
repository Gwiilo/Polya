#include <iostream>
#include <vulkan/vulkan.h>
#include <../classes/Camera.h>

/* TODO

    * Copy over old data from old engine into Vulkan engine

    * Configure tasks.json and launch.json to work with Vulkan and the old engine
    
 */

int main() {

    VkApplicationInfo appInfo = {};
    appInfo.apiVersion = VK_API_VERSION_1_0;
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Polygin";
    appInfo.pApplicationName = "Polya";

    VkInstanceCreateInfo instanceInfo = {};
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pApplicationInfo = &appInfo;

    VkInstance instance;

    VkResult result = vkCreateInstance(&instanceInfo, 0, &instance);
    if (result == VK_SUCCESS) {
        std::cout << "Successfully created Vulkan instance!" << std::endl;
    }
    else {
        std::cerr << "Failed to create Vulkan instance!" << std::endl;
    }

    /*
    std::cout << "Hello, World!" << std::endl;

    // Initialize Vulkan (example code)
    VkInstance instance;
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Vulkan";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        std::cerr << "Failed to create Vulkan instance!" << std::endl;
        return -1;
    }

    // Clean up Vulkan instance
    vkDestroyInstance(instance, nullptr);
    */
    return 0;
}