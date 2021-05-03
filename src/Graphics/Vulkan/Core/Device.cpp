//
// Created by nineball on 4/16/21.
//

#include "Device.h"
#include <iostream>
#include <vector>


std::vector<const char *> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
};
bool enableValidationLayers = false;

namespace Device {

    VkPhysicalDevice pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface) {
        VkPhysicalDevice physicalDevice;

        ///Get all devices
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
        if (deviceCount == 0) {
            throw std::runtime_error("Failed to find GPUs with vulkan support. LMAO, loser.");
        } else {
            std::cout << deviceCount << " Device(s) Found!\n";
        }
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        ///Check if device can be used
        for (const auto &device : devices) {
            if (isDeviceSuitable(device, surface)) {
                physicalDevice = device;
                break;
            }
        }

        if (physicalDevice == VK_NULL_HANDLE) {
            throw std::runtime_error("Failed to find a suitable GPU!");
        }
        else {
            return physicalDevice;
        }
    }


    bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) {

        QueueFamilyIndices indices = findQueueFamilies(device, surface);

        ///Can it even have video out, etc...
        bool extensionsSupported = checkDeviceExtensionSupport(device);

        bool swapChainAdequate = false;
        if (extensionsSupported) {
            SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device, surface);
            swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        ///Does it have a value?
        return  indices.graphicsFamily.has_value() && extensionsSupported && swapChainAdequate;
    }

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
        SwapChainSupportDetails details;

        ///Thingy that gets surface capabilities
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);


        ///Get supported surface formats
        uint32_t formatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
        if (formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
        }

        ///Get supported resent formats
        uint32_t presentModeCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount,
                                                      details.presentModes.data());
        }

        return details;
    }


    DeviceQueues createLogicalDevice(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
        QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);
        VkDevice device;

        //Filling in vulkan info to create the Present and Graphics queues
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies ={indices.graphicsFamily.value(), indices.presentFamily.value()};

        for (uint32_t QueueFamily: uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{};

            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
            //The most interesting option here, I wonder if multiple queues have a perf/latency impact.
            queueCreateInfo.queueCount = 1;

            //0.0f-1.0f range for queue priority
            float queuePriority = 1.0f;
            queueCreateInfo.pQueuePriorities = &queuePriority;

            queueCreateInfos.push_back(queueCreateInfo);
        };

        VkPhysicalDeviceFeatures deviceFeatures{};
        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        //Reads da vector for the queues to create
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();

        //Extra jazz im assuming, DLSS and the like.
        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());;
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device!");
        }



        DeviceQueues deviceQueues{};
        deviceQueues.physicalDevice = physicalDevice;
        deviceQueues.device = device;
        vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &deviceQueues.GraphicsQueue);
        vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &deviceQueues.PresentQueue);

        return deviceQueues;
    }


    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface){

        QueueFamilyIndices indices;

        ///Get device queue properties
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
        ///Find a queue family that supports VK_QUEUE_GRAPHICS_BIT.
        int i = 0;
        for (const auto &queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
            }
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.presentFamily = i;
            }
            if (indices.isComplete()) {
                break;
            }
            i++;
        }
        return indices;
    }


    bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
        //Get all extensions
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        //Get length of extension list.
        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

        //If the extension does exist it will erase from the required extensions;
        for (const auto &extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        //Return true if empty/all extensions supported.
        return requiredExtensions.empty();
    }

    void destroy(VkDevice device){
        vkDestroyDevice(device, nullptr);
        std::cout << "Cleaned up devices :)\n";
    }
}
