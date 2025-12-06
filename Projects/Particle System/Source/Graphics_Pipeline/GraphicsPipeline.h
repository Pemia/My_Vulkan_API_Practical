#pragma once

#include "ShaderModule.h"
#include "VertexInput.h"

class GraphicsPipeline
{
public:
	// Graphics Pipeline Objects
	VkPipeline graphicsPipeline;
	VkPipelineLayout graphicsPipelineLayout;

	// Main Function of Grahics Pipeline
	void createGraphicsPipeline(VkDevice logicalDevice,VkRenderPass renderPass, VkExtent2D swapChainExtent);
	void createShaderModuleInGraphicsPipeline(VkDevice logicalDevice, const std::string& vertexShaderFileName, const std::string& fragmentShaderFileName);

	// Clear Function
	void clear(VkDevice logicalDevice);
private:
	// ShaderModule Object
	ShaderModule shaderModule;

	// Vertex Input Object
	VertexInput vertexInput;
};