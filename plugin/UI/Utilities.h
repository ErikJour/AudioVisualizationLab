//
// Created by Erik Jourgensen on 6/11/26.
//

#ifndef TRAININGHOUR_UTILITIES_H
#define TRAININGHOUR_UTILITIES_H

#include <cassert>
#include <iostream>
#include <webgpu/webgpu.h>
#include <filesystem>

inline WGPUAdapter requestAdapterSync(WGPUInstance instance, WGPURequestAdapterOptions const* options) {
    struct UserData {
        WGPUAdapter adapter  = nullptr;
        bool requestEnded    = false;
    };
    UserData userData;

    wgpuInstanceRequestAdapter(instance, options,
        [](WGPURequestAdapterStatus status, WGPUAdapter adapter, WGPUStringView message, void* pUserData) {
            UserData& userDataReference = *reinterpret_cast<UserData*>(pUserData);
            if (status == WGPURequestAdapterStatus_Success)
                userDataReference.adapter = adapter;
            else
                std::cout << "Could not get WebGPU Adapter: " << message.data << std::endl;
            userDataReference.requestEnded = true;
        },
        &userData
    );
}

#endif //TRAININGHOUR_UTILITIES_H