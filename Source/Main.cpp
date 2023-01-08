#include "Window.h"
#include "Pipeline.h"
#include "MathLib.h"

#include <iostream>

int main(int argc, char** argv)
{
	std::cout << "hi ronan\n" << std::endl;

    Window::Init();

    Pipeline pipeline("../Shaders/main.vert.spv", "../Shaders/main.frag.spv");
    std::cout << pipeline.GetExtensionCount() << " extensions supported\n";

    Window window(1920, 1080, "Vulkan Engine");
    window.Run();

    glm::mat4 matrix;
    glm::vec4 vec;
    auto test = matrix * vec;

    ComplexNumber cn1(1, 5);
    ComplexNumber cn2(3, 4);

    std::cout << cn1 + cn2 << std::endl;
    std::cout << cn1 - cn2 << std::endl;
    std::cout << cn1 * cn2 << std::endl;
    std::cout << cn1 / cn2 << std::endl;
    std::cout << cn1.Abs() << std::endl;
    std::cout << cn2.Abs() << std::endl;
    std::cout << cn1.Arg() << std::endl;
    std::cout << cn2.Arg() << std::endl;

    Window::Terminate();
}

