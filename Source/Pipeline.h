#pragma once

#include <stdint.h>
#include <string>
#include <vector>

class Pipeline 
{
public:
    Pipeline(const std::string& vertFilepath, const std::string& fragFilepath);
    virtual ~Pipeline();

    uint32_t GetExtensionCount();
private:
    std::vector<char> ReadFile(const std::string& filepath);
    void CreateGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath);
};
