#include "Devices.h"

void Devices::getPhysicalDevice(VkInstance instance, VkSurfaceKHR surface)
{
    // Enumerate Physical device the VkInstance can access
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    // If no device available, then none support Vulkan!
    if (deviceCount == 0) {
        throw std::runtime_error("Can't find GPUs that support Vulkan Instance!");
    }

    // Get list of Physical Devices
    std::vector<VkPhysicalDevice> deviceList(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, deviceList.data());

    for (const auto& device : deviceList) {
        if (checkDeviceSuitable(device, surface)) {
            physicalDevice = device;
            break;
        }
    }
}

void Devices::createLogicalDevice(VkSurfaceKHR surface, QueueFamily& queueFamilies)
{
    // Get Queue Family
    queueFamilies = QueueFamily::getQueueFamilies(physicalDevice, surface);

    // List for create Device Queue informations
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

    // Set of each queue family indice
    std::set<int> queueFamilyIndices = 
    {
        queueFamilies.graphicsFamily,
        queueFamilies.presentationFamily
    };

    // Create Queue Family information for Logical Device
    for (int queueFamilyIndex : queueFamilyIndices) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueCount = 1;
        float priority = 1.0f;
        queueCreateInfo.pQueuePriorities = &priority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    // Create Information for logical device
    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(DEVICE_EXTENSIONS.size());
    deviceCreateInfo.ppEnabledExtensionNames = DEVICE_EXTENSIONS.data();
    deviceCreateInfo.enabledLayerCount = 0;

    // Physical Device Features the Logical Device will be using
    VkPhysicalDeviceFeatures deviceFeature = {};
    deviceCreateInfo.pEnabledFeatures = &deviceFeature;				// Physical Device features Logical Device will use

    // Create the logical
    VkResult result = vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &logicalDevice);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to create a Logical Device");
    }

    // Queue are create at the same time as the device...
    // So we want handle to queues
    // From given logical device, of given Queue Family, of given Queue Index (0 since only one queue), place reference in given VkQueue
    vkGetDeviceQueue(logicalDevice, queueFamilies.graphicsFamily, 0, &queueFamilies.graphicsQueue);
    vkGetDeviceQueue(logicalDevice, queueFamilies.presentationFamily, 0, &queueFamilies.presentationQueue);
}

void Devices::clear()
{
    vkDestroyDevice(logicalDevice, nullptr);
}

bool Devices::checkDeviceSuitable(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
    QueueFamily indices = QueueFamily::getQueueFamilies(physicalDevice, surface);

    bool extensionsSupported = checkDeviceExtensionSupport(physicalDevice);

    bool swapChainValid = false;

    if (extensionsSupported) {
        SwapChain swapChainDetails;
        swapChainDetails.getSwapChainDetails(physicalDevice, surface);
        swapChainValid = !swapChainDetails.presentationModeDetails.empty() && !swapChainDetails.formatDetails.empty();
    }

    return indices.isValid() && extensionsSupported && swapChainValid;
}

bool Devices::checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice)
{
    uint32_t extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

    // If not found return false
    if (extensionCount == 0) {
        return false;
    }

    // List of extensions
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, extensions.data());

    bool hasExtension = false;
    // Check for Extensions
    for (const auto& deviceExtension : DEVICE_EXTENSIONS) {
        for (const auto& extension : extensions) {
            if (strcmp(deviceExtension, extension.extensionName) == 0) {
                hasExtension = true;
                break;
            }
        }
    }

    if (!hasExtension) {
        return false;
    }

    return true;
}
