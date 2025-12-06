#include "ShaderModule.h"

void ShaderModule::createVertexShaderModule(VkDevice logicalDevice, const std::string& vertexShaderFileName)
{
	auto vertexShader = readShaderFile(vertexShaderFileName);
	vertexShaderModule = createShaderModule(logicalDevice, vertexShader);
}

void ShaderModule::createFragmentShaderModule(VkDevice logicalDevice, const std::string& fragmentShaderFileName)
{
	auto fragmentShader = readShaderFile(fragmentShaderFileName);
	fragmentShaderModule = createShaderModule(logicalDevice, fragmentShader);
}

void ShaderModule::DestroyShaderModule(VkDevice logicalDevice)
{
	vkDestroyShaderModule(logicalDevice, vertexShaderModule, nullptr);
	vkDestroyShaderModule(logicalDevice, fragmentShaderModule, nullptr);
}

std::vector<char> ShaderModule::readShaderFile(const std::string& filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	// Size of file
	size_t fileSize = (size_t)file.tellg();

	// Buffer of file
	std::vector<char> buffer(fileSize);

	// Seek Back to begining of the file and reaf all of bytes at once
	file.seekg(0);
	file.read(buffer.data(), fileSize);

	if (!file.is_open()) {
		throw std::runtime_error("failed to open file!");
	}

	// Close and return the bytes
	file.close();

	return buffer;
}

VkShaderModule ShaderModule::createShaderModule(VkDevice logicalDevice, const std::vector<char>& binaryCode)
{
	VkShaderModuleCreateInfo createInfo{};

	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = binaryCode.size();											// Size of code
	createInfo.pCode = reinterpret_cast<const uint32_t*>(binaryCode.data());			// Pointer to block is store byte of code

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(logicalDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module!");
	}

	return shaderModule;
}