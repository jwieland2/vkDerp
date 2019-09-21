#pragma once

#include <array>
#include <sstream>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include <glm/glm.hpp>

// Initialization structs
// ----------------------------------------------------------------------
struct DerpPipelineInit {
	std::string vertPath;
	std::string tescPath;
	std::string tesePath;
	std::string geomPath;
	std::string fragPath;
	bool tessellationEnabled = false;
	bool geometryEnabled = false;
	bool wireframe = false;
	uint32_t pushSize;
	vk::ShaderStageFlagBits pushStage = vk::ShaderStageFlagBits::eVertex;
	bool hasUniform = false;
	bool hasPushConstants = false;
};

struct DerpImageInit {
	std::string path = "";

};

struct DerpSamplerInit {
	vk::SamplerAddressMode repeat = vk::SamplerAddressMode::eRepeat;
	bool compareEnable = VK_FALSE;
	vk::CompareOp compareOp = vk::CompareOp::eAlways;
};

struct DerpDescriptorInit {
	vk::Sampler* sampler;
	vk::ImageView* textureView;
};

// Data types
// ----------------------------------------------------------------------

struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 texCoord;

	static vk::VertexInputBindingDescription getBindingDescription() {
		return vk::VertexInputBindingDescription(0, sizeof(Vertex), vk::VertexInputRate::eVertex);
	}

	static std::array<vk::VertexInputAttributeDescription, 3> getAttributeDescriptions() {
		std::array<vk::VertexInputAttributeDescription, 3> attributeDescriptions = {};

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = vk::Format::eR32G32B32Sfloat;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);
		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = vk::Format::eR32G32B32Sfloat;
		attributeDescriptions[1].offset = offsetof(Vertex, normal);
		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = vk::Format::eR32G32Sfloat;
		attributeDescriptions[2].offset = offsetof(Vertex, texCoord);
		return attributeDescriptions;
	}
};

struct mvp4 {
	glm::mat4 mvp;
};

// Shared values for tessellation control and evaluation stages
struct UBO {
	glm::mat4 projection;
	glm::mat4 modelview;
	glm::vec4 lightPos = glm::vec4(-48.0f, -40.0f, 46.0f, 0.0f);
	glm::vec4 frustumPlanes[6];
	float displacementFactor = 32.0f;
	float tessellationFactor = 0.75f;
	glm::vec2 viewportDim;
	// Desired size of tessellated quad patch edge
	float tessellatedEdgeSize = 20.0f;
};

//struct ubo {
//	glm::mat4 dummy = glm::mat4(1.0f);
//};

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
	std::ostringstream out;
	out.precision(n);
	out << std::fixed << a_value;
	return out.str();
}

