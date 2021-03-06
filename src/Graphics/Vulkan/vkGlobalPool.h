//
// Created by nineball on 1/8/21.
//

#ifndef NINEENGINE_VKGLOBALPOOL_H
#define NINEENGINE_VKGLOBALPOOL_H
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <set>
#include <vector>
#include <array>

///Singleton to help handle the sheer amount of shared variables
///Will only be accessible if a vulkan instance is created

class vkGlobalPool{


    //SINGLETON-CLASS-CREATION-START---------------------------------------------//

public:
    vkGlobalPool(const vkGlobalPool&) = delete;

    static vkGlobalPool& Get(){
        return s_Instance;
    }

private:
    const int MAX_FRAMES_IN_FLIGHT = 2;

    vkGlobalPool() : renderFinishedSemaphores(MAX_FRAMES_IN_FLIGHT), imageAvailableSemaphores(MAX_FRAMES_IN_FLIGHT),
    inFlightFences(MAX_FRAMES_IN_FLIGHT) {};

    static vkGlobalPool s_Instance;

    //SINGLETON-CLASS-CREATION-END-----------------------------------------------//




    //VARIABLES-START------------------------------------------------------------//

public:
    //STRUCTS-START------------------------------------------------------------//

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return graphicsFamily.has_value();
        }
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    struct Vertex {
        glm::vec2 pos;
        glm::vec3 color;

        static VkVertexInputBindingDescription getBindingDescription() {
            VkVertexInputBindingDescription bindingDescription{};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return bindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
            std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex, pos);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(Vertex, color);

            return attributeDescriptions;
        }
    };

    const std::vector<uint16_t> indices = {
            0, 1, 2, 2, 3, 0
    };

    const std::vector<Vertex> vertices = {
            {{-0.5f, -0.5f}, {1.0f, 0.0f, 1.0f}},
            {{0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}},
            {{0.5f, 0.5f}, {1.0f, 0.0f, 5.0f}},
            {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
    };

    //STRUCTS-END--------------------------------------------------------------//

public:
    const VkSurfaceKHR getVkSurfaceKhr() const {
        return vkSurfaceKhr;
    }

    void setVkSurfaceKhr(const VkSurfaceKHR _vkSurfaceKhr) {
        vkGlobalPool::vkSurfaceKhr = _vkSurfaceKhr;
    }

    GLFWwindow *getWindow() const {
        return window;
    }

    void setWindow(GLFWwindow *_window) {
        vkGlobalPool::window = _window;
    }

    VkDevice getVkDevice() {
        return vkDevice;
    }

    void setVkDevice(const VkDevice _vkDevice) {
        vkGlobalPool::vkDevice = _vkDevice;
    }

    const VkPhysicalDevice getVkPhysicalDevice() const {
        return vkPhysicalDevice;
    }

    void setVkPhysicalDevice(const VkPhysicalDevice _vkPhysicalDevice) {
        vkGlobalPool::vkPhysicalDevice = _vkPhysicalDevice;
    }

    const VkRenderPass getVkRenderPass() const {
        return vkRenderPass;
    }

    void setVkRenderPass(const VkRenderPass _vkRenderPass) {
        vkGlobalPool::vkRenderPass = _vkRenderPass;
    }

    const VkPipeline getVkPipeline() const {
        return vkPipeline;
    }

    void setVkPipeline(const VkPipeline _vkPipeline) {
        vkGlobalPool::vkPipeline = _vkPipeline;
    }

    const VkShaderModule getVertShaderModule() const {
        return vertShaderModule;
    }

    void setVertShaderModule(const VkShaderModule _vertShaderModule) {
        vkGlobalPool::vertShaderModule = _vertShaderModule;
    }

    const VkShaderModule getFragShaderModule() const {
        return fragShaderModule;
    }

    void setFragShaderModule(const VkShaderModule _fragShaderModule) {
        vkGlobalPool::fragShaderModule = _fragShaderModule;
    }

    const VkPipelineLayout getPipelineLayout() const {
        return pipelineLayout;
    }

    void setPipelineLayout(const VkPipelineLayout _pipelineLayout) {
        vkGlobalPool::pipelineLayout = _pipelineLayout;
    }

    const VkInstance getVkInstance() const {
        return vkInstance;
    }

    void setVkInstance(const VkInstance _vkInstance) {
        vkGlobalPool::vkInstance = _vkInstance;
    }

    const VkSwapchainKHR getSwapChain() const {
        return swapChain;
    }

    void setSwapChain(const VkSwapchainKHR _swapChain) {
        vkGlobalPool::swapChain = _swapChain;
    }

    VkFormat getSwapChainImageFormat() const {
        return swapChainImageFormat;
    }

    void setSwapChainImageFormat(VkFormat _swapChainImageFormat) {
        vkGlobalPool::swapChainImageFormat = _swapChainImageFormat;
    }

    const VkExtent2D getSwapChainExtent() const {
        return swapChainExtent;
    }

    void setSwapChainExtent(const VkExtent2D _swapChainExtent) {
        vkGlobalPool::swapChainExtent = _swapChainExtent;
    }

    std::vector<VkImage>&  getSwapChainImages() {
        return  swapChainImages;
    }

    std::vector<VkImageView>& getSwapChainImageViews() {
        return swapChainImageViews;
    }

     std::vector<VkFramebuffer>& getSwapChainFrameBuffers() {
        return swapChainFramebuffers;
    }

    const QueueFamilyIndices getQueueFamilyIndices() const {
        return queueFamilyIndices;
    }

    void setQueueFamilyIndices(const QueueFamilyIndices _queueFamilyIndices) {
        vkGlobalPool::queueFamilyIndices = _queueFamilyIndices;
    }

    const VkCommandPool getCommandPool() const {
        return commandPool;
    }

    void setCommandPool(const VkCommandPool _commandPool) {
        vkGlobalPool::commandPool = _commandPool;
    }
    const VkSemaphore getImageAvailableSemaphore(int i) {
        return imageAvailableSemaphores[i];
    }

    void setImageAvailableSemaphore(const VkSemaphore _imageAvailableSemaphore, int i) {
        vkGlobalPool::imageAvailableSemaphores[i] = _imageAvailableSemaphore;
    }

    const VkSemaphore getRenderFinishedSemaphore(int i) {
        return renderFinishedSemaphores[i];
    }

    void setRenderFinishedSemaphore(const VkSemaphore _renderFinishedSemaphore, int i) {
        vkGlobalPool::renderFinishedSemaphores[i] = _renderFinishedSemaphore;
    }
    const VkQueue getGraphicsQueue() const {
        return graphicsQueue;
    }

    void setGraphicsQueue(const VkQueue _graphicsQueue) {
        vkGlobalPool::graphicsQueue = _graphicsQueue;
    }

    const VkQueue getPresentQueue() const {
        return presentQueue;
    }

    void setPresentQueue(const VkQueue _presentQueue) {
        vkGlobalPool::presentQueue = _presentQueue;
    }

    const int& getMaxFramesInFlight() const {
        return MAX_FRAMES_IN_FLIGHT;
    }

    size_t& getCurrentFrame() {
        return currentFrame;
    }

    void setCurrentFrame(size_t currentFrame) {
        vkGlobalPool::currentFrame = currentFrame;
    }

    const std::vector<VkFence>& getInFlightFences(int i) {
        return inFlightFences;
    }

    void setInFlightFences(std::vector<VkFence>& _inFlightFences) {
        vkGlobalPool::inFlightFences = _inFlightFences;
    }

    const std::vector<VkFence> getImageInFlight() {
        return imagesInFlight;
    }

    void setImagesInFlight(std::vector<VkFence>& _imagesInFlight) {
        vkGlobalPool::imagesInFlight = _imagesInFlight;
    }

    const SwapChainSupportDetails &getSwapChainSupportDetails() {
        return swapChainSupportDetails;
    }

    void setSwapChainSupportDetails(const SwapChainSupportDetails &supportDetails) {
        vkGlobalPool::swapChainSupportDetails = supportDetails;
    }

    VkBuffer &getVertexBuffer() {
        return vertexBuffer;
    }

    void setVertexBuffer(VkBuffer _vertexBuffer) {
        vertexBuffer = _vertexBuffer;
    }

    VkBuffer &getIndexBuffer() {
        return indexBuffer;
    }

    void setIndexBuffer(VkBuffer _indexBuffer) {
        vertexBuffer = _indexBuffer;
    }

    VkDeviceMemory& getIndexBufferMemory() {
        return indexBufferMemory;
    }
    void setIndexBufferMemory( VkDeviceMemory _indexBufferMemory) {
        indexBufferMemory = _indexBufferMemory;
    }

    VkDeviceMemory& getVertexBufferMemory() {
        return vertexBufferMemory;
    }
    void setVertexBufferMemory( VkDeviceMemory _vertexBufferMemory) {
        vertexBufferMemory = _vertexBufferMemory;
    }

private:
    QueueFamilyIndices queueFamilyIndices;

    ///Vulkan-Instance-Creation?-Variables--------------///
    VkInstance vkInstance;
    VkSurfaceKHR vkSurfaceKhr;
    GLFWwindow *window;

    ///Device Variables---------------------------------///
    VkDevice vkDevice;
    VkPhysicalDevice vkPhysicalDevice;
    VkQueue graphicsQueue;
    VkQueue presentQueue;

    ///Pipeline Variables------------------------------///
    VkPipeline vkPipeline;
    VkPipelineLayout pipelineLayout;
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
    VkRenderPass vkRenderPass;

    ///Swapbuffer Variables----------------------------///
    VkSwapchainKHR swapChain;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    SwapChainSupportDetails swapChainSupportDetails;

    ///Buffers-----------------------------------------///
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;

    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
private:

    ///Swapbuffer Vectors-----------------------------///
    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    ///Rendering-------------------------------------///
    VkCommandPool commandPool;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;



public:
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;

private:
    ///Presentation----------------------------------///
    size_t currentFrame = 0;

    //VARIABLES-END--------------------------------------------------------------//




    //FUNCTIONS-START------------------------------------------------------------//

public:
    void findQueueFamilies(VkPhysicalDevice device) {
        ///Get device queue properties
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
        ///Find a queue family that supports VK_QUEUE_GRAPHICS_BIT.
        int i = 0;
        for (const auto &queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                queueFamilyIndices.graphicsFamily = i;
            }
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, vkGlobalPool::Get().getVkSurfaceKhr(), &presentSupport);
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                queueFamilyIndices.presentFamily = i;
            }
            if (queueFamilyIndices.isComplete()) {
                break;
            }
            i++;
        }
    }
    //FUNCTIONS-END------------------------------------------------------------//





};



#endif //NINEENGINE_VKGLOBALPOOL_H
