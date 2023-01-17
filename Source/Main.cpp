#include "MathLib.h"
#include "Pipeline.h"
#include "Renderer.h"
#include "Window.h"

#include <iostream>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

int main(int argc, char** argv)
{
    Window::Init();

    /* Pipeline Creation
    * CreateGraphicsPipeline
    * CreateShaderModule
    */

    Window window(WINDOW_WIDTH, WINDOW_HEIGHT, "Vulkan Engine");
    Renderer renderer(window);

    Pipeline pipeline(renderer, Pipeline::DefaultPipelineConfigInfo(), "../Shaders/vert.spv", "../Shaders/frag.spv");

    window.Run();

    Window::Terminate();
}

