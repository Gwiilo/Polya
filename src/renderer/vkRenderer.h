#ifndef VKRENDERER_H
#define VKRENDERER_H

#include <vulkan/vulkan.h>
#include "../classes/Cube.h"
#include "../classes/Vector3.h"

struct VkContext {
    VkInstance instance;
    VkDevice device;
    VkPhysicalDevice physicalDevice;
    VkQueue graphicsQueue;
    VkCommandPool commandPool;
    VkCommandBuffer commandBuffer;
    VkSurfaceKHR surface;
    VkSwapchainKHR swapchain;
    std::vector<VkImage> swapchainImages;
    VkSurfaceFormatKHR surfaceFormat; // Change from VkFormat to VkSurfaceFormatKHR
    VkPipeline pipeline;
    VkPipelineLayout pipelineLayout;
    VkRenderPass renderPass;
    VkFramebuffer framebuffer;
    VkExtent2D extent;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    VkDescriptorSet descriptorSet;
};

bool vkInit(VkContext* vkcontext);
void renderCube(VkContext* vkcontext, const Cube& cube, const Vector3& position);

#endif
