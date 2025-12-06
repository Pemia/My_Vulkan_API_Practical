#include "GraphicsPipeline.h"

void GraphicsPipeline::createGraphicsPipeline(VkDevice logicalDevice, VkRenderPass renderPass, VkExtent2D swapChainExtent)
{
	// To actually use the shader we need to assign them to a specific pipeline stage
	// We will start by filling in the Info structure for the Vertex Shader
	VkPipelineShaderStageCreateInfo vertShaderCreateInfo{};

	vertShaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;								// Stage of Vertex Shader
	vertShaderCreateInfo.module = shaderModule.vertexShaderModule;											// Module of Vertex Shader
	vertShaderCreateInfo.pName = "main";													// Pointer Name of Vertex Shader for Fragment Shader

	// Next, pipeline stage info for Fragment Shader
	VkPipelineShaderStageCreateInfo fragShaderCreateInfo{};

	fragShaderCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;								// Stage of Fragment Shader
	fragShaderCreateInfo.module = shaderModule.fragmentShaderModule;											// Module of Fragment Shader
	fragShaderCreateInfo.pName = "main";													// Pointer name of Vertex Shader

	// Defining an array that contains these two struct, which will later use to reference them in the actual Pipeline Create step
	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderCreateInfo, fragShaderCreateInfo };

	std::vector<VkDynamicState> dynamicStates = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};

	// For Dynamic State
	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicState.pDynamicStates = dynamicStates.data();

	// Pipeline of Vertex Input
	// is Struct describes the format of the vertex data that will be passed to the Vertex Shader
	// Binding Descriptions : Space between data and whether the data is per-vertex or per-instance
	// Attriubute Descriptions : Type of the attributes passed to the Vertex Shader, Which binding to load them from and at which offset
	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};

	// Input for Vertex
	vertexInput.createVertexInputData();
	auto bindingDescription = vertexInput.getBindingDescription();
	auto attributeDescriptions = vertexInput.getAttributeDescriptions();

	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;

	// Ok, next it is Input Assembly, that describes two thing (vertex,fragment shader) what kind of geometry will drawn from vertices
	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};

	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;			// Properties of a geometric object that are preserved under continuouse deformations
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	// Viewport
	VkViewport viewport{};

	// Positions of Viewport
	viewport.x = 0.0f;
	viewport.y = 0.0f;

	// Width and Height of Viewport
	viewport.width = (float)swapChainExtent.width;
	viewport.height = (float)swapChainExtent.height;

	// Depth of Viewport (Normalize Size(0-1))
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	// Scissor
	VkRect2D scissor{};

	// Offset or range from starting position of Scissor
	scissor.offset = { 0,0 };					 // Center

	// Extent of Scissor
	scissor.extent = swapChainExtent;

	// Pipeline For Viewport and Scissors
	VkPipelineViewportStateCreateInfo viewportState{};

	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;		// Number of viewports
	viewportState.pViewports = &viewport;	// Viewport
	viewportState.scissorCount = 1;			// Number of scissors on viewports
	viewportState.pScissors = &scissor;		// Scissor

	// Rasterizer
	VkPipelineRasterizationStateCreateInfo rasterizer{};

	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;

	// Multisampling
	VkPipelineMultisampleStateCreateInfo multisampling{};

	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	// Color Blending after fragment shader has return a color, it need to combined with color that is already in the framebuffer
	// First
	VkPipelineColorBlendAttachmentState colorBlendAttachment{};

	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;

	// Second structure, reference the array of structures for all the framebuffers and allows you to set blend constants
	VkPipelineColorBlendStateCreateInfo colorBlending{};

	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;

	// Pipeline for layout in shader
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};

	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

	VkResult pipelineLayoutResult = vkCreatePipelineLayout(logicalDevice, &pipelineLayoutInfo, nullptr, &graphicsPipelineLayout);

	if (pipelineLayoutResult != VK_SUCCESS) {
		throw std::runtime_error("Failed to Create Pipeline Layout!");
	}

	// For Graphics Pipeline
	VkGraphicsPipelineCreateInfo pipelineInfo{};

	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;								// Vertex and Fragment Shader
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &dynamicState;
	pipelineInfo.layout = graphicsPipelineLayout;
	pipelineInfo.renderPass = renderPass;
	pipelineInfo.subpass = 0;

	VkResult graphicsPipelineResult = vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline);
	if (graphicsPipelineResult != VK_SUCCESS) {
		throw std::runtime_error("Failed to create Graphics Pipeline!");
	}

	// Claer unused Data
	shaderModule.DestroyShaderModule(logicalDevice);
}

void GraphicsPipeline::createShaderModuleInGraphicsPipeline(VkDevice logicalDevice, const std::string& vertexShaderFileName, const std::string& fragmentShaderFileName)
{
	shaderModule.createVertexShaderModule(logicalDevice, vertexShaderFileName);
	shaderModule.createFragmentShaderModule(logicalDevice, fragmentShaderFileName);
}

void GraphicsPipeline::clear(VkDevice logicalDevice)
{
	vkDestroyPipeline(logicalDevice, graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(logicalDevice, graphicsPipelineLayout, nullptr);
}