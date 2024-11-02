#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <vulkan/vulkan.h>
#include <../classes/Camera.h>
#include <../globals.h>

struct VkContext {
    VkInstance instance;
};

bool vkInit(VkContext* vkcontext){
    //VkInstance instance;

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Polya";
    appInfo.pEngineName = "Polygin";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    VkResult result = vkCreateInstance(&createInfo, 0, &vkcontext->instance);

    /*if (vkCreateInstance(&createInfo, nullptr, &vkcontext) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create instance!");
    }*/
    return true;
}