//
// Created by nineball on 1/13/21.
//

#include "Swapchain.h"
#include "iostream"

Swapchain::Swapchain() {
    createSwapChain();
    createImageViews();
}

Swapchain::~Swapchain() {

    for (auto imageview : vkGlobalPool::Get().getSwapChainImageViews()){
        vkDestroyImageView(vkGlobalPool::Get().getVkDevice(), imageview, nullptr);
    }
};

void Swapchain::createImageViews() {
    //Want them to be the same length. 1 view, 1 image.
    vkGlobalPool::Get().getSwapChainImageViews().resize(vkGlobalPool::Get().getSwapChainImages().size());

    for(size_t i = 0; i < vkGlobalPool::Get().getSwapChainImages().size(); i++){
        VkImageViewCreateInfo  createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = vkGlobalPool::Get().getSwapChainImages()[i];
        ////TODO 3D???!?!?
        createInfo.viewType =  VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = vkGlobalPool::Get().getSwapChainImageFormat();

        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if(vkCreateImageView(vkGlobalPool::Get().getVkDevice(), &createInfo, nullptr, &vkGlobalPool::Get().getSwapChainImageViews()[i]) != VK_SUCCESS){
            throw std::runtime_error("Failed to create image views!");
        }
    }

}

void Swapchain::createSwapChain() {
    VkSwapchainKHR swapChain;

    vkGlobalPool::SwapChainSupportDetails swapChainSupport = vkGlobalPool::Get().getSwapChainSupportDetails();

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

    ///How many images are in the swapchain
    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = vkGlobalPool::Get().getVkSurfaceKhr();
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    ///always one unless stereoscopic 3D
    createInfo.imageArrayLayers = 1;
    ///     For rendering directly to image we do this, for post processing it will be changed to
    ///VK_IMAGE_USAGE_TRANSFER_DST_BIT as that allows us to do the render in memory and copy it to the
    ///swapchain instead of directly drawing to it.
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    ///Queue family sharing thing.
    vkGlobalPool::QueueFamilyIndices indices = vkGlobalPool::Get().getQueueFamilyIndices();
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};
    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional
    }

    ///No transform applied, ignore alpha channel
    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    createInfo.presentMode = presentMode;
    ///We dont care about obscured pixels
    createInfo.clipped = VK_TRUE;

    ////A new SwapChain needs to be created for each window resize.
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(vkGlobalPool::Get().getVkDevice(), &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create swap chain!");
    }
    else {
        std::cout << "SwapChain probably successfully created.\n";
    }

    vkGetSwapchainImagesKHR(vkGlobalPool::Get().getVkDevice(), swapChain, &imageCount, nullptr);
    vkGlobalPool::Get().getSwapChainImages().resize(imageCount);
    vkGetSwapchainImagesKHR(vkGlobalPool::Get().getVkDevice(), swapChain, &imageCount,vkGlobalPool::Get().getSwapChainImages().data());


    vkGlobalPool::Get().setSwapChain(swapChain);
    vkGlobalPool::Get().setSwapChainImageFormat(surfaceFormat.format);
    vkGlobalPool::Get().setSwapChainExtent(extent);
}


VkPresentModeKHR Swapchain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes) {
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Swapchain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities) {
    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    } else {
        int width, height;

        glfwGetFramebufferSize(vkGlobalPool::Get().getWindow(), &width, &height);

        VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
        };

        actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
}

VkSurfaceFormatKHR Swapchain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) {
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}