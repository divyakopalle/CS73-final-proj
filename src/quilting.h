#pragma once

#include <iostream>
#include "floatimage.h"

FloatImage quilt_random(const FloatImage &sample, int outsize, int patchsize);
FloatImage quilt_simple(const FloatImage &sample, int out_size, int patch_size, int overlap, float tol);
FloatImage quilt_cut(const FloatImage &sample, int out_size, int patch_size, int overlap, float tol);
FloatImage min_boundary(const FloatImage &error, int overlap, int edge_case);
std::vector <std::vector<int>> find_patches(const FloatImage &sample, int patch_size);


int xy_to_i_upward(int x, int y, int width, int height);
int xy_to_i_leftward(int x, int y, int width, int height);
std::vector<int> i_to_xy_upward(int i, int width, int height);
std::vector<int> i_to_xy_leftward(int i, int width, int height);
