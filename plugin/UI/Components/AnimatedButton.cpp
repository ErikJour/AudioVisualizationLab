//
// Created by Erik Jourgensen on 9/1/26.
//

#include "AnimatedButton.h"

#include <iostream>
#include <ostream>

AnimatedButton::AnimatedButton() {}
AnimatedButton::~AnimatedButton() {}
void AnimatedButton::printMouseCoords(const float x, const float y) {
    std::cout << x << " : " << y << std::endl;
}
