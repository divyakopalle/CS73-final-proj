#pragma once

#include <iostream>
#include "floatimage.h"
// #include "quilting.h"
// #include "a2.h"

FloatImage corr_map(const FloatImage &source);

FloatImage texture_transfer(const FloatImage &sample, const FloatImage &target, 
    int out_size, int patch_size, int overlap, float tol, float alpha)