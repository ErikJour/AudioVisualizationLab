//
// Created by Erik Jourgensen on 7/2/26.
//

#include "Scene.h"

#include <iostream>
#include <ostream>
//===================================================================================
//Setup
//====================================================================================
void Scene::init(WGPUDevice device, WGPUQueue queue)                   { mDevice = device; mQueue = queue;       }
void Scene::setSurface(const WGPUSurface surface)                      { mSurface                 = surface;      }
void Scene::setSurfaceFormat(const WGPUTextureFormat format)           { mSurfaceFormat           = format;       }
void Scene::setSurfaceSize(uint32_t width, uint32_t height)            { mWidth = width; mHeight  = height;       }
void Scene::setShaderModule(WGPUShaderModule shaderModule)             { mShaderModule            = shaderModule; }
void Scene::setPipelineDesc(WGPURenderPipelineDescriptor pipelineDesc) { mPipelineDesc            = pipelineDesc; }
void Scene::terminate()
{
    if (mSurface)   { wgpuSurfaceUnconfigure(mSurface); wgpuSurfaceRelease(mSurface); mSurface = nullptr; }
}
//===================================================================================
//Shader
//===================================================================================
bool Scene::createShader() {
#ifdef DEBUG
    mShaderPaths = getShaderPaths();
    mLastShaderWriteTime = latestWriteTime(mShaderPaths);
    mShaderModule = ResourceManager::loadShaderModules(mShaderPaths, mDevice);
#endif
    if (!mShaderModule) {
        std::cerr << "Failed to create shader module." << std::endl;
        return false;
    }
    return true;
}

void Scene::reloadShader()
{
    const WGPUShaderModule newShaderModule = ResourceManager::loadShaderModules(mShaderPaths, mDevice);
    if (!newShaderModule) {
        std::cerr << "Shader compile failed" << std::endl;
        return;
    }
    wgpuShaderModuleRelease(mShaderModule);
    mShaderModule = newShaderModule;
    mPipelineDesc.vertex.module = mShaderModule;
    createPipeline();
    std::cout << "Shader reloaded" << std::endl;
}
//====================================================================================
//Rendering
//====================================================================================
void Scene::configureVertexLayout() {
    //==========================================================
    //Position
    //=========================================================
    mVertexAttributes[0].shaderLocation = 0;
    mVertexAttributes[0].format         = WGPUVertexFormat_Float32x3;
    mVertexAttributes[0].offset         = 0;
    //==========================================================
    //Normal
    //=========================================================
    mVertexAttributes[1].shaderLocation = 1;
    mVertexAttributes[1].format         = WGPUVertexFormat_Float32x3;
    mVertexAttributes[1].offset         = 3 * sizeof(float);;
    //==========================================================
    //Color
    //=========================================================
    mVertexAttributes[2].shaderLocation = 2;
    mVertexAttributes[2].format         = WGPUVertexFormat_Float32x3;
    mVertexAttributes[2].offset         = 6 * sizeof(float);

    mVertexBufferLayout.resize(1);
    mVertexBufferLayout[0].attributeCount = 3;
    mVertexBufferLayout[0].attributes     = mVertexAttributes.data();
    mVertexBufferLayout[0].arrayStride    = 9 * sizeof(float);
    mVertexBufferLayout[0].stepMode = WGPUVertexStepMode_Vertex;
    mPipelineDesc.vertex.bufferCount = 1;
    mPipelineDesc.vertex.buffers = mVertexBufferLayout.data();
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
    const WGPUBindGroupLayout bglLayout       = wgpuDeviceCreateBindGroupLayout(mDevice, &bglDesc);
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
    if (!mDevice) {
        std::cout << "Failed to create device" << std::endl;
        return false;
    }

    mPipeline                   = wgpuDeviceCreateRenderPipeline(mDevice, &mPipelineDesc);

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
    updateTexture(mWidth, mHeight);
    wgpuBindGroupLayoutRelease(bglLayout);

    return true;
}

void Scene::updateTexture(const uint32_t width, const uint32_t height)
{
    //====================================================================================
    //Assign width and height
    //====================================================================================
    mWidth                             = width;
    mHeight                            = height;
    //====================================================================================
    //Release texture view and texture
    //====================================================================================
    if (mTextureView) { wgpuTextureViewRelease(mTextureView); mTextureView = nullptr; }
    if (mTexture)     { wgpuTextureRelease(mTexture);         mTexture     = nullptr; }
    //====================================================================================
    //Setup texture and texture view descriptors
    //====================================================================================
    WGPUTextureDescriptor desc         = {};
    desc.usage                         = WGPUTextureUsage_RenderAttachment;
    desc.dimension                     = WGPUTextureDimension_2D;
    desc.size                       = { mWidth, mHeight, 1 };
    desc.format                        = WGPUTextureFormat_Depth24Plus;
    desc.mipLevelCount                 = 1;
    desc.sampleCount                   = 1;
    WGPUTextureViewDescriptor descView = {};
    descView.format                    = WGPUTextureFormat_Depth24Plus;
    descView.dimension                 = WGPUTextureViewDimension_2D;
    descView.mipLevelCount             = 1;
    descView.arrayLayerCount           = 1;
    descView.aspect                    = WGPUTextureAspect_DepthOnly;
    //====================================================================================
    //Assign texture and texture view
    //====================================================================================
    mTexture                           = wgpuDeviceCreateTexture(mDevice, &desc);
    mTextureView                       = wgpuTextureCreateView(mTexture, &descView);
}

std::pair<WGPUSurfaceTexture, WGPUTextureView> Scene::getNextSurfaceViewData() const {
    //====================================================================================
    //Setup our surface to draw into - turn this into a single function that returns mSurfaceTexture and mTargetView
    //====================================================================================
    WGPUSurfaceTexture surfaceTexture = {};
    wgpuSurfaceGetCurrentTexture(mSurface, &surfaceTexture);
    if (surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_Success) {
        return { surfaceTexture, nullptr };
    }
    //====================================================================================
    //Let's format our blank texture
    //====================================================================================
    WGPUTextureViewDescriptor viewDescriptor = {};
    viewDescriptor.nextInChain               = nullptr;
    viewDescriptor.label                     = { "Surface texture view", 20};
    viewDescriptor.format                    = mSurfaceFormat;
    viewDescriptor.dimension                 = WGPUTextureViewDimension_2D;
    viewDescriptor.baseMipLevel              = 0;
    viewDescriptor.mipLevelCount             = 1;
    viewDescriptor.baseArrayLayer            = 0;
    viewDescriptor.arrayLayerCount           = 1;
    viewDescriptor.aspect                    = WGPUTextureAspect_All;
    const WGPUTextureView targetView         = wgpuTextureCreateView(surfaceTexture.texture, &viewDescriptor);

    return { surfaceTexture, targetView };
}

void Scene::renderFrame() const {
    //====================================================================================
    //Add reloadable shader setup here
    //====================================================================================
    if (!mPipeline) return;
    if (!mSurface)  return;
    //====================================================================================
    //Get the surface texture and the target view
    //====================================================================================
    auto [ surfaceTexture, targetView ] = getNextSurfaceViewData();
    //====================================================================================
    //Release Resources
    //====================================================================================
    wgpuTextureViewRelease(targetView);
    //====================================================================================
    //Present frame
    //====================================================================================
    wgpuSurfacePresent(mSurface);
}

//===================================================================================
//Meshes
//====================================================================================






