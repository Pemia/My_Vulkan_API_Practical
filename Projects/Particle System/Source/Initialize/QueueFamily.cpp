#include "QueueFamily.h"

// Check if Queue families are valid
bool QueueFamily::isValid()
{
    return graphicsFamily >= 0 && presentationFamily >= 0;
}

QueueFamily QueueFamily::getQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
    QueueFamily indics;

    // Get all Queue Families of this GPU(Physical Device)
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilyList(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyList.data());

    // Go through each queue family and check if it has at least 1 of the required type of queue
    int i = 0;
    VkBool32 presentationSupport = false;
    for (const auto& queueFamily : queueFamilyList) {
        // Check Graphics Queue First
        // Queue can be multiple types defined through. bitfield (Need to bitwise AND with VK_QUEUE_GRAPHICS_BIT to check if has required type)
        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indics.graphicsFamily = i;
        }

        // Check if Family having presentation queue family. (The Presentation Queue can be both Graphics and Presentation Queue)
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentationSupport);
        if (queueFamily.queueCount > 0 && presentationSupport) {
            indics.presentationFamily = i;
        }

        if (indics.isValid()) {
            break;
        }

        i++;
    }

    return indics;
}