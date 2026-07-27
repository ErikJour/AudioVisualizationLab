//
// Created by Erik Jourgensen on 7/2/26.
//

#include "Scene.h"

#include <iostream>
#include <ostream>

void Scene::init(WGPUDevice device, WGPUQueue queue) { mDevice = device; mQueue = queue; }

void Scene::setSurface(const WGPUSurface surface)    { mSurface = surface; }

void Scene::setSurfaceFormat(const WGPUTextureFormat format) { mSurfaceFormat = format; }

void Scene::setSurfaceSize(uint32_t width, uint32_t height) { mWidth = width; mHeight = height; }

void Scene::terminate()
{
    if (mSurface)   { wgpuSurfaceUnconfigure(mSurface); wgpuSurfaceRelease(mSurface); mSurface = nullptr; }
}

bool Scene::createPipeline()
{
    //====================================================================================
    //Configure communication from the CPU to the GPU
    //====================================================================================
    //Release bind group, uniform buffer, and pipeline so we can reset these
    //====================================================================================
    if (mBindGroup)     { wgpuBindGroupRelease(mBindGroup);     mBindGroup     = nullptr; }
    if (mUniformBuffer) { wgpuBufferRelease(mUniformBuffer);    mUniformBuffer = nullptr; }
    if (mPipeline)      { wgpuRenderPipelineRelease(mPipeline); mPipeline      = nullptr; }
    //====================================================================================
    //Rules for sending CPU data to the GPU
    //====================================================================================
    static constexpr uint32_t kAlignment      = 256;
    mUniformStride                            = (sizeof(MyUniforms) + kAlignment - 1) & ~(kAlignment - 1);
    WGPUBindGroupLayoutEntry bglEntry         = {};
    bglEntry.binding                          = 0;
    bglEntry.visibility                       = WGPUShaderStage_Vertex | WGPUShaderStage_Fragment;
    bglEntry.buffer.type                      = WGPUBufferBindingType_Uniform;
    bglEntry.buffer.hasDynamicOffset          = true;
    bglEntry.buffer.minBindingSize            = sizeof(MyUniforms);
    WGPUBindGroupLayoutDescriptor bglDesc     = {};
    bglDesc.entryCount                        = 1;
    bglDesc.entries                           = &bglEntry;
    const WGPUBindGroupLayout bglLayout       = nullptr;
    WGPUPipelineLayoutDescriptor pipelineDesc = {};
    pipelineDesc.bindGroupLayoutCount         = 1;
    pipelineDesc.bindGroupLayouts             = &bglLayout;
    if (mPipelineDesc.layout) {
        wgpuPipelineLayoutRelease(mPipelineDesc.layout);
        mPipelineDesc.layout = nullptr;
    }
    mPipelineDesc.layout = wgpuDeviceCreatePipelineLayout(mDevice, &pipelineDesc);
    //====================================================================================
    //Configure Shaders
    //====================================================================================
    mColorTarget.format         = mSurfaceFormat; //This is our pixel format
    mColorTarget.blend          = &mBlendState;   //This is our blend format
    mColorTarget.writeMask      = WGPUColorWriteMask_All; //All color channels can be written to
    mPipelineDesc.vertex.module = mShaderModule;
    mFragmentState.module       = mShaderModule;
    mFragmentState.entryPoint   = WGPU_STR("fs_main");
    mFragmentState.targetCount  = 1;
    mFragmentState.targets      = &mColorTarget;
    mFragmentState.constants    = nullptr;
    mPipelineDesc.fragment      = &mFragmentState;
    mPipeline = wgpuDeviceCreateRenderPipeline(mDevice, &mPipelineDesc);
    if (!mPipeline) {
        std::cerr << "Failed to create render pipeline." << std::endl;
        return false;
    }
    //====================================================================================
    //Allocate GPU Memory
    //====================================================================================
    WGPUBufferDescriptor bufferDesc = {};
    bufferDesc.size                 = materialCount * mUniformStride;
    bufferDesc.usage                = WGPUBufferUsage_Uniform | WGPUBufferUsage_CopyDst;
    mUniformBuffer                  = wgpuDeviceCreateBuffer(mDevice, &bufferDesc);
    WGPUBindGroupEntry bgEntry      = {};
    bgEntry.binding                 = 0;
    bgEntry.buffer                  = mUniformBuffer;
    bgEntry.offset                  = 0;
    bgEntry.size                    = sizeof(MyUniforms);
    WGPUBindGroupDescriptor bgDesc  = {};
    bgDesc.layout                   = bglLayout;
    bgDesc.entryCount               = 1;
    bgDesc.entries                  = &bgEntry;
    mBindGroup                      = wgpuDeviceCreateBindGroup(mDevice, &bgDesc);
    updateDepthTexture(mWidth, mHeight);
    wgpuBindGroupLayoutRelease(bglLayout);
    return true;
}





