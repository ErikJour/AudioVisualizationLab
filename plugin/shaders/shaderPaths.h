//
// Created by Erik Jourgensen on 7/29/26.
//

#pragma once
#include <string>
#include <filesystem>

inline std::vector<std::filesystem::path> getShaderPaths() {
    const std::string dir = DEBUG_SHADER_DIR;
    std::vector<std::filesystem::path> shaderPaths = {
        dir + "/vertex_main.wgsl",
        dir + "/fragment_main.wgsl"
    };
    return shaderPaths;
}