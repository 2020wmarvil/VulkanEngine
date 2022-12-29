#include "Window.h"
#include "Pipeline.h"
#include "MathLib.h"

#include <iostream>

int main(int argc, char** argv)
{
	std::cout << "hi ronan\n" << std::endl;

    Pipeline::Init();
    Window::Init();

    Pipeline pipeline;
    std::cout << pipeline.GetExtensionCount() << " extensions supported\n";

    Window window(1920, 1080, "Vulkan Engine");
    window.Run();

    glm::mat4 matrix;
    glm::vec4 vec;
    auto test = matrix * vec;

    Window::Terminate();
    Pipeline::Terminate();
}

