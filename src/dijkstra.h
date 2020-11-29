#pragma once

#include <iostream>
#include "floatimage.h"


using namespace std;

vector<int> find_paths(const FloatImage &error, const FloatImage &cost, 
                       int width, int height, int overlap);

int xy_to_i_upward(int x, int y, int width, int height);
int xy_to_i_leftward(int x, int y, int width, int height);
vector<int> i_to_xy_upward(int i, int width, int height);
vector<int> i_to_xy_leftward(int i, int width, int height);