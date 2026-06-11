//
// Created by Erik Jourgensen on 6/11/26.
//

#include "webGpuWindow.h"

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
