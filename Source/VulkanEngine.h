#pragma once

#include "VulkanTypes.h"
#include "Window.h"

#include <memory>

class VulkanEngine {
public:
	bool _isInitialized{ false };
	int _frameNumber {0};

	VkExtent2D _windowExtent{ 1700 , 900 };

	std::unique_ptr<Window> _window{ nullptr };
	
	//initializes everything in the engine
	void init();

	//shuts down the engine
	void cleanup();

	//draw loop
	void draw();

	//run main loop
	void run();
};