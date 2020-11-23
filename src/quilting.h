#pragma once

#include <iostream>
#include "floatimage.h"

FloatImage quilt_random(const FloatImage &sample, int outsize, int patchsize);
FloatImage quilt_simple(const FloatImage &sample, int out_size, int patch_size, int overlap, float tol=0.10);
FloatImage quilt_cut(const FloatImage &sample, int out_size, int patch_size, int overlap, float tol);
FloatImage min_boundary(FloatImage error, int overlap);
std::vector <std::vector<int>> find_patches(const FloatImage &sample, int patch_size);
