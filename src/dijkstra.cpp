#include "dijkstra.h"
#include <math.h>
#include "utils.h"
#include <assert.h>
#include <iostream>
#include <cstdlib>


using namespace std;

// // Uses Dijkstra's Algorithm to find the lowest cost path through a given rectangle (horiz or vert) in the overlap region of the patch
// // Uses error image to iteratively populate the cost image to store lowest cumulative cost to reach each pixel
// // Paths are returned as a vector of integer indices recording each pixel i's previous pixel in the path
vector<int> find_paths(FloatImage &error, FloatImage &cost, int width, int height, int overlap)
{
    vector<vector<int>> visited;   // visited pixels
    vector<vector<int>> toVisit;    // next neighbors to visit
    vector<int> prev;              // for each pixel, its most updated previous pixel in best path 
                                   // (for backtracking), STORED AS INDICES, NOT COORDINATES
    
    bool vertical = false, horizontal = false;
    if (width == overlap) bool vertical = true;         // indicates this is a vertical rectangle
    else if (height == overlap) bool horizontal = true; // indicates this is a horizontal rectangle

    if (vertical) { 
        // Mark the first bottom row of pixels as initial pixels to visit
        for (int x = 0; x < width; x++) {
            toVisit.push_back({x, height-1});
        }
        // Initialize all the pixels' prev pixels (stored as indices in upward row-major order)
        // for backtracking later
        for (int y = cost.height()-1; y >= 0; y--) {
            for (int x = 0; x < overlap; x++) {
                prev.push_back(-1); // -1 indicates a prev that doesn't exist yet
            }
        }
        
    }
    else if (horizontal) { 
        // Mark the first rightmost column of pixels as initial pixels to visit
        for (int y = 0; y < overlap; y++) {
            toVisit.push_back({width-1, y});
        }
        // Initialize all the pixels' prev pixels (stored as indices in leftward column-major order)
        // for backtracking later
        for (int x = cost.width()-1; x >= 0; x--) {
            for (int y = 0; y < overlap; y++) {
                prev.push_back(-1); // -1 indicates a prev that doesn't exist yet
            }
        }
    }

    // As long as there are pixels to visit
    while (!toVisit.empty()) {
        // visit the last pixel in the toVisit vector
        vector<int> curr_pixel = toVisit[toVisit.size()-1];

        int curr_x = curr_pixel[0], curr_y = curr_pixel[1];
        int curr_idx;   // pixel location in index form
        if (vertical) int curr_idx = xy_to_i_upward(curr_x, curr_y, overlap, cost.height());
        else if (horizontal) int curr_idx = xy_to_i_leftward(curr_x, curr_y, cost.width(), overlap);

        visited.push_back(curr_pixel);
        toVisit.pop_back(); // don't visit it again

        // FIND THE CURRENT PIXEL'S NEIGHBORS
        vector<vector<int>> neighbors;

        if (vertical) {
            if (curr_y > 0) {
                vector<int> top = {curr_x, curr_y-1};
                neighbors.push_back(top);
                
                if (curr_x != 0) {              // if not at left edge
                    vector<int> left = {curr_x-1, curr_y-1};
                    neighbors.push_back(left);
                }
                if (curr_x != overlap - 1) {    // if not at right edge
                    vector<int> right = {curr_x+1, curr_y-1};
                    neighbors.push_back(right);
                }
            }
        }
        else if (horizontal) {
            if (curr_x > 0) {
                vector<int> left = {curr_x-1, curr_y};
                neighbors.push_back(left);

                if (curr_y != 0) {              // if not at top edge
                    vector<int> up = {curr_x-1, curr_y-1};
                    neighbors.push_back(up);
                }
                if (curr_y != overlap - 1) {    // if not at bottom edge
                    vector<int> down = {curr_x-1, curr_y+1};
                    neighbors.push_back(down);
                }
            }
        }
        
        // LOOP THROUGH THE NEIGHBORS
        for (auto i = 0; i < neighbors.size(); i++) {
            int nb_x = neighbors[i][0], nb_y = neighbors[i][1];
            int nb_idx;
            if (vertical) nb_idx = xy_to_i_upward(nb_x, nb_y, overlap, cost.height());
            else if (horizontal) nb_idx = xy_to_i_leftward(nb_x, nb_y, cost.width(), overlap);

            // COMPUTE CUMULATIVE COST TO GET TO NEIGHBOR
            float cumulative_cost = error(nb_x, nb_y) + cost(curr_x, curr_y);
            
            // IF THE COMPUTED CUMULATIVE COST IS LESS THAN THE EXISITNG CUMULATIVE COST, UPDATE IT
            if (cumulative_cost < cost(nb_x, nb_y)) {
                cost(nb_x, nb_y) = cumulative_cost;

                // ALSO UPDATE THIS NEIGHBOR'S PREV PIXEL TO BE THE CURRENT PIXEL TO TRACE BACK
                prev[nb_idx] = curr_idx;
            }

            // Only add this neighbor to toVisit if it's not already visited
            if (find(visited.begin(), visited.end(), neighbors[i]) == visited.end()) {
                toVisit.push_back({nb_x, nb_y});
            }
        }
    }
    return prev;
}


// Converts xy-coordinates to an upward row-major indexing scheme for given grid dimensions
int xy_to_i_upward(int x, int y, int width, int height)
{
    return (height-1-y)*(width) + x;
}

// Converts xy-coordinates to an leftward column-major indexing scheme for given grid dimensions
int xy_to_i_leftward(int x, int y, int width, int height)
{
    return (width-1-x)*(height) + y;
}

// Converts index i to xy-coordinates for upward row-major scheme
vector<int> i_to_xy_upward(int i, int width, int height)
{
    return {i % width, height - 1 - (i / width)};
}

// Converts index i to xy-coordinates for leftward row-major scheme
vector<int> i_to_xy_leftward(int i, int width, int height)
{
    return {width - 1 - (i / height), i % height};
}