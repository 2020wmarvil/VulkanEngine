#pragma once

struct GLFWwindow;

class Window 
{
public:
    Window(int width, int height, const char* name);
    virtual ~Window();

    Window(Window const&) = delete;
    Window& operator=(Window other) = delete;

    void Run();

    static void Init();
    static void Terminate();
private:
    GLFWwindow* window;
};
