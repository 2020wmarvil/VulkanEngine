#pragma once

#include "VulkanTypes.h"
#include "VulkanMesh.h"
#include "Window.h"

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

#include <deque>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

/* TODO:
 * - Clean up all the hardcoded pipelines and meshes from VulkanEngine class
 * - Create multiple pipelines with newer shaders, and use them to render monkeys each with a different material each
 * - Load more meshes. As long as it�s an obj with TRIANGLE meshes, it should work fine. Make sure on export that the obj includes normals and colors
 * - Add WASD controls to the camera. For that, you would need to modify the camera matrices in the draw functions.
 * - Sort the renderables array before rendering by Pipeline and Mesh, to reduce number of binds.
 * - Instanced rendering + per instance custom data
 */

struct VmaAllocator_T;
VK_DEFINE_HANDLE(VmaAllocator)

struct Material
{
	VkPipeline pipeline;
	VkPipelineLayout pipelineLayout;
};

struct RenderObject
{
	Mesh* mesh;
	Material* material;
	glm::mat4 transformMatrix;
};

struct DeletionQueue
{
	std::deque<std::function<void()>> deletors;

	void push_function(std::function<void()>&& function) { deletors.push_back(function); }

	void flush()
	{
		for (auto it = deletors.rbegin(); it != deletors.rend(); it++)
		{ 
			(*it)(); 
		}

		deletors.clear();
	}
};

class PipelineBuilder
{
public:
	std::vector<VkPipelineShaderStageCreateInfo> _shaderStages;
	VkPipelineVertexInputStateCreateInfo _vertexInputInfo;
	VkPipelineInputAssemblyStateCreateInfo _inputAssembly;
	VkViewport _viewport;
	VkRect2D _scissor;
	VkPipelineRasterizationStateCreateInfo _rasterizer;
	VkPipelineColorBlendAttachmentState _colorBlendAttachment;
	VkPipelineMultisampleStateCreateInfo _multisampling;
	VkPipelineDepthStencilStateCreateInfo _depthStencil;
	VkPipelineLayout _pipelineLayout;

	VkPipeline build_pipeline(VkDevice device, VkRenderPass pass);
};

class VulkanEngine
{
public:
	bool _isInitialized{ false };
	int _frameNumber {0};

	VkExtent2D _windowExtent{ 1700 , 900 };
	std::unique_ptr<Window> _window{ nullptr };

	VkInstance _instance;
	VkDebugUtilsMessengerEXT _debug_messenger;
	VkPhysicalDevice _chosenGPU;
	VkDevice _device;
	VkSurfaceKHR _surface;
	VkQueue _graphicsQueue;
	uint32_t _graphicsQueueFamily;

	VkSwapchainKHR _swapchain;
	VkFormat _swapchainImageFormat;
	std::vector<VkImage> _swapchainImages;
	std::vector<VkImageView> _swapchainImageViews;

	VkImageView _depthImageView;
	AllocatedImage _depthImage;
	VkFormat _depthFormat;

	VkRenderPass _renderPass;
	std::vector<VkFramebuffer> _framebuffers;

	VkCommandPool _commandPool;
	VkCommandBuffer _mainCommandBuffer;
	VkSemaphore _presentSemaphore, _renderSemaphore;
	VkFence _renderFence;

	VmaAllocator _allocator;
	DeletionQueue _mainDeletionQueue;

	std::vector<RenderObject> _renderables;
	std::unordered_map<std::string, Material> _materials;
	std::unordered_map<std::string, Mesh> _meshes;

	VkPipelineLayout _meshPipelineLayout;
	VkPipeline _meshPipeline;
	Mesh _monkeyMesh, _triangleMesh;
	
	void init();
	void init_vulkan();
	void init_swapchain();
	void init_commands();
	void init_default_renderpass();
	void init_framebuffers();
	void init_sync_structures();
	void init_pipelines();
	void init_scene();

	void cleanup();
	void draw();
	void run();

	bool load_shader_module(const char* filePath, VkShaderModule* outShaderModule);
	void load_meshes();
	void upload_mesh(Mesh& mesh);

	Material* create_material(VkPipeline pipeline, VkPipelineLayout layout, const std::string& name);
	Material* get_material(const std::string& name);
	Mesh* get_mesh(const std::string& name);
	void draw_objects(VkCommandBuffer cmd,RenderObject* first, int count);
};
