#pragma once

#include "VulkanTypes.h"
#include "Window.h"

#include <deque>
#include <functional>
#include <memory>
#include <vector>

struct VmaAllocator_T;
VK_DEFINE_HANDLE(VmaAllocator)

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

	VkRenderPass _renderPass;
	std::vector<VkFramebuffer> _framebuffers;

	VkCommandPool _commandPool;
	VkCommandBuffer _mainCommandBuffer;
	VkSemaphore _presentSemaphore, _renderSemaphore;
	VkFence _renderFence;

	VkPipelineLayout _trianglePipelineLayout;
	VkPipeline _trianglePipeline;

	VmaAllocator _allocator;
	DeletionQueue _mainDeletionQueue;
	
	void init();
	void init_vulkan();
	void init_swapchain();
	void init_commands();
	void init_default_renderpass();
	void init_framebuffers();
	void init_sync_structures();
	void init_pipelines();

	void cleanup();
	void draw();
	void run();

	bool load_shader_module(const char* filePath, VkShaderModule* outShaderModule);
};
