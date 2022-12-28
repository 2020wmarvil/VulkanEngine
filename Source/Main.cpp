#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>

static void KeyCallback(GLFWwindow* Window, int Key, int Scancode, int Action, int Mods)
{
    if (Key == GLFW_KEY_ESCAPE && Action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(Window, GLFW_TRUE);
    }
}

int main(int argc, char** argv)
{
	std::cout << "hi ronan\n" << std::endl;

    GLFWwindow* Window;
     
    if (!glfwInit())
    {
        exit(EXIT_FAILURE);
    }
 
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
 
    Window = glfwCreateWindow(1920, 1080, "Vulkan Engine", NULL, NULL);
    if (!Window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
 
    glfwSetKeyCallback(Window, KeyCallback);
 
    glfwMakeContextCurrent(Window);
    glfwSwapInterval(1);

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::cout << extensionCount << " extensions supported\n";

    glm::mat4 matrix;
    glm::vec4 vec;
    auto test = matrix * vec;
 
    while (!glfwWindowShouldClose(Window))
    {
        int Width, Height;
        glfwGetFramebufferSize(Window, &Width, &Height);
        glfwSwapBuffers(Window);
        glfwPollEvents();
    }
 
    glfwDestroyWindow(Window);
 
    glfwTerminate();

    exit(EXIT_SUCCESS);
}
