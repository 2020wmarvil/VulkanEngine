#include "Renderer.h"

#include "Window.h"

#include "glslang/Public/ShaderLang.h"

#include <iostream>
#include <optional>
#include <unordered_set>
#include <limits>
#include <algorithm>

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;
};

#if _DEBUG
static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) 
{
    std::cerr << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}
#endif

Renderer::Renderer(Window& window)
	: window(&window)
{
	CreateVulkanInstance();
#if _DEBUG
	CreateDebugMessenger();
#endif
	window.CreateSurface(instance, nullptr, &surface);
	PickPhysicalDevice();
	CreateLogicalDevice();
	CreateSwapchain();
	CreateImageViews();
}

Renderer::~Renderer()
{
	for (VkImageView& imageView : swapchainImageViews)
	{
		vkDestroyImageView(device, imageView, nullptr);
	}

	vkDestroySwapchainKHR(device, swapchain, nullptr);
	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyDevice(device, nullptr);
#if _DEBUG
	DestroyDebugMessenger();
#endif
	vkDestroyInstance(instance, nullptr);
}

VkDevice Renderer::GetLogicalDevice()
{
	return device;
}

VkFormat Renderer::GetSwapchainImageFormat()
{
	return swapchainImageFormat;
}

void Renderer::CreateVulkanInstance()
{
	VkApplicationInfo appInfo = { VK_STRUCTURE_TYPE_APPLICATION_INFO };
	appInfo.pApplicationName = "Vulkan Engine";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

	uint32_t apiVersion;
	vkEnumerateInstanceVersion(&apiVersion);

	appInfo.apiVersion = apiVersion;

	VkInstanceCreateInfo createInfo = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
	createInfo.pApplicationInfo = &appInfo;

	/* Layers */
	uint32_t availableLayerCount = 0;
	vkEnumerateInstanceLayerProperties(&availableLayerCount, nullptr);
	std::vector<VkLayerProperties> availableLayers(availableLayerCount);
	vkEnumerateInstanceLayerProperties(&availableLayerCount, availableLayers.data());

	std::cout << "There are " << availableLayerCount << " available Vulkan layers:\n";
	for (const VkLayerProperties& layer : availableLayers)
	{
	    std::cout << '\t' << layer.layerName << '\n';
	}

	std::vector<const char*> layerNames = { };

#if _DEBUG
	layerNames.push_back("VK_LAYER_KHRONOS_validation");
#endif

	createInfo.enabledLayerCount = layerNames.size();
	createInfo.ppEnabledLayerNames = layerNames.data();

	std::cout << "Enabling " << layerNames.size() << " Vulkan layers:\n";
	for (const char* extension : layerNames)
	{
	    std::cout << '\t' << extension << '\n';
	}

	for (const char* requiredLayer : layerNames)
	{
		bool bLayerFound = false;
		for (const VkLayerProperties& availableLayer : availableLayers)
		{
			if (*requiredLayer == *(availableLayer.layerName))
			{
				bLayerFound = true;
				break;
			}
		}

		if (!bLayerFound)
		{
			std::cerr << "required layer " << requiredLayer << " is not available\n";
		}
	}

	/* Extensions */
	uint32_t availableExtensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, nullptr);
	std::vector<VkExtensionProperties> availableExtensions(availableExtensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, availableExtensions.data());

	std::cout << "There are " << availableExtensionCount << " available Vulkan extensions:\n";
	for (const VkExtensionProperties& extension : availableExtensions)
	{
	    std::cout << '\t' << extension.extensionName << '\n';
	}

	std::vector<const char*> extensionNames = { };

#if _DEBUG
	extensionNames.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	for (uint32_t i = 0; i < glfwExtensionCount; ++i)
	{
		extensionNames.push_back(glfwExtensions[i]);
	}

	createInfo.enabledExtensionCount = extensionNames.size();
	createInfo.ppEnabledExtensionNames = extensionNames.data();

	std::cout << "Enabling " << extensionNames.size() << " Vulkan extensions:\n";
	for (const char* extension : extensionNames)
	{
	    std::cout << '\t' << extension << '\n';
	}

	for (const char* requiredExtension : extensionNames)
	{
		bool bExtensionFound = false;
		for (const VkExtensionProperties& availableExtension : availableExtensions)
		{
			if (*requiredExtension == *(availableExtension.extensionName))
			{
				bExtensionFound = true;
				break;
			}
		}

		if (!bExtensionFound)
		{
			std::cerr << "required extension " << requiredExtension << " is not available\n";
		}
	}

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
	{
		std::cerr << "failed to create Vulkan instance\n";
	}
}

#if _DEBUG
void Renderer::CreateDebugMessenger()
{
	VkDebugUtilsMessengerCreateInfoEXT createInfo = { VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT };
	createInfo.messageSeverity =
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType =
		VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = VulkanDebugCallback;
	createInfo.pUserData = nullptr;

	PFN_vkCreateDebugUtilsMessengerEXT CreateDebugMessengerFunc = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (CreateDebugMessengerFunc)
	{
		CreateDebugMessengerFunc(instance, &createInfo, nullptr, &debugMessenger);
	}
	else
	{
		std::cerr << "Failed to create debug messenger\n";
	}
}

void Renderer::DestroyDebugMessenger()
{
	PFN_vkDestroyDebugUtilsMessengerEXT DestroyDebugMessengerFunc = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (DestroyDebugMessengerFunc)
	{
		DestroyDebugMessengerFunc(instance, debugMessenger, nullptr);
	}
	else
	{
		std::cerr << "Failed to destroy debug messenger\n";
	}
}
#endif

void Renderer::PickPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	std::cout << "Available devices:\n";
	for (const VkPhysicalDevice& device : devices)
	{
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);
		std::cout << "\t" << deviceProperties.deviceName << "\n";

		if (physicalDevice == VK_NULL_HANDLE && deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		{
			physicalDevice = device;
		}

		// VkPhysicalDeviceFeatures deviceFeatures;
		// vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
	}

	// If no discrete GPU is available, choose the first available device
	if (physicalDevice == VK_NULL_HANDLE)
	{
		if (deviceCount > 0)
		{
			physicalDevice = devices[0];
		}
	}

	if (physicalDevice != VK_NULL_HANDLE)
	{
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
		std::cout << "Using device " << deviceProperties.deviceName << "\n";
	}
	else
	{
		std::cerr << "failed to find a suitable GPU\n";
	}
}

QueueFamilyIndices Renderer::GetQueueFamilyIndices()
{
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

	QueueFamilyIndices indices;
	for (uint32_t i = 0; i < queueFamilyCount; ++i)
	{
		const VkQueueFamilyProperties& queueFamily = queueFamilies[i];

		if (!indices.graphicsFamily.has_value() && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}

		if (!indices.presentFamily.has_value())
		{
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
			if (presentSupport)
			{
				indices.presentFamily = i;
			}
		}
	}

	return indices;
}

void Renderer::CreateLogicalDevice()
{
	QueueFamilyIndices indices = GetQueueFamilyIndices();

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::unordered_set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };
	
	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

	for (const char* requiredExtension : deviceExtensions)
	{
		bool bExtensionFound = false;
		for (const VkExtensionProperties& availableExtension : availableExtensions)
		{
			if (*requiredExtension == *(availableExtension.extensionName))
			{
				bExtensionFound = true;
				break;
			}
		}

		if (!bExtensionFound)
		{
			std::cerr << "required device extension " << requiredExtension << " is not available\n";
		}
	}

	VkPhysicalDeviceFeatures deviceFeatures{};

	VkDeviceCreateInfo createInfo = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.queueCreateInfoCount = queueCreateInfos.size();
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();
	createInfo.enabledExtensionCount = deviceExtensions.size();
	createInfo.pEnabledFeatures = &deviceFeatures;

	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
	{
		std::cerr << "Failed to create logical device\n";
	}

	if (indices.graphicsFamily.has_value())
	{
		vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
	}
	else
	{
		std::cerr << "Failed to create graphics family queue\n";
	}

	if (indices.presentFamily.has_value())
	{
		vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
	}
	else
	{
		std::cerr << "Failed to create present family queue\n";
	}
}

void Renderer::CreateSwapchain()
{
	VkSurfaceCapabilitiesKHR capabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
	std::vector<VkSurfaceFormatKHR> formats(formatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, formats.data());

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
	std::vector<VkPresentModeKHR> presentModes(presentModeCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data());

	if (formats.empty() || presentModes.empty())
	{
		std::cerr << "Swapchain is insufficient\n";
		return;
	}

	VkSurfaceFormatKHR format = formats[0];
	for (const VkSurfaceFormatKHR& availableFormat : formats)
	{
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			format = availableFormat;
        }
    }

	VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
	for (const VkPresentModeKHR& availablePresentMode : presentModes)
	{
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) // Note: this is not preferred on mobile devices, as it is less energy efficient
		{
			presentMode = availablePresentMode;
        }
    }

	VkExtent2D extent;
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		extent = capabilities.currentExtent;
	}
	else
	{
		int width, height;
		window->GetFramebufferSize(width, height);

		extent.width = std::clamp(static_cast<uint32_t>(width), capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		extent.height = std::clamp(static_cast<uint32_t>(height), capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
	}

	uint32_t imageCount = capabilities.minImageCount + 1; // to avoid waiting on the driver, it is recommended to have an extra image view
	if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
	{
		imageCount = capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo = { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
	createInfo.surface = surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = format.format;
	createInfo.imageColorSpace = format.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	createInfo.preTransform = capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	QueueFamilyIndices indices = GetQueueFamilyIndices();
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };
	if (indices.graphicsFamily != indices.presentFamily)
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = nullptr;
	}

	if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapchain) != VK_SUCCESS)
	{
		std::cerr << "Failed to create swapchain\n";
	}

	vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr);
	swapchainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(device, swapchain, &imageCount, swapchainImages.data());

	swapchainImageFormat = format.format;
	swapchainExtent = extent;
}

void Renderer::CreateImageViews()
{
	swapchainImageViews.resize(swapchainImages.size());

	for (size_t i = 0; i < swapchainImages.size(); ++i)
	{
		VkImageViewCreateInfo createInfo = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
		createInfo.image = swapchainImages[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = swapchainImageFormat;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(device, &createInfo, nullptr, &swapchainImageViews[i]) != VK_SUCCESS)
		{
			std::cerr << "Failed to create image view\n";
		}
	}
}
