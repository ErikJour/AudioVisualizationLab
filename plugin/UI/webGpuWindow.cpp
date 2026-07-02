//
// Created by Erik Jourgensen on 6/11/26.
//

#include "webGpuWindow.h"

#define WGPU_STR(s) WGPUStringView{s, sizeof(s) -1}

WebGpuWindow::WebGpuWindow()  = default;
WebGpuWindow::~WebGpuWindow() = default;

bool WebGpuWindow::createInstance()
{
    WGPUInstanceDescriptor descriptor       = {};
    descriptor.nextInChain                  = nullptr;
    WGPUDawnTogglesDescriptor dawnToggles   = {};
    dawnToggles.chain.next                  = nullptr;
    dawnToggles.chain.sType                 = WGPUSType_DawnTogglesDescriptor;
    dawnToggles.disabledToggleCount         = 0;
    dawnToggles.enabledToggleCount          = 1;
    static constexpr auto toggleName        = "enable_immediate_error_handling";
    dawnToggles.enabledToggles              = &toggleName;
    descriptor.nextInChain                  = &dawnToggles.chain;
    mInstance
    = wgpuCreateInstance(&descriptor);

    if (!mInstance) {
        std::cerr << "Failed to crearte the WGPUInstance" << std::endl;
        return false;
    }
    std::cout << "WGPUInstance: " << mInstance << std::endl;
    return true;
}

bool WebGpuWindow::createAdapter()
{
    WGPURequestAdapterOptions adapterOptions    = {};
    adapterOptions.nextInChain                  = nullptr;
    mAdapter                                    = requestAdapterSync(mInstance, &adapterOptions);

    if (!mAdapter) {
        std::cerr << "Failed to get WGPUAdapter." << std::endl;
        return false;
    }
    getAdapter(mAdapter, mInitProperties);
    getLimits(mAdapter, mSupportedLimits);
    setFeatures(mAdapter);
    return true;
}

void WebGpuWindow::getAdapter(const WGPUAdapter adapter, const WGPUAdapterInfo& properties)
{
    wgpuAdapterGetInfo(adapter, &mInitProperties);
    std::cout << "Adapter backend: 0x" << std::hex << properties.backendType << std::dec << std::endl;
}

bool WebGpuWindow::createDevice()
{
    WGPUDeviceDescriptor deviceDescriptor         = {};
    deviceDescriptor.label                        = WGPU_STR("My device");
    deviceDescriptor.deviceLostCallbackInfo2.mode = WGPUCallbackMode_AllowSpontaneous;
    deviceDescriptor.requiredLimits               = nullptr;
    deviceDescriptor.deviceLostCallbackInfo2.callback = [](WGPUDevice const*,
                                                                        const WGPUDeviceLostReason reason,
                                                                        const WGPUStringView message,
                                                                            void*, void*) {
        if (reason == WGPUDeviceLostReason_Destroyed) return;
        std::cerr << "Device lost: reason " << reason;
        if (message.length > 0) std::cerr << " (" << message.data << ")";
        std::cerr << std::endl;
    };
    deviceDescriptor.uncapturedErrorCallbackInfo2.callback = [](WGPUDevice const*,
                                                                        const WGPUErrorType type,
                                                                        const WGPUStringView message,
                                                                            void*, void*) {
        std::cerr << "Uncaptured device error: type " << type;
        if (message.length > 0) std::cerr << " (" << message.data << ")";
        std::cerr << std::endl;
    };

    mDevice = requestDeviceSync(mInstance, mAdapter, &deviceDescriptor);

    if (!mDevice) {
        std::cerr << "Failed to secure WGPUDevice." << std::endl;
        return false;
    }

    return true;
}

void WebGpuWindow::configurePipeline()
{
    mFragmentState                                          = mScene.getFragmentState();
    mColorTarget                                            = mScene.getColorTarget();
    mBlendState                                             = mScene.getBlendState();
    //Pipeline
    mPipelineDescriptor.nextInChain                         = nullptr;
    mPipelineDescriptor.layout                              = nullptr;
    mPipelineDescriptor.vertex.bufferCount                  = 0;
    mPipelineDescriptor.vertex.buffers                      = nullptr;
    mPipelineDescriptor.vertex.entryPoint                   = WGPU_STR("vs_main");
    mPipelineDescriptor.vertex.constantCount                = 0;
    mPipelineDescriptor.vertex.constants                    = nullptr;
    mPipelineDescriptor.primitive.topology                  = WGPUPrimitiveTopology_TriangleList;
    mPipelineDescriptor.primitive.stripIndexFormat          = WGPUIndexFormat_Undefined;
    mPipelineDescriptor.primitive.frontFace                 = WGPUFrontFace_CCW;
    mPipelineDescriptor.primitive.cullMode                  = WGPUCullMode_None;
    mPipelineDescriptor.fragment                            = &mFragmentState;
    mPipelineDescriptor.depthStencil                        = &mDepthStencilState;
    setDefault(mDepthStencilState);
    mPipelineDescriptor.multisample.count                   = kMSAASamples;
    mPipelineDescriptor.multisample.mask                    = ~0u;
    mPipelineDescriptor.multisample.alphaToCoverageEnabled  = false;;
    //Depth Stencil
    mDepthStencilState.format                               = WGPUTextureFormat_Depth24Plus;
    mDepthStencilState.depthCompare                         = WGPUCompareFunction_Less;
    mDepthStencilState.depthWriteEnabled                    = WGPUOptionalBool_True;
    mDepthStencilState.stencilReadMask                      = 0;
    mDepthStencilState.stencilWriteMask                     = 0;
    //Color Target
    mColorTarget.blend                                      = &mBlendState;
    mColorTarget.writeMask                                  = WGPUColorWriteMask_All;
    //Blend State
    mBlendState.color.srcFactor                             = WGPUBlendFactor_SrcAlpha;
    mBlendState.color.dstFactor                             = WGPUBlendFactor_OneMinusSrcAlpha;
    mBlendState.color.operation                             = WGPUBlendOperation_Add;
    mBlendState.alpha.srcFactor                             = WGPUBlendFactor_Zero;
    mBlendState.alpha.dstFactor                             = WGPUBlendFactor_One;
    mBlendState.alpha.operation                             = WGPUBlendOperation_Add;
}
