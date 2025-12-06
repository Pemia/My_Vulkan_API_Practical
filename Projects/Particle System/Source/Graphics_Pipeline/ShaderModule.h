#pragma once

#include <vulkan/vulkan.h>

#include <vector>
#include <fstream>

class ShaderModule
{
public:
	// ShaderModule Objects for use in Graphics Pipeline operation
	VkShaderModule vertexShaderModule;
	VkShaderModule fragmentShaderModule;

	// Function to create Shader Module
	void createVertexShaderModule(VkDevice logicalDevice, const std::string& vertexShaderFileName);
	void createFragmentShaderModule(VkDevice logicalDevice, const std::string& fragmentShaderFileName);

	// Clear Function
	void DestroyShaderModule(VkDevice logicalDevice);
private:
	std::vector<char> readShaderFile(const std::string& filename);
	VkShaderModule createShaderModule(VkDevice logicalDevice, const std::vector<char>& binaryCode);
};