#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class Window 
{
public:
    Window(int width, int height, const char* name);
    virtual ~Window();

    Window(Window const&) = delete;
    Window& operator=(Window other) = delete;

    void Run();
    void CreateSurface(VkInstance instance, const VkAllocationCallbacks* allocationCallbacks, VkSurfaceKHR* surface) const;
    void GetWindowSize(int& outWidth, int& outHeight);
    void GetFramebufferSize(int& outWidth, int& outHeight);

    static void Init();
    static void Terminate();
private:
    GLFWwindow* window;
};
