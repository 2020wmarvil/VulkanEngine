#pragma once

#include "Renderer.h"

#include <stdint.h>
#include <string>
#include <vector>

#include "vulkan/vulkan.h"

// This abstraction is usefule because we want our application to be able to 
// configure the pipeline deeply, as well as share configurations between pipelines
struct PipelineConfigInfo 
{
    VkViewport viewport;
    VkRect2D scissor;
    VkPipelineViewportStateCreateInfo viewportInfo;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineColorBlendStateCreateInfo colorBlendInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    VkPipelineDynamicStateCreateInfo dynamicStateInfo;
    std::vector<VkDynamicState> dynamicStates;
};

class Pipeline 
{
public:
    Pipeline(Renderer& renderer, const PipelineConfigInfo& configInfo, const std::string& vertFilepath, const std::string& fragFilepath);
    virtual ~Pipeline();

    Pipeline(const Pipeline&) = delete;
    void operator=(const Pipeline&) = delete;

    static PipelineConfigInfo DefaultPipelineConfigInfo();
private:
    VkShaderModule CreateShaderModule(const std::vector<char>& code);
    void CreateRenderPass();
    void CreateGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath);
private:
    Renderer& renderer;
    PipelineConfigInfo configInfo;
    VkRenderPass renderPass;
    VkPipeline graphicsPipeline;
    VkPipelineLayout pipelineLayout;
};
