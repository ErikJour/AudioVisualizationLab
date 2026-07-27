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

bool WebGpuWindow::createQueue() {
    mQueue = wgpuDeviceGetQueue(mDevice);
    if (!mQueue) { return false; }
    std::cout << "WGPUQueue: " << mQueue << std::endl;
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
    //mScene.setPipelineDescriptor((mPipelineDescriptor);
}

bool WebGpuWindow::initialize()
{
    if (!createInstance())  return false;
    if (!createAdapter())   return false;
    if (!createDevice())    return false;
    if (!createQueue())     return false;
    mScene.init(mDevice, mQueue);
    // if (!mScene.createShader()) return false;
    configurePipeline();
    // mScene.configureVertexLayout();
    // mScene.initializeScene();
    return true;
}

bool WebGpuWindow::initSurface(const double contentScale, uint32_t width, uint32_t height)
{
    std::cout << "Init surface called" << width << "x" << height << std::endl;
    const MetalSurface metal    = createMetalSurface(mInstance, contentScale);
    mSurface                    = metal.surface;
    mNativeView                 = metal.view;
    if (!mSurface) { std::cerr << " Surface creation failed" << std::endl; return false;}

    applySurfaceConfig(width, height);
    mScene.setSurface(mSurface);
    mScene.setSurfaceFormat(mSurfaceFormat);
    mScene.setSurfaceSize(width, height);
    const bool result = mScene.createPipeline();
    std::cout << "Create Pipeline: " << result << std::endl;
    return result;
}

void WebGpuWindow::terminate()
{
    mScene.terminate();
    if (mPipelineDescriptor.layout) { wgpuPipelineLayoutRelease(mPipelineDescriptor.layout);    mPipelineDescriptor.layout  = nullptr; }
    if (mQueue)                     { wgpuQueueRelease(mQueue);                                 mQueue                      = nullptr; }
    if (mDevice)                    { wgpuDeviceRelease(mDevice);                               mDevice                     = nullptr; }
    if (mAdapter)                   { wgpuAdapterRelease(mAdapter);                             mAdapter                    = nullptr; }
    if (mInstance)                  { wgpuInstanceRelease(mInstance);                           mInstance                   = nullptr; }
}

void WebGpuWindow::setFeatures(const WGPUAdapter adapter)
{
    WGPUSupportedFeatures supported = {};
    wgpuAdapterGetFeatures(adapter, &supported);
    std::cout << "Adapter features: " << std::endl;
    std::cout << std::hex;
    for (size_t i = 0; i < supported.featureCount; i++)
        std::cout << " - 0x" << supported.features[i] << std::endl;
    std::cout << std::dec;
}

void WebGpuWindow::getAdapter(const WGPUAdapter adapter, const WGPUAdapterInfo& properties)
{
    std::cout << "Got adapter: " << adapter << std::endl;
    wgpuAdapterGetInfo(adapter, &mInitProperties);
    std::cout << " Adapter name: ";
    std::cout.write(properties.device.data, static_cast<std::streamsize>(properties.device.length));
    std::cout << std::endl;
    std::cout << "Adapter backend: 0x" << std::hex << properties.backendType << std::dec << std::endl;
}

void WebGpuWindow::getLimits(const WGPUAdapter /*adapter*/, const WGPUSupportedLimits &limits)
{
    std:: cout << "Adapter limits: " << std::endl;
    std::cout << " - maxTextureDimension1D: " << limits.limits.maxTextureDimension1D << std::endl;
    std::cout << " - maxTextureDimension2D: " << limits.limits.maxTextureDimension2D << std::endl;
    std::cout << " - maxTextureDimension3D: " << limits.limits.maxTextureDimension3D << std::endl;
    std::cout << " - maxTextureArrayLayers: " << limits.limits.maxTextureArrayLayers << std::endl;
}

void WebGpuWindow::setDefault(WGPULimits &limits) {
    limits.maxTextureDimension1D = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxTextureDimension2D = WGPU_LIMIT_U32_UNDEFINED;
    limits.maxTextureDimension3D = WGPU_LIMIT_U32_UNDEFINED;
}

void WebGpuWindow::setDefault(WGPUStencilFaceState& stencilFaceState)
{
    stencilFaceState.compare        = WGPUCompareFunction_Always;
    stencilFaceState.failOp         = WGPUStencilOperation_Keep;
    stencilFaceState.depthFailOp    = WGPUStencilOperation_Keep;
    stencilFaceState.passOp         = WGPUStencilOperation_Keep;
}

void WebGpuWindow::setDefault(WGPUDepthStencilState& depthStencilState)
{
    depthStencilState.format                = WGPUTextureFormat_Undefined;
    depthStencilState.depthWriteEnabled     = WGPUOptionalBool_False;
    depthStencilState.depthCompare          = WGPUCompareFunction_Always;
    depthStencilState.stencilReadMask       = 0xFFFFFFFF;
    depthStencilState.stencilWriteMask      = 0xFFFFFFFF;
    depthStencilState.depthBias             = 0;
    depthStencilState.depthBiasSlopeScale   = 0;
    depthStencilState.depthBiasClamp        = 0;

    setDefault(depthStencilState.stencilFront);
    setDefault(depthStencilState.stencilBack);
}

WGPURequiredLimits WebGpuWindow::GetRequiredLimit(WGPUAdapter adapter) {
    WGPUSupportedLimits supportedLimits                 = {};
    supportedLimits.nextInChain = nullptr;
    wgpuAdapterGetLimits(adapter, &supportedLimits);

    WGPURequiredLimits requiredLimits                   = {};
    setDefault(requiredLimits.limits);
    requiredLimits.limits.maxVertexAttributes           = 2;
    requiredLimits.limits.maxVertexBuffers              = 1;
    requiredLimits.limits.maxBufferSize                 = 15 * 5 * sizeof(float);
    requiredLimits.limits.maxVertexBufferArrayStride    = 9 * sizeof(float);
    requiredLimits.limits.maxInterStageShaderComponents = 3;
    return requiredLimits;
}

