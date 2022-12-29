#include "Pipeline.h"

#include <vulkan/vulkan.h>

Pipeline::Pipeline()
{ 
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

void Pipeline::Init()
{
}

void Pipeline::Terminate()
{
}
