#pragma once

#include <iostream>
#include "floatimage.h"
// #include "quilting.h"
// #include "a2.h"

FloatImage corr_map(const FloatImage &source);

FloatImage transfer(const FloatImage &sample, const FloatImage &target, 
                    int patch_size, int overlap, float tol, float alpha, int iters, int i=1);