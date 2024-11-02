#include "vkRenderer.h"
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <vulkan/vulkan.h>
#include <windows.h>
#include <vulkan/vulkan_win32.h>
#include <../classes/Camera.h>
#include <../globals.h>
#include "../classes/Cube.h"
#include "../classes/Vector3.h"

bool vkInit(VkContext* vkcontext) {
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

    // Enable required extensions
    const std::vector<const char*> extensions = {
        VK_KHR_SURFACE_EXTENSION_NAME,
#ifdef _WIN32
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#endif
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    // Enable validation layers (optional)
    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();

    VkResult result = vkCreateInstance(&createInfo, nullptr, &vkcontext->instance);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan instance!");
    }

    // Select a physical device
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(vkcontext->instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        throw std::runtime_error("Failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(vkcontext->instance, &deviceCount, devices.data());
    vkcontext->physicalDevice = devices[0];

    // Get surface formats
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(vkcontext->physicalDevice, vkcontext->surface, &formatCount, nullptr);
    if (formatCount == 0) {
        throw std::runtime_error("Failed to find surface formats!");
    }

    std::vector<VkSurfaceFormatKHR> formats(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(vkcontext->physicalDevice, vkcontext->surface, &formatCount, formats.data());

    // Choose the first available format
    vkcontext->surfaceFormat = formats[0];

    // Create logical device and get graphics queue
    float queuePriority = 1.0f;
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = 0; // Assume graphics queue family index is 0
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;

    if (vkCreateDevice(vkcontext->physicalDevice, &deviceCreateInfo, nullptr, &vkcontext->device) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create logical device!");
    }

    vkGetDeviceQueue(vkcontext->device, 0, 0, &vkcontext->graphicsQueue);

    // Create command pool
    VkCommandPoolCreateInfo poolCreateInfo = {};
    poolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolCreateInfo.queueFamilyIndex = 0; // Assume graphics queue family index is 0

    if (vkCreateCommandPool(vkcontext->device, &poolCreateInfo, nullptr, &vkcontext->commandPool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create command pool!");
    }

    // Allocate command buffer
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = vkcontext->commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(vkcontext->device, &allocInfo, &vkcontext->commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate command buffer!");
    }

    // Create swapchain
    VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
    swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCreateInfo.surface = vkcontext->surface;
    swapchainCreateInfo.minImageCount = 2; // Double buffering
    swapchainCreateInfo.imageFormat = vkcontext->surfaceFormat.format; // Assuming vkcontext->surfaceFormat is of type VkSurfaceFormatKHR
    swapchainCreateInfo.imageColorSpace = vkcontext->surfaceFormat.colorSpace;
    swapchainCreateInfo.imageExtent = vkcontext->extent;
    swapchainCreateInfo.imageArrayLayers = 1;
    swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchainCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainCreateInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
    swapchainCreateInfo.clipped = VK_TRUE;

    if (vkCreateSwapchainKHR(vkcontext->device, &swapchainCreateInfo, nullptr, &vkcontext->swapchain) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create swapchain!");
    }

    // Get swapchain images
    uint32_t imageCount;
    vkGetSwapchainImagesKHR(vkcontext->device, vkcontext->swapchain, &imageCount, nullptr);
    vkcontext->swapchainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(vkcontext->device, vkcontext->swapchain, &imageCount, vkcontext->swapchainImages.data());

    return true;
}

void renderCube(VkContext* vkcontext, const Cube& cube, const Vector3& position) {
    // Implement Vulkan rendering logic for the cube at the specified position
    // This is a placeholder function and needs to be filled with actual Vulkan rendering code

    // Begin command buffer recording
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

    if (vkBeginCommandBuffer(vkcontext->commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("Failed to begin recording command buffer!");
    }

    // Begin render pass
    VkRenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = vkcontext->renderPass;
    renderPassInfo.framebuffer = vkcontext->framebuffer;
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = vkcontext->extent;

    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(vkcontext->commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    // Bind pipeline
    vkCmdBindPipeline(vkcontext->commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkcontext->pipeline);

    // Bind vertex buffer
    VkBuffer vertexBuffers[] = {vkcontext->vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(vkcontext->commandBuffer, 0, 1, vertexBuffers, offsets);

    // Bind index buffer
    vkCmdBindIndexBuffer(vkcontext->commandBuffer, vkcontext->indexBuffer, 0, VK_INDEX_TYPE_UINT32);

    // Set model transformation matrix
    vkCmdBindDescriptorSets(vkcontext->commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkcontext->pipelineLayout, 0, 1, &vkcontext->descriptorSet, 0, nullptr);

    // Draw indexed
    vkCmdDrawIndexed(vkcontext->commandBuffer, static_cast<uint32_t>(cube.indices.size()), 1, 0, 0, 0);

    // End render pass
    vkCmdEndRenderPass(vkcontext->commandBuffer);

    // End command buffer recording
    if (vkEndCommandBuffer(vkcontext->commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to record command buffer!");
    }

    // Submit command buffer to the graphics queue
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &vkcontext->commandBuffer;

    if (vkQueueSubmit(vkcontext->graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
        throw std::runtime_error("Failed to submit draw command buffer!");
    }

    vkQueueWaitIdle(vkcontext->graphicsQueue);
}