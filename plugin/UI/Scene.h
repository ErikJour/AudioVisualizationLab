//
// Created by Erik Jourgensen on 7/2/26.
//

#ifndef TRAININGHOUR_SCENE_H
#define TRAININGHOUR_SCENE_H
#include <webgpu/webgpu.h>
#include "MyUniforms.h"
#include <utility>

#define WGPU_STR(s) WGPUStringView { s, sizeof(s) -1 };
static constexpr uint32_t materialCount = 0;

class Scene {
public:
    Scene()  = default;
    ~Scene() = default;

    void init(WGPUDevice device, WGPUQueue queue);
    void setSurface(WGPUSurface surface);
    void setSurfaceFormat(WGPUTextureFormat format);
    void setSurfaceSize(uint32_t width, uint32_t height);
    void setPipelineDesc(WGPURenderPipelineDescriptor pipelineDesc);
    void terminate();
    bool createPipeline();
    void updateTexture(uint32_t width, uint32_t height);
    void renderFrame() const;
    std::pair<WGPUSurfaceTexture, WGPUTextureView> getNextSurfaceViewData() const;




    //Public Getter Functions======================================================
    WGPUFragmentState    getFragmentState() const { return mFragmentState; }
    WGPUColorTargetState getColorTarget()   const { return mColorTarget; }
    WGPUBlendState       getBlendState()    const { return mBlendState; }

private:
    WGPUFragmentState            mFragmentState    = {};
    WGPUColorTargetState         mColorTarget      = {};
    WGPUBlendState               mBlendState       = {};
    WGPURenderPipeline           mPipeline         = {};
    WGPUSurface                  mSurface          = nullptr;
    WGPUDevice                   mDevice           = nullptr;
    WGPUQueue                    mQueue            = nullptr;
    WGPUTextureFormat            mSurfaceFormat    = WGPUTextureFormat_Undefined;
    uint32_t                     mWidth            = 0;
    uint32_t                     mHeight           = 0;
    WGPUBuffer                   mUniformBuffer    = nullptr;
    WGPUBindGroup                mBindGroup        = nullptr;
    uint32_t                     mUniformStride    = 0;
    MyUniforms                   mUniforms         = {};
    WGPURenderPipelineDescriptor mPipelineDesc     = {};
    WGPUShaderModule             mShaderModule     = {};
    WGPUTextureView              mTextureView      = nullptr;
    WGPUTexture                  mTexture          = nullptr;
};


#endif //TRAININGHOUR_SCENE_H