#pragma once

#include <iostream>
#include "floatimage.h"

using namespace std;

// QUILTING METHODS
FloatImage quilt_random(const FloatImage &sample, int outsize, int patchsize);
<<<<<<< HEAD
FloatImage quilt_simple(const FloatImage &sample, int out_size, int patch_size, int overlap, float tol);
FloatImage quilt_cut(const FloatImage &sample, int out_size, int patch_size, int overlap, float tol);

// HELPER FUNCTIONS FOR QUILTING
vector<vector<int>> find_patches(const FloatImage &sample, int patch_size);
vector<int> best_patch(const FloatImage &sample, FloatImage &output, int out_size, int patch_size, int overlap, float tol, int out_x, int out_y, vector<vector<int>> patches);
FloatImage min_boundary(const FloatImage &error, int overlap, int edge_case);
vector<int> find_paths(const FloatImage &error, FloatImage &cost, int width, int height, int overlap);

// UTILITY FUNCTIONS FOR PIXEL COORDINATE-INDEX CONVERSION
int xy_to_i_upward(int x, int y, int width, int height);
int xy_to_i_leftward(int x, int y, int width, int height);
vector<int> i_to_xy_upward(int i, int width, int height);
vector<int> i_to_xy_leftward(int i, int width, int height);

// OTHER UTILITY METHODS
FloatImage logim(const FloatImage &im);
void check_args(const FloatImage &sample, int patch_size);
=======
FloatImage quilt_simple(const FloatImage &sample, int out_size, int patch_size, int overlap, float tol = 0.10);
FloatImage quilt_cut(const FloatImage &sample, int out_size, int patch_size, int overlap, float tol);
FloatImage min_boundary(FloatImage error, int overlap, bool left, bool top);
std::vector<std::vector<int>> find_patches(const FloatImage &sample, int patch_size);
>>>>>>> 9393bb5c6e4bfa5b9b208cf07da2aaeae0bc1f63
