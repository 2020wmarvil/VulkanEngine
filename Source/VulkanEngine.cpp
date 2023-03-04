#include "VulkanEngine.h"

#include "VulkanInitializers.h"
#include "VulkanTypes.h"
#include "Window.h"
#include "VkBootstrap.h"

void VulkanEngine::init()
{
	_window = std::make_unique<Window>(_windowExtent.width, _windowExtent.height, "Vulkan Engine");

	_isInitialized = true;
}

void VulkanEngine::cleanup()
{

}

void VulkanEngine::draw()
{}

void VulkanEngine::run()
{
    while (!_window->ShouldClose())
    {
        _window->PollEvents();

        // draw

        _window->SwapBuffers();
    }
}
