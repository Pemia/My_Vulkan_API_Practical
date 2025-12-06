#include "VertexInput.h"

void VertexInput::createVertexInputData()
{
	// Vertices
	vertices = {
	{{-0.5f, -0.5}, {0.0f, 0.5f, 0.3f}},
	{{0.5f, -0.5f}, {0.8f, 0.0f, 0.7f}},
	{{0.5f, 0.5f}, {0.1f, 0.8f, 0.4f}},
	{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
	};

	// Index of each Vertex
	vertexIndices = {0, 1, 2, 2, 3, 0};
}

VkVertexInputBindingDescription VertexInput::getBindingDescription()
{
	VkVertexInputBindingDescription bindingDescription{};

	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(VertexInfo);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return bindingDescription;
}

std::array<VkVertexInputAttributeDescription, 2> VertexInput::getAttributeDescriptions()
{
	std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions[0].offset = offsetof(VertexInfo, pos);

	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[1].offset = offsetof(VertexInfo, color);

	return attributeDescriptions;
}