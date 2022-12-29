#pragma once

#include <stdint.h>

class Pipeline 
{
public:
    Pipeline();
    virtual ~Pipeline();

    //Pipeline(Pipeline const&) = delete;
    //Pipeline& operator=(Pipeline other) = delete;

    uint32_t GetExtensionCount();

    static void Init();
    static void Terminate();
private:
};
