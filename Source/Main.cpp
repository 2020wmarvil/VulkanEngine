#include "Window.h"
#include "Pipeline.h"
#include "MathLib.h"

#include <iostream>

#include "glslang/Public/ShaderLang.h"

int main(int argc, char** argv)
{
	std::cout << "hi ronan\n" << std::endl;

    Window::Init();

    Pipeline pipeline("../Shaders/main.vert.spv", "../Shaders/main.frag.spv");
    std::cout << pipeline.GetExtensionCount() << " extensions supported\n";

    Window window(1920, 1080, "Vulkan Engine");
    window.Run();

    Window::Terminate();
}

