#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <vulkan/vulkan.h>
#include <../classes/Camera.h>
#include <../globals.h>

void vk_init() {
    class VulkanApp {
public:
    void run() {
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    VkInstance instance;

    void initVulkan() {
        createInstance();
    }

    void createInstance() {
        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Polya";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Polygin";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create instance!");
        }
    }

    void mainLoop() {
        /*while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }*/
    }

    void cleanup() {
        vkDestroyInstance(instance, nullptr);
    }
};
}