//
// Created by Erik Jourgensen on 7/2/26.
//

#ifndef TRAININGHOUR_SCENE_H
#define TRAININGHOUR_SCENE_H
#include <webgpu/webgpu.h>


class Scene {
public:
    Scene();
    ~Scene();

    //Public Getter Functions======================================================
    WGPUFragmentState    getFragmentState() const { return mFragmentState; }
    WGPUColorTargetState getColorTarget()   const { return mColorTarget; }
    WGPUBlendState       getBlendState()    const { return mBlendState; }

private:
    WGPUFragmentState    mFragmentState = {};
    WGPUColorTargetState mColorTarget   = {};
    WGPUBlendState       mBlendState    = {};
};


#endif //TRAININGHOUR_SCENE_H