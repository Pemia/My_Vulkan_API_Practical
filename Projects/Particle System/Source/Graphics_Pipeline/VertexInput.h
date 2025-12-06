#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include <vector>
#include <array>

class VertexInput
{
private:
	// Information of Vertex
	struct VertexInfo {
		glm::vec2 pos;
		glm::vec3 color;
	};
public:
	// Data of Vertex
	std::vector<VertexInfo> vertices;
	std::vector<uint16_t> vertexIndices;

	// Create Vertex Input Data Function
	void createVertexInputData();

	// Getter Function
	VkVertexInputBindingDescription getBindingDescription();
	std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();
};