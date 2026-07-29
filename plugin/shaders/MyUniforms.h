//
// Created by Erik Jourgensen on 7/27/26.
//

#pragma once

struct MyUniforms {
    float time;
};

static_assert(sizeof(MyUniforms) % 4 == 0);
