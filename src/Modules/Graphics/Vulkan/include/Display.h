//
// Created by nineball on 7/6/21.
//

#ifndef NINEENGINE_DISPLAY_H
#define NINEENGINE_DISPLAY_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include "Types.h"

struct displayCreateInfo {
    VkExtent2D extent = {800, 600};

    bool resizable = false;

    std::string title;
    VkInstance instance = VK_NULL_HANDLE;
    VkPhysicalDevice GPU = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;
    VkQueue queue = VK_NULL_HANDLE;
};

class Display {
public:
    Display(const displayCreateInfo& createInfo);
    ~Display();

    Display(const Display&) = delete;
    Display& operator = (const Display&) = delete;


    //Initialization methods (Necessary that all of these are done before the render methods are called)
    void createSurface(VkInstance);
    void createSwapchain(VkDevice, VkPhysicalDevice, VkQueue presentQueue);

    void createFramebuffers(VkRenderPass renderpass);
    void createSyncStructures();
    void createCommandBuffer(VkCommandPool);

    //Render methods
    VkCommandBuffer startFrame();
    void endFrame();

    bool shouldExit();

public:
    VkSurfaceKHR surface();
    uint16_t frameNumber();
    VkFormat format();
    VkExtent2D extent();

private:
    void createWindow(VkExtent2D extent, const std::string& title, bool resizable);

    //Swapchain variables
    VkSwapchainKHR mSwapchain = VK_NULL_HANDLE;
    VkFormat mFormat;
    std::vector<VkImage> mImages;
    std::vector<VkImageView> mImageViews;
    std::vector<VkFramebuffer> mFramebuffers;
    VkRenderPass mRenderpass = VK_NULL_HANDLE;

    //Device variables
    VkCommandBuffer mPrimaryCommandBuffer;
    VkQueue mGraphicsQueue;
    VkQueue mPresentQueue;

    //Sync
    VkSemaphore mPresentSemaphore = VK_NULL_HANDLE, mRenderSemaphore = VK_NULL_HANDLE;
    VkFence mRenderFence = VK_NULL_HANDLE;
    uint16_t mFrameNumber = 0;
    uint32_t mSwapchainImageIndex = 0;

    //Window variables
    VkExtent2D mExtent;
    GLFWwindow* mWindow = nullptr;
    VkSurfaceKHR mSurface = VK_NULL_HANDLE;

private:
    //Mainly destruction variables
    VkDevice mDevice = VK_NULL_HANDLE;
    VkInstance mInstance = VK_NULL_HANDLE;
};



#endif //NINEENGINE_DISPLAY_H