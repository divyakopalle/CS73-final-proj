#pragma once

#include <iostream>
#include "floatimage.h"
using namespace std;

FloatImage quilt_random(const FloatImage &sample, int outsize, int patchsize);
FloatImage quilt_simple(const FloatImage &sample, int out_size, int patch_size, int overlap, float tol);
vector<vector<int>> find_patches(const FloatImage &sample, int patch_size);
FloatImage quilt_cut(const FloatImage &sample, int out_size, int patch_size, int overlap, float tol);
vector<int> best_patch(const FloatImage &sample, FloatImage &output, int out_size, int patch_size, int overlap, float tol, int out_x, int out_y, vector<vector<int>> patches);
FloatImage min_boundary(const FloatImage &error, int overlap, int edge_case);
vector<int> find_paths(const FloatImage &error, FloatImage &cost, int width, int height, int overlap);

int xy_to_i_upward(int x, int y, int width, int height);
int xy_to_i_leftward(int x, int y, int width, int height);
vector<int> i_to_xy_upward(int i, int width, int height);
vector<int> i_to_xy_leftward(int i, int width, int height);
