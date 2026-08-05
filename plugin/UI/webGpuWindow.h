//
// Created by Erik Jourgensen on 6/11/26.
//

#ifndef TRAININGHOUR_WEBGPUWINDOW_H
#define TRAININGHOUR_WEBGPUWINDOW_H

#include<webgpu/webgpu.h>
#include <iostream>
#include "Utilities.h"
#include "Scene.h"
#include "GpuSurface.h"
// #define WEBGPU_CPP_IMPLEMENTATION
// #include <webgpu/webgpu.hpp>


class WebGpuWindow {
public:
    WebGpuWindow();
    ~WebGpuWindow();
    bool createInstance();
    bool createAdapter();
    bool createDevice();
    bool createQueue();
    void configurePipeline();
    bool initialize();
    bool initSurface(double contentScale, uint32_t width, uint32_t height);
    void terminate();
    static void setFeatures(WGPUAdapter adapter);
    void getAdapter(WGPUAdapter adapter, const WGPUAdapterInfo &properties);
    static void getLimits(WGPUAdapter adapter, const WGPUSupportedLimits &limits);
    void applySurfaceConfig(uint32_t width,uint32_t height);


    Scene& getScene() { return mScene; }
    [[nodiscard]] bool hasSurface()     const { return mSurface != nullptr; }
    [[nodiscard]] void* getNativeView() const { return mNativeView; }
private:


    static void setDefault(WGPULimits &limits);
    static void setDefault(WGPUStencilFaceState &stencilFaceState);
    static void setDefault(WGPUDepthStencilState &depthStencilState);
    static WGPURequiredLimits GetRequiredLimit(WGPUAdapter adapter);

    //Variables
    void*           mNativeView = nullptr;
    WGPUInstance    mInstance   = nullptr;
    WGPUAdapter     mAdapter    = nullptr;
    WGPUDevice      mDevice     = nullptr;
    WGPUQueue       mQueue      = nullptr;
    WGPUSurface     mSurface    = nullptr;

    WGPUTextureFormat mSurfaceFormat = WGPUTextureFormat_Undefined;

    WGPUAdapterInfo                 mInitProperties     = {};
    WGPUSupportedLimits             mSupportedLimits    = {};
    WGPURenderPipelineDescriptor    mPipelineDescriptor = {};
    WGPUFragmentState               mFragmentState      = {};
    WGPUBlendState                  mBlendState         = {};
    WGPUColorTargetState            mColorTarget        = {};
    WGPUDepthStencilState           mDepthStencilState  = {};

    Scene mScene;
};


#endif //TRAININGHOUR_WEBGPUWINDOW_H