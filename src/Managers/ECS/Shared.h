//
// Created by nineball on 6/2/21.
//

#ifndef NINEENGINE_SHARED_H
#define NINEENGINE_SHARED_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include <set>
#include <cstdint>
#include <vector>


using Entity = size_t;

class System
{
public:
    std::array<std::set<Entity>, 10> mEntities;
};

struct Transform
{
    glm::vec3 position;
    glm::vec3 rotation;
    int scale;
};

struct Forces {
    glm::vec3 velocity;
    glm::vec3 acceleration;
};

struct VkRenderable
{
    ///Generic buffers
    VkDeviceMemory indexBufferMemory = VK_NULL_HANDLE;
    VkDeviceMemory vertexBufferMemory = VK_NULL_HANDLE;
    VkBuffer indexBuffer = VK_NULL_HANDLE;
    VkBuffer vertexBuffer = VK_NULL_HANDLE;

    ///Texture
    VkImage textureImage = VK_NULL_HANDLE;
    VkDeviceMemory textureImageMemory = VK_NULL_HANDLE;
    VkImageView textureImageView = VK_NULL_HANDLE;

    uint32_t pipeline = 0;
};




#endif //NINEENGINE_SHARED_H