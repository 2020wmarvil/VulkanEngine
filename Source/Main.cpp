#include "VulkanEngine.h"
#include "Window.h"

int main(int argc, char** argv)
{
	Window::Init();

    VulkanEngine engine;
	engine.init();
	engine.run();
	engine.cleanup();

	Window::Terminate();

	return 0;
}

