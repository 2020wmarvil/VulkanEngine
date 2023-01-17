#pragma once

#include "vulkan/vulkan.h"

#include <vector>

class Window;
struct QueueFamilyIndices;

class Renderer
{
public:
	Renderer(Window& window);
	~Renderer();

    Renderer(const Renderer&) = delete;
    void operator=(const Renderer&) = delete;

	VkDevice GetLogicalDevice();
	VkFormat GetSwapchainImageFormat();
private:
	void CreateVulkanInstance();
#if _DEBUG
	void CreateDebugMessenger();
	void DestroyDebugMessenger();
#endif
	void PickPhysicalDevice();
	QueueFamilyIndices GetQueueFamilyIndices();
	void CreateLogicalDevice();
	void CreateSwapchain();
	void CreateImageViews();
	
private:
	VkInstance instance;
	VkPhysicalDevice physicalDevice = nullptr;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	VkSurfaceKHR surface;
	Window* window;
	VkSwapchainKHR swapchain;
	std::vector<VkImage> swapchainImages;
	VkFormat swapchainImageFormat;
	VkExtent2D swapchainExtent;
	std::vector<VkImageView> swapchainImageViews;

#if _DEBUG
	VkDebugUtilsMessengerEXT debugMessenger;
#endif
};
