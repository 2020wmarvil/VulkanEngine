#include "Pipeline.h"

#include <vulkan/vulkan.h>

#include <fstream>
#include <iostream>

Pipeline::Pipeline(const std::string& vertFilepath, const std::string& fragFilepath) {
    CreateGraphicsPipeline(vertFilepath, fragFilepath);
}

Pipeline::~Pipeline()
{
}

uint32_t Pipeline::GetExtensionCount()
{
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    return extensionCount;
}

std::vector<char> Pipeline::ReadFile(const std::string& filepath) {
    std::ifstream file{filepath, std::ios::ate | std::ios::binary};

    std::vector<char> buffer;

    if (!file.is_open()) {
        std::cerr << "failed to open file: " << filepath << std::endl;
        return buffer;
    }

    size_t fileSize = static_cast<size_t>(file.tellg());
    buffer.reserve(fileSize);
    
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    
    file.close();
    return buffer;
}

void Pipeline::CreateGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath) {
    auto vertCode = ReadFile(vertFilepath);
    auto fragCode = ReadFile(fragFilepath);
    
    std::cout << "Vertex Shader Code Size: " << vertCode.size() << '\n';
    std::cout << "Fragment Shader Code Size: " << fragCode.size() << '\n';
}
