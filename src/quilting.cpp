#include "quilting.h"
#include <math.h>
#include "utils.h"
#include <assert.h>
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <thread>

using namespace std;

<<<<<<< HEAD
////////////////////////////////////////////////////
/* IMAGE QUILTING VIA RANDOM PLACEMENT OF BLOCKS */
///////////////////////////////////////////////////
FloatImage quilt_random(const FloatImage &sample, int outsize, int patch_size)
{
    check_args(sample, patch_size);
=======
// random placement of blocks
FloatImage quilt_random(const FloatImage &sample, int outsize, int patch_size)
{

    // Make sure patch_size is smaller than sample dimensions
    if (patch_size > sample.width() || patch_size > sample.height())
    {
        cout << "Error: Patch size must be smaller the sample image dimensions." << endl;
        exit(1);
    }
>>>>>>> 9393bb5c6e4bfa5b9b208cf07da2aaeae0bc1f63

    FloatImage output(outsize, outsize, sample.channels()); // initialize output image
    srand(time(NULL));                                      // seed the random number generator

    // loop over output image by intervals of the patch size
<<<<<<< HEAD
    for (int x = 0; x <= output.width(); x += patch_size)
    {
        for (int y = 0; y <= output.height(); y += patch_size)
        {
=======
    for (int x = 0; x <= output.width() - patch_size; x += patch_size)
    {
        for (int y = 0; y <= output.height() - patch_size; y += patch_size)
        {

>>>>>>> 9393bb5c6e4bfa5b9b208cf07da2aaeae0bc1f63
            // randomly pick a patch from sample image by defining its origin (top-left) pixel (making sure the patch won't go out of bands)
            int patch_x = rand() % (sample.width() - patch_size + 1);
            int patch_y = rand() % (sample.height() - patch_size + 1);

            // loop over the patch area in the output image and populate it with the corresponding pixels of the randomly sampled patch
<<<<<<< HEAD
            for (int x1 = 0; x1 < patch_size; x1++)
            {
                if (x + x1 == output.width()) break;
                for (int y1 = 0; y1 < patch_size; y1++)
                {
                    if (y + y1 == output.height()) break;
                    for (int c = 0; c < output.channels(); c++)
                    {
                        output(x + x1, y + y1, c) = sample(patch_x + x1, patch_y + y1, c);
=======
            for (int x1 = x; x1 < x + patch_size; x1++)
            {
                for (int y1 = y; y1 < y + patch_size; y1++)
                {
                    for (int c = 0; c < output.channels(); c++)
                    {
                        output(x1, y1, c) = sample(patch_x0 + x1 - x, patch_y0 + y1 - y, c);
>>>>>>> 9393bb5c6e4bfa5b9b208cf07da2aaeae0bc1f63
                    }
                }
            }
        }
    }
    return output;
}

/* Helper function: get a random sample of 10% of all possible patches, 
   stored as x,y coordinates of the patch's origin */
vector<vector<int>> find_patches(const FloatImage &sample, int patch_size)
{

<<<<<<< HEAD
=======
// helper function to get a random sample of 10% of all possible patches, stored as x,y coordinates of the patch's origin
vector<vector<int>> find_patches(const FloatImage &sample, int patch_size)
{

>>>>>>> 9393bb5c6e4bfa5b9b208cf07da2aaeae0bc1f63
    vector<vector<int>> all_coords; // stores all possible x,y coordinates possible
    vector<vector<int>> out_coords; // stores only the output x,y coordinates which are randomly selected

    // number of patches/pairs that we need to find (10% of all possible)
    int num_patches = (sample.width() - patch_size) * (sample.height() - patch_size) * 0.1f;

    // Save every possible coordinate as a vector in all_pairs
    for (int x = 0; x < sample.width() - patch_size; x++)
    {
        for (int y = 0; y < sample.height() - patch_size; y++)
        {
            vector<int> v{x, y};
            all_coords.push_back(v);
        }
    }
    // shuffle the coordinates
    random_shuffle(all_coords.begin(), all_coords.end());

<<<<<<< HEAD
    // select the first "num_patches" number of x,y pairs to be saved to out_pairs
    for (int i = 0; i < num_patches; i++)
    {
        out_coords.push_back(all_coords[i]);
    }
    return out_coords;
}

/////////////////////////////////////////////////////////////
/* IMAGE QUILTING VIA SIMPLE OVERLAPPING TO MINIMIZE ERROR */
/////////////////////////////////////////////////////////////
FloatImage quilt_simple(const FloatImage &sample, int out_size, int patch_size, int overlap, float tol)
{
    check_args(sample, patch_size);
    FloatImage output(out_size, out_size, sample.channels()); // initialize output image
    srand(time(NULL));

=======
    // select the first "num_patches number" of x,y pairs to be saved to out_pairs
    for (int i = 0; i < num_patches; i++)
    {
        out_coords.push_back(all_coords[i]);
        // cout << out_coords[i][0] << "," << out_coords[i][1] << endl;
    }

    return out_coords;
}

// Perform simple quilting by overlapping patches to minimize SSD in the overlapping regions
FloatImage quilt_simple(const FloatImage &sample, int out_size, int patch_size, int overlap, float tol)
{

    FloatImage output(out_size, out_size, sample.channels()); // initialize output image

    srand(time(NULL));

    // Make sure patch_size is smaller than sample dimensions
    if (patch_size > sample.width() || patch_size > sample.height())
    {
        cout << "Error: Patch size must be smaller the sample image dimensions." << endl;
        exit(1);
    }

>>>>>>> 9393bb5c6e4bfa5b9b208cf07da2aaeae0bc1f63
    // Randomly pick an initial patch from sample image and place it in top-left corner of output image
    int first_patch_x = rand() % (sample.width() - patch_size + 1);
    int first_patch_y = rand() % (sample.height() - patch_size + 1);

    for (int x = 0; x < patch_size; x++)
    {
        for (int y = 0; y < patch_size; y++)
        {
            for (int c = 0; c < output.channels(); c++)
            {
                output(x, y, c) = sample(first_patch_x + x, first_patch_y + y, c);
            }
        }
    }

    // use find_patches helper function to get a randomized list (x,y) patch coordinates
    vector<vector<int>> patches = find_patches(sample, patch_size);

<<<<<<< HEAD
    // Loop over output image in increments of (patch size - overlap) (making sure patches don't go out of bounds)
    for (int out_x = 0; out_x < output.width(); out_x += patch_size - overlap)
    {
        for (int out_y = 0; out_y < output.height(); out_y += patch_size - overlap)
        {
            cout << "Quilt simple at (" << out_x << ", " << out_y << ")" << endl;

            // Calculate the best choice of patch to place here
            vector<int> patch = best_patch(sample, output, out_size, patch_size, overlap, tol, out_x, out_y, patches);

            // Write this patch to the output image
            if (out_x > 0 || out_y > 0)
            {
                for (int x = 0; x < patch_size; x++)
                {
                    if ( (out_x) + x == output.width()) break;
                    for (int y = 0; y < patch_size; y++)
                    {
                        if ( (out_y) + y == output.height()) break;
                        for (int c = 0; c < output.channels(); c++)
                        {
                            output(out_x + x, out_y + y, c) = sample(patch[0] + x, patch[1] + y, c);
                        }
                    }
                }
            }
        }
    }
    return output;
}

/* Helper function: given a set of patches, calculate the best one to be placed 
   at each location in the output image, by minimizng sum of squared differences (SSD) */
vector<int> best_patch(const FloatImage &sample, FloatImage &output, int out_size, int patch_size, int overlap, float tol, int out_x, int out_y, vector<vector<int>> patches)
{
    vector<float> SSD_values;    // Vector that holds SSD values of all selected patches

    // Loop over all selected patches
    for (int i = 0; i < patches.size(); i++)
    {
        int patch_x = patches[i][0];
        int patch_y = patches[i][1];

        // initialize running sum of SSD for current patch
        float patch_SSD = 0.f;

        /* If we're not bordering the image's left-edge,
           compare SSDs of overlap between patch-left with output-right */
        if (out_x > 0)
        {
            for (int x1 = 0; x1 < overlap; x1++)
            {
                if ( (out_x + x1) == output.width()) break;
                for (int y1 = 0; y1 < patch_size; y1++)
                {
                    if ( (out_y + y1) == output.height()) break;
                    for (int c = 0; c < sample.channels(); c++)
                    {
                        patch_SSD += pow(sample(patch_x + x1, patch_y + y1, c) - output(out_x  + x1, out_y + y1, c), 2);
=======
    bool debug = true;
    // Loop over output image in increments of (patch size - overlap) (making sure patches don't go out of bounds)
    for (int out_x = 0; out_x < output.width() - patch_size; out_x += patch_size - overlap)
    {
        for (int out_y = 0; out_y < output.height() - patch_size; out_y += patch_size - overlap)
        {
            cout << "Current position in output image is "
                 << "(" << out_x << ", " << out_y << ")" << endl;

            // Vector that holds SSD values of all selected patches
            vector<float> SSD_values;

            // Loop over all selected patches
            for (int i = 0; i < patches.size(); i++)
            {
                int patch_x = patches[i][0];
                int patch_y = patches[i][1];

                // initialize SSD of the current patch to store running sum of its SSD
                float patch_SSD = 0.f;

                // If we're not bordering the image's left-edge then loop over the overlap of patch-left with output-right
                if (out_x > 0)
                {
                    // for x1 from 0 to overlap
                    for (int x1 = 0; x1 < overlap; x1++)
                    {
                        // for y1 from 0 to patchsize
                        for (int y1 = 0; y1 < patch_size; y1++)
                        {
                            // for each channel
                            for (int c = 0; c < sample.channels(); c++)
                            {
                                patch_SSD += pow(sample(patch_x + x1, patch_y + y1, c) - output(out_x - overlap + x1, out_y + y1, c), 2);
                            }
                        }
                    }
                }

                // If we're not bordering the image's top-edge, then loop over the overlap of patch-top with out-bottom
                if (out_y > 0)
                {
                    // for x1 from 0 to patchsize
                    for (int x1 = 0; x1 < patch_size; x1++)
                    {
                        // for y1 from 0 to overlap
                        for (int y1 = 0; y1 < overlap; y1++)
                        {
                            // for each channel
                            for (int c = 0; c < sample.channels(); c++)
                            {
                                patch_SSD += pow(sample(patch_x + x1, patch_y + y1, c) - output(out_x + x1, out_y - overlap + y1, c), 2);
                            }
                        }
                    }
                }

                // Record SSD for this patch
                SSD_values.push_back(patch_SSD);
                //cout << "SSD value for patch " << i << " is " << patch_SSD << endl;
            }

            // find the lowest values in SSD_values given by tol (tolerance)
            // randomly select one of those values, store the corresponding (x, y) in variables best_pat_x, best_pat_y

            // repeatedly take the next smallest SSD and put the corresponding patch into a new vector (best patches)
            vector<vector<int>> best_patches;

            for (int i = 0; i < SSD_values.size() * tol; i++)
            {
                int min_idx = 0;
                //cout << "START" << endl;
                for (int j = 0; j < SSD_values.size(); j++)
                {
                    if (SSD_values[j] < SSD_values[min_idx])
                    {
                        min_idx = j;
                        //cout << "updated global min=" << SSD_values[min_idx] << endl;
>>>>>>> 9393bb5c6e4bfa5b9b208cf07da2aaeae0bc1f63
                    }
                }
            }
        }

<<<<<<< HEAD
        /* If we're not bordering the image's top-edge,
           compare SSDs of overlap between patch-top with output-bottom */
        if (out_y > 0)
        {
            for (int x1 = 0; x1 < patch_size; x1++)
            {
                if ( (out_x + x1) == output.width()) break;
                for (int y1 = 0; y1 < overlap; y1++)
                {
                    if ( (out_y + y1) == output.height()) break;
                    for (int c = 0; c < sample.channels(); c++)
                    {
                        patch_SSD += pow(sample(patch_x + x1, patch_y + y1, c) - output(out_x + x1, out_y + y1, c), 2);
=======
            // randomly pick the index of ONE of the best patches
            int rand_idx = rand() % (int)(best_patches.size());

            // Write this patch to the output image
            if (debug)
            {
                if (out_x > 0 || out_y > 0)
                {
                    for (int x = 0; x < patch_size; x++)
                    {
                        for (int y = 0; y < patch_size; y++)
                        {
                            for (int c = 0; c < output.channels(); c++)
                            {
                                output(out_x + x, out_y + y, c) = sample(best_patches[rand_idx][0] + x, best_patches[rand_idx][1] + y, c);
                                //debug = false;
                            }
                        }
>>>>>>> 9393bb5c6e4bfa5b9b208cf07da2aaeae0bc1f63
                    }
                }
            }
        }
        SSD_values.push_back(patch_SSD);  // Record SSD for this patch
    }
<<<<<<< HEAD
=======
    return output;
}

// compute SSD (intensity of a - intensity of b)squared
>>>>>>> 9393bb5c6e4bfa5b9b208cf07da2aaeae0bc1f63

    /* Find the lowest values in SSD_values given by tol (tolerance) by repeatedly 
       taking the next smallest SSD and putting the corresponding patch into vector of best patches */
    vector<vector<int>> best_patches;
    for (int i = 0; i < SSD_values.size() * tol; i++)
    {
        int min_idx = 0;
        for (int j = 0; j < SSD_values.size(); j++)
        {
            if (SSD_values[j] < SSD_values[min_idx])
            {
                min_idx = j;
            }
        }
        best_patches.push_back(patches[min_idx]);
        SSD_values[min_idx] = 1e7; // remove it from consideration
    }

    // randomly pick ONE of the best patches to return
    int rand_idx = rand() % (int)(best_patches.size());
    return best_patches[rand_idx];
}


<<<<<<< HEAD
///////////////////////////////////////////////////
/* IMAGE QUILTING VIA MINIMUM-ERROR BOUNDARY CUT */
///////////////////////////////////////////////////
FloatImage quilt_cut(const FloatImage &sample, int out_size, int patch_size, int overlap, float tol)
{
    FloatImage output(out_size, out_size, sample.channels()); // initialize output image
=======
// }
// }
//minimum value of the last rown in F will indicate the end of the minimal vertical path through the surface
//trace back to find the path of best cut
//int j=n int i=1
// vector
FloatImage quilt_cut(const FloatImage &sample, int out_size, int patch_size, int overlap, float tol)
{

    FloatImage output(out_size, out_size, sample.channels()); // initialize output image

>>>>>>> 9393bb5c6e4bfa5b9b208cf07da2aaeae0bc1f63
    srand(time(NULL));
    check_args(sample, patch_size);

<<<<<<< HEAD
=======
    // Make sure patch_size is smaller than sample dimensions
    if (patch_size > sample.width() || patch_size > sample.height())
    {
        cout << "Error: Patch size must be smaller the sample image dimensions." << endl;
        exit(1);
    }

>>>>>>> 9393bb5c6e4bfa5b9b208cf07da2aaeae0bc1f63
    // Randomly pick an initial patch from sample image and place it in top-left corner of output image
    int first_patch_x = rand() % (sample.width() - patch_size + 1);
    int first_patch_y = rand() % (sample.height() - patch_size + 1);

<<<<<<< HEAD
    for (int x = 0; x < patch_size; x++) {
        for (int y = 0; y < patch_size; y++) {
            for (int c = 0; c < output.channels(); c++) {
=======
    for (int x = 0; x < patch_size; x++)
    {
        for (int y = 0; y < patch_size; y++)
        {
            for (int c = 0; c < output.channels(); c++)
            {
>>>>>>> 9393bb5c6e4bfa5b9b208cf07da2aaeae0bc1f63
                output(x, y, c) = sample(first_patch_x + x, first_patch_y + y, c);
            }
        }
    }

    // use find_patches helper function to get a randomized list (x,y) patch coordinates
    vector<vector<int>> patches = find_patches(sample, patch_size);
<<<<<<< HEAD

    // Loop over output image in increments of (patch size - overlap) (making sure patches don't go out of bounds)
    for (int out_x = 0; out_x < output.width(); out_x += patch_size - overlap)
    {
        for (int out_y = 0; out_y < output.height(); out_y += patch_size - overlap)
        {
            cout << "Quilt cut at (" << out_x << ", " << out_y << ")" << endl;

            vector<int> best_patch1 = best_patch(sample, output, out_size, patch_size, overlap, tol, out_x, out_y, patches);

            int chosen_patch_x = best_patch1[0], chosen_patch_y = best_patch1[1];

            // SAVE ERROR PATCH
            // stores SSD Value at each pixel in overlap region
            FloatImage error(patch_size, patch_size, 1);
            for (int i = 0; i < error.size(); i++) {
                error(i) = 0;
            } // initialize to all black pixels

            // Compute SSD at each pixel in overlap region
            if (out_x > 0) {
                for (int x1 = 0; x1 < overlap; x1++) {
                    if ( (out_x + x1) == output.width()) break;
                    for (int y1 = 0; y1 < patch_size; y1++) {
                        if ( (out_y + y1) == output.height()) break;
                        for (int c = 0; c < sample.channels(); c++) {
                            error(x1, y1) += pow(sample(chosen_patch_x + x1, chosen_patch_y + y1, c) 
                                             - output(out_x + x1, out_y + y1, c), 2);
                        }
                    }
                }
            }
            if (out_y > 0) {
                for (int x1 = 0; x1 < patch_size; x1++) {
                    if ( (out_x + x1) == output.width()) break;
                    for (int y1 = 0; y1 < overlap; y1++) {
                        if ( (out_y + y1) == output.height()) break;
                        for (int c = 0; c < sample.channels(); c++) {
                            error(x1, y1) += pow(sample(chosen_patch_x + x1, chosen_patch_y + y1, c) 
                                             - output(out_x + x1, out_y + y1, c), 2);
                        }
=======

    bool debug = true;
    // Loop over output image in increments of (patch size - overlap) (making sure patches don't go out of bounds)
    for (int out_x = 0; out_x < output.width() - patch_size; out_x += patch_size - overlap)
    {
        for (int out_y = 0; out_y < output.height() - patch_size; out_y += patch_size - overlap)
        {
            cout << "Current position in output image is "
                 << "(" << out_x << ", " << out_y << ")" << endl;

            // Vector that holds SSD values of all selected patches
            vector<float> SSD_values;

            // Loop over all selected patches
            for (int i = 0; i < patches.size(); i++)
            {
                int patch_x = patches[i][0];
                int patch_y = patches[i][1];

                // initialize SSD of the current patch to store running sum of its SSD
                float patch_SSD = 0.f;

                // If we're not bordering the image's left-edge then loop over the overlap of patch-left with output-right
                if (out_x > 0)
                {
                    // for x1 from 0 to overlap
                    for (int x1 = 0; x1 < overlap; x1++)
                    {
                        // for y1 from 0 to patchsize
                        for (int y1 = 0; y1 < patch_size; y1++)
                        {
                            // for each channel
                            for (int c = 0; c < sample.channels(); c++)
                            {
                                patch_SSD += pow(sample(patch_x + x1, patch_y + y1, c) - output(out_x - overlap + x1, out_y + y1, c), 2);
                            }
                        }
                    }
                }

                // If we're not bordering the image's top-edge, then loop over the overlap of patch-top with out-bottom
                if (out_y > 0)
                {
                    // for x1 from 0 to patchsize
                    for (int x1 = 0; x1 < patch_size; x1++)
                    {
                        // for y1 from 0 to overlap
                        for (int y1 = 0; y1 < overlap; y1++)
                        {
                            // for each channel
                            for (int c = 0; c < sample.channels(); c++)
                            {
                                patch_SSD += pow(sample(patch_x + x1, patch_y + y1, c) - output(out_x + x1, out_y - overlap + y1, c), 2);
                            }
                        }
                    }
                }

                // Record SSD for this patch
                SSD_values.push_back(patch_SSD);
                //cout << "SSD value for patch " << i << " is " << patch_SSD << endl;
            }

            // find the lowest values in SSD_values given by tol (tolerance)
            // randomly select one of those values, store the corresponding (x, y) in variables best_pat_x, best_pat_y

            // repeatedly take the next smallest SSD and put the corresponding patch into a new vector (best patches)
            vector<vector<int>> best_patches;

            for (int i = 0; i < SSD_values.size() * tol; i++)
            {
                int min_idx = 0;
                //cout << "START" << endl;
                for (int j = 0; j < SSD_values.size(); j++)
                {
                    if (SSD_values[j] < SSD_values[min_idx])
                    {
                        min_idx = j;
                        //cout << "updated global min=" << SSD_values[min_idx] << endl;
>>>>>>> 9393bb5c6e4bfa5b9b208cf07da2aaeae0bc1f63
                    }
                }
            }

<<<<<<< HEAD
            // error patches stored in FloatImage error, error patch stores the SSD values of each pixel in the overlap region
            // Use error patch to find the min-error boundary cut (returned as a binary mask)
            FloatImage cut_mask;
            int edge_case;
            if (out_x == 0)         edge_case = 1; // case where patch goes in first column
            else if (out_y == 0)    edge_case = 2; // case where patch goes in first row
            else                    edge_case = 3; // case where patch borders other patches above and to the left

            // Write this patch to the output image
            cut_mask = min_boundary(error, overlap, edge_case);

            if (out_x > 0 || out_y > 0) {
                for (int x = 0; x < patch_size; x++) {
                    if ( (out_x + x) == output.width()) break;
                    for (int y = 0; y < patch_size; y++) {
                        if ( (out_y) + y == output.height()) break;
                        for (int c = 0; c < output.channels(); c++) {
                            if (cut_mask(x, y) != 0) {
                                output(out_x + x, out_y + y, c) = 
                                sample(chosen_patch_x + x, chosen_patch_y + y, c);
=======
            // cout << "line 371" << endl;
            // randomly pick the index of ONE of the best patches
            int rand_idx = rand() % (int)(best_patches.size());
            // CHANGES WITH DYNAMIC PROGRAMMING //

            //save error patch
            //patch- existing outputimage
            //error patch stores SSD Value at each pixel
            FloatImage error(patch_size, patch_size, 1);
            for (int i = 0; i < error.size(); i++)
            {
                error(i) = 0;
            }
            if (out_x > 0)
            {
                // for x1 from 0 to overlap
                for (int x1 = 0; x1 < overlap; x1++)
                {
                    // for y1 from 0 to patchsize
                    for (int y1 = 0; y1 < patch_size; y1++)
                    {
                        // for each channel
                        for (int c = 0; c < sample.channels(); c++)
                        {
                            error(x1, y1, 0) += pow((sample(best_patches[rand_idx][0] + x1, best_patches[rand_idx][1] + y1, c) - output(out_x - overlap + x1, out_y + y1, c)), 2);
                            // cout << "error is " << x1 << " " << y1 << " :" << error(x1, y1, 0) << endl;
                        }
                    }
                }
            }
            if (out_y > 0)
            {
                // for x1 from 0 to patchsize
                for (int x1 = 0; x1 < patch_size; x1++)
                {
                    // for y1 from 0 to overlap
                    for (int y1 = 0; y1 < overlap; y1++)
                    {
                        // for each channel
                        for (int c = 0; c < sample.channels(); c++)
                        {
                            error(x1, y1, 0) += pow(sample(best_patches[rand_idx][0] + x1, best_patches[rand_idx][1] + y1, c) - output(out_x - overlap + x1, out_y + y1, c), 2);
                        }
                    }
                }
            }

            // error patches stored in FloatImage error, error patch stores the SSD values of each pixel in the overlap region
            // use error patches to find min boundary cut
            FloatImage cut_image;
            if (out_x == 0)
            {
                cut_image = min_boundary(error, overlap, true, false);
            }
            else if (out_y == 0)
            {
                cut_image = min_boundary(error, overlap, false, true);
            }
            else
            {
                cut_image = min_boundary(error, overlap, false, false);
            }
            // if (out_x > 1 && out_y > 1)
            // {
            //     return cut_image;
            // }
            // Write this patch to the output image

            if (debug)
            {
                if (out_x > 0 || out_y > 0)
                {
                    for (int x = 0; x < patch_size; x++)
                    {
                        for (int y = 0; y < patch_size; y++)
                        {
                            for (int c = 0; c < output.channels(); c++)
                            {
                                if (cut_image(x, y, 0) != 0)
                                {
                                    output(out_x + x, out_y + y, c) = sample(best_patches[rand_idx][0] + x, best_patches[rand_idx][1] + y, c);
                                }
                                // debug = false;
>>>>>>> 9393bb5c6e4bfa5b9b208cf07da2aaeae0bc1f63
                            }
                        }
                    }
                }
            }
        }
    }
    cout << "output is " << output.size() << endl;
    return output;
}

<<<<<<< HEAD

// Helper method to find the minimum boundary cut for a given error patch
// returns binary mask: white pixels indicate pixels in the patch to be kept, black pixels are cut out
FloatImage min_boundary(const FloatImage &error, int overlap, int edge_case)
{
    error.write(DATA_DIR "/output/error.png"); // visualization

    // binary mask image to be returned at end of algorithm, initialized as all 1s
    FloatImage binary_mask(error.width(), error.height(), error.channels());
    for (int i = 0; i < binary_mask.size(); i++) {
        binary_mask(i) = 1;
    }

    // Initialize cost image: will store the cumulative cost to reach each pixel once Dijkstra's algorithm completes
    FloatImage cost(error.width(), error.height(), error.channels());

    // if patch borders other patches above, search for lowest-cost HORIZONTAL path
    if (edge_case == 1)
    {
        // Find all paths using a Dijkstra's algorithm implementation (see find_paths)
        vector<int> paths = find_paths(error, cost, cost.width(), overlap, overlap);

        // TRACE BACK FROM LOWEST ENDING COST TO GET BEST PATH
        // Find the ending pixel (in left-most column) with lowest cumulative cost
        int min_idx = 0;
        int last_pixel;
        for (int y = 0; y < overlap; y++)
        {
            if (cost(0, y) < cost(0, min_idx))
            {
                min_idx = y;
            }
        }
        last_pixel = xy_to_i_leftward(0, min_idx, cost.width(), overlap);

        // Now trace back through paths to build the best path
        vector<int> best_path;
        best_path.push_back(last_pixel);

        int prev_pixel = paths[last_pixel];
        while (prev_pixel != -1)
        {
            best_path.push_back(prev_pixel);
            prev_pixel = paths[prev_pixel];
        }

        // Populate binary mask
        for (int x = cost.width() - 1; x >= 0; x--)
        {
            vector<int> path_pixel = i_to_xy_leftward(best_path[x], cost.width(), overlap);
            int path_pixel_y = path_pixel[1];
            for (int y = 0; y < path_pixel_y; y++)
            {
                binary_mask(x, y) = 0; // set pixels above the path to 0
            }
        }
    }

    // if patch borders other patches the left, search for lowest-cost VERTICAL path
    else if (edge_case == 2)
    {
        // FIND ALL PATHS WITH DIJKSTRA'S ALG
        vector<int> paths = find_paths(error, cost, overlap, cost.height(), overlap);

        // TRACE BACK FROM LOWEST ENDING COST TO GET BEST PATH
        // First find the ending pixel (in last row) with lowest cumulative cost
        int min_idx = 0;
        int last_pixel;
        for (int x = 0; x < overlap; x++) {
            if (cost(x, 0) < cost(min_idx, 0)) {
                min_idx = x;
            }
        }

        last_pixel = xy_to_i_upward(min_idx, 0, overlap, cost.height());

        // Now trace back through prev to build the minimum-cost path
        vector<int> best_path;
        best_path.push_back(last_pixel);

        int prev_pixel = paths[last_pixel];
        while (prev_pixel != -1) {
            best_path.push_back(prev_pixel);
            prev_pixel = paths[prev_pixel];
        }

        // Populate binary mask
        for (int y = cost.height() - 1; y >= 0; y--) {
            vector<int> path_pixel = i_to_xy_upward(best_path[y], overlap, cost.height());
            int path_pixel_x = path_pixel[0];
            for (int x = 0; x < path_pixel_x; x++) {
                binary_mask(x, y) = 0; // set pixels to the left of the path to 0
            }
        }
    }
    else if (edge_case == 3)
    {
        FloatImage cost_ver(cost);
        FloatImage cost_hor(cost);

        // find both vertical and horizontal paths
        vector<int> prev_hor = find_paths(error, cost_hor, cost.width(), overlap, overlap);
        vector<int> prev_vert = find_paths(error, cost_ver, overlap, cost.height(), overlap);

        // VERTICAL BACKTRACKING
        int last_pixel_x_vertical = 0;
        int last_pixel_i_vertical;
        for (int x = 0; x < overlap; x++)
        {
            if (cost_ver(x, 0) < cost_ver(last_pixel_x_vertical, 0))
            {
                last_pixel_x_vertical = x;
            }
        }
        last_pixel_i_vertical = xy_to_i_upward(last_pixel_x_vertical, 0, overlap, cost.height());

        // trace back vertically to build the minimum-cost vertical path
        vector<int> best_path_vertical;
        best_path_vertical.push_back(last_pixel_i_vertical);

        int prev_pixel_down = prev_vert[last_pixel_i_vertical];
        while (prev_pixel_down != -1)
        {
            best_path_vertical.push_back(prev_pixel_down);
            vector<int> curr_pixel = i_to_xy_upward(prev_pixel_down, overlap, cost.height());

            prev_pixel_down = prev_vert[prev_pixel_down];
            vector<int> prev_pixel = i_to_xy_upward(prev_pixel_down, overlap, cost.height());
        }
        
        //HORIZONTAL BACKTRACKING
        int last_pixel_y_horizontal = 0;
        int last_pixel_i_horizontal;
        for (int y = 0; y < overlap; y++)
        {
            if (cost_hor(0, y) < cost_hor(0, last_pixel_y_horizontal))
            {
                last_pixel_y_horizontal = y;
            }
        }
        last_pixel_i_horizontal = xy_to_i_leftward(0, last_pixel_y_horizontal, cost_hor.width(), overlap);
        last_pixel_i_vertical = xy_to_i_upward(last_pixel_x_vertical, 0, overlap, cost_ver.height());

        // trace back horizontally to build the minimum-cost horizontal path
        vector<int> best_path_horizontal;
        best_path_horizontal.push_back(last_pixel_i_horizontal);

        int prev_pixel_right = prev_hor[last_pixel_i_horizontal];
        while (prev_pixel_right != -1)
        {
            best_path_horizontal.push_back(prev_pixel_right);
            prev_pixel_right = prev_hor[prev_pixel_right];
        }

        // Populate binary mask vertically
        for (int y = 0; y < best_path_vertical.size(); y++)
        {
            vector<int> path_pixel = i_to_xy_upward(best_path_vertical[y], overlap, cost.height());
            int path_pixel_x = path_pixel[0];
            for (int x = 0; x < path_pixel_x; x++)
            {
                binary_mask(x, y) = 0; // set pixels to the left of the path to 0
            }
        }

        // Populate binary mask horizontally
        for (int x = 0; x < best_path_horizontal.size(); x++)
        {
            vector<int> path_pixel = i_to_xy_leftward(best_path_horizontal[x], cost.width(), overlap);
            int path_pixel_y = path_pixel[1];
            for (int y = 0; y < path_pixel_y; y++)
            {
                binary_mask(x, y) = 0; // set pixels above the path to 0
            }
        }
    }
    binary_mask.write(DATA_DIR "/output/mask.png");
    return binary_mask;
}

// Implementation of Dijkstra's algorithm to find the lowest cost path through a given rectangle (horiz or vert) in the overlap region of the patch
// Uses error patch to iteratively populate the cost image to store lowest cumulative cost to reach each pixel
// Paths are returned as a vector of integer indices recording each pixel i's previous pixel in the path
vector<int> find_paths(const FloatImage &error, FloatImage &cost, int width, int height, int overlap)
{
    vector<vector<int>> unvisited; // unvisited pixels
    // mark all pixels as unvisited to begin with
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            unvisited.push_back({x, y});
        }
    }

    // initialize all costs to infinty
    for (int i = 0; i < cost.size(); i++) {
        cost(i) = INFINITY; 
    }
    
    vector<int> prev;            // for each pixel, its most updated previous pixel in best path
                                 // (for backtracking), STORED AS INDICES, NOT COORDINATES

    bool vertical = false, horizontal = false;
    if (width == overlap)
        vertical = true; // indicates this is a vertical rectangle
    else if (height == overlap)
        horizontal = true; // indicates this is a horizontal rectangle
    if (vertical)
    {
        // Initialize all the pixels' prev pixels (stored as indices in upward row-major order)
        // for backtracking later
        for (int y = cost.height() - 1; y >= 0; y--)
        {
            for (int x = 0; x < overlap; x++)
            {
                prev.push_back(-1); // -1 indicates a prev that doesn't exist yet
            }
        }
        for (int x = 0; x < overlap; x++) {
            cost(x, cost.height()-1) = error(x, cost.height()-1);
        }
    }
    else if (horizontal)
    {
        // Initialize all the pixels' prev pixels (stored as indices in leftward column-major order)
        // for backtracking later
        for (int x = cost.width() - 1; x >= 0; x--)
        {
            for (int y = 0; y < overlap; y++)
            {
                prev.push_back(-1); // -1 indicates a prev that doesn't exist yet
            }
        }
        for (int y = 0; y < overlap; y++) {
            cost(cost.width()-1, y) = error(cost.width()-1, y);
        }
    }

    // START OF SEARCH
    bool start = true;
    // As long as there are pixels to visit
    while (!unvisited.empty())
    {
        // SORT unvisited from biggest to smallest in order to extract the smallest each time
        for (int i = 0; i < unvisited.size(); i++) {
            int max_idx = i;
            int max_x = unvisited[max_idx][0], max_y = unvisited[max_idx][1];
            float max_cost = cost(max_x, max_y);
            for (int j = i+1; j < unvisited.size(); j++) {
                float next_x = unvisited[j][0], next_y = unvisited[j][1];
                float next_cost = cost(next_x, next_y);
                if (next_cost > max_cost) {
                    swap(unvisited[max_idx], unvisited[j]);
                }
            }
        }

        // FIND THE CURRENT PIXEL'S NEIGHBORS
        vector<vector<int>> neighbors;
        int curr_x, curr_y, curr_idx; // keep track of current location

        if (start) {
            if (vertical) {
                // initial neighbors are the first bottom row of pixels
                for (int x = 0; x < width; x++) {
                    neighbors.push_back({x, height - 1});
                }
                start = false;
            }
            else if (horizontal) {
                // initial neighbors are the first rightmost column of pixels
                for (int y = 0; y < overlap; y++) {
                    neighbors.push_back({width - 1, y});
                }
                start = false;
            }
        }

        else if (!start) {
            // visit the pixel with smallest known cumulative cost in the toVisit vector
            vector<int> curr_pixel = unvisited[unvisited.size() - 1];
            curr_x = curr_pixel[0], curr_y = curr_pixel[1];
            if (vertical)
                curr_idx = xy_to_i_upward(curr_x, curr_y, overlap, cost.height());
            else if (horizontal)
                curr_idx = xy_to_i_leftward(curr_x, curr_y, cost.width(), overlap);
        
            if (vertical)
            {
                if (curr_y > 0) 
                { // no neighbors for top row
                    vector<int> top = {curr_x, curr_y - 1};
                    neighbors.push_back(top);

                    if (curr_x != 0)
                    { // if not at left edge
                        vector<int> left = {curr_x - 1, curr_y - 1};
                        neighbors.push_back(left);
                    }
                    if (curr_x != overlap - 1)
                    { // if not at right edge
                        vector<int> right = {curr_x + 1, curr_y - 1};
                        neighbors.push_back(right);
                    }
                }
            }
            else if (horizontal)
            {
                if (curr_x > 0) // no neighbors for leftmost row 
                {
                    vector<int> left = {curr_x - 1, curr_y};
                    neighbors.push_back(left);

                    if (curr_y != 0)
                    { // if not at top edge
                        vector<int> up = {curr_x - 1, curr_y - 1};
                        neighbors.push_back(up);
                    }
                    if (curr_y != overlap - 1)
                    { // if not at bottom edge
                        vector<int> down = {curr_x - 1, curr_y + 1};
                        neighbors.push_back(down);
                    }
                }
            }
        }

        // LOOP THROUGH THE NEIGHBORS AND COMPUTE TENTATIVE CUMULATIVE COST
        for (int i = 0; i < neighbors.size(); i++)
        {
            int nb_x = neighbors[i][0], nb_y = neighbors[i][1];
            int nb_idx;
            if (vertical)
                nb_idx = xy_to_i_upward(nb_x, nb_y, overlap, cost.height());
            else if (horizontal)
                nb_idx = xy_to_i_leftward(nb_x, nb_y, cost.width(), overlap);

            // compute tentative cumulative cost to reach neighbor
            float cumulative_cost;

            if (unvisited.size() == width*height) { // if we're at the starting point
                cumulative_cost = error(nb_x, nb_y) + 0;
            }
            else {
                cumulative_cost = error(nb_x, nb_y) + cost(curr_x, curr_y);
            }

            // IF THE COMPUTED CUMULATIVE COST IS LESS THAN THE EXISITNG CUMULATIVE COST, UPDATE IT
            if (cumulative_cost < cost(nb_x, nb_y))
            {
                cost(nb_x, nb_y) = cumulative_cost;
                (logim(cost)).write(DATA_DIR "/output/cost.png");
                //__libcpp_thread_sleep_for(std::chrono::nanoseconds(10000000)); // visualize cost computation

                // ALSO UPDATE THIS NEIGHBOR'S PREV PIXEL TO BE THE CURRENT PIXEL TO TRACE BACK
                prev[nb_idx] = curr_idx;
            }
        }
        // current pixel has been visited, never visit again
        unvisited.pop_back();

        // if we've just visited a pixel at the end of the path, we're done
        if ( (vertical && curr_y == 0) || (horizontal && curr_x == 0) ) {
            return prev;
        }
    }
    return prev;
}

// Converts xy-coordinates to an upward row-major indexing scheme for given grid dimensions
int xy_to_i_upward(int x, int y, int width, int height)
{return (height - 1 - y) * (width) + x;}

// Converts xy-coordinates to an leftward column-major indexing scheme for given grid dimensions
int xy_to_i_leftward(int x, int y, int width, int height)
{return (width - 1 - x) * (height) + y;}

// Converts index i to xy-coordinates for upward row-major scheme
vector<int> i_to_xy_upward(int i, int width, int height)
{return {i % width, height - 1 - (i / width)};}

// Converts index i to xy-coordinates for leftward row-major scheme
vector<int> i_to_xy_leftward(int i, int width, int height)
{return {width - 1 - (i / height), i % height};}

// Convert image to log domain for better visualization of cost computation
FloatImage logim(const FloatImage &im)
{
    FloatImage output(im);
    for (int i = 0; i < im.size(); i++) {
        output(i) = log10(output(i));
    }
    return output;
}

// Check quilting arguments for validity
void check_args(const FloatImage &sample, int patch_size)
{
    // Make sure patch_size is smaller than sample dimensions
    if (patch_size > sample.width() || patch_size > sample.height())
    {
        cout << "Error: Patch size must be smaller the sample image dimensions." << endl;
        exit(1);
    }
}
=======
FloatImage min_boundary(FloatImage error, int overlap, bool left, bool top)
{
    FloatImage cost_image(error);
    // FloatImage path_image(error);
    FloatImage path_image(error.width(), error.height(), error.channels()); // will be image that is returned
    FloatImage best_path(error.width(), error.height(), error.channels());  // will be image that is returned
    for (int x = 0; x < path_image.width(); x++)
    { //initalize all pixels in path_image to 0
        for (int y = 0; y < path_image.height(); y++)
        {
            path_image(x, y, 0) = 0;
        }
    }
    for (int x = 0; x < best_path.width(); x++)
    { //initalize all pixels in best_path to 1
        for (int y = 0; y < best_path.height(); y++)
        {
            best_path(x, y, 0) = 1;
        }
    }
    //error patch used to create cost image
    for (int y = 0; y < error.height(); y++)
    { // calculating cost_image vertical
        for (int x = 1; x < overlap; x++)
        {
            path_image(x, y, 0) = min(min(cost_image(x - 1, y + 1, 0), cost_image(x, y + 1, 0)), cost_image(x + 1, y + 1, 0));
            cost_image(x, y, 0) = error(x, y, 0) + path_image(x, y - 1, 0);
        }
    }
    for (int x = error.width(); x > 0; x--)
    { // calculating cost_image horizontal
        for (int y = 1; y < overlap; y++)
        {
            path_image(x, y, 0) = min(min(cost_image(x - 1, y - 1, 0), cost_image(x - 1, y, 0)), cost_image(x - 1, y + 1, 0));
            cost_image(x, y, 0) = error(x, y, 0) + path_image(x - 1, y, 0);
        }
    }
    //iterate through path image
    if (!left)
    {
        int path_idx_vertical = 2;
        for (int y = error.height(); y > 0; y--)
        {
            int min_idx = path_idx_vertical;
            for (int x = path_idx_vertical - 1; x <= min(path_idx_vertical + 1, overlap - 2); x++)
            {
                if (path_image(x, y, 0) < path_image(min_idx, y, 0))
                {
                    min_idx = max(2, x); //find min idx which indicates pixel in min cut
                }
            }
            path_idx_vertical = min_idx;
            //populate best image
            for (int x = 0; x < overlap - 1; x++)
            {
                if (x <= path_idx_vertical)
                {
                    best_path(x, y, 0) = 0; //set pixels before min idx to 0
                }
            }
        }
    }
    if (!top)
    {
        int path_idx_horizontal = 2;
        for (int x = error.width(); x > 0; x--)
        {
            int min_idx = path_idx_horizontal;
            for (int y = path_idx_horizontal - 1; y <= min(path_idx_horizontal + 1, overlap - 2); y++)
            // for (int y = 1; y < overlap - 1; y++)
            {
                if (path_image(x, y, 0) < path_image(x, min_idx, 0))
                {
                    min_idx = max(2, y);
                }
            }
            path_idx_horizontal = min_idx;
            //populate best image
            for (int y = 0; y < overlap - 1; y++)
            {
                if (y <= path_idx_horizontal)
                {
                    best_path(x, y, 0) = 0;
                }
            }
        }
    }
    // return path_image;
    // return cost_image;

    return best_path;
}
>>>>>>> 9393bb5c6e4bfa5b9b208cf07da2aaeae0bc1f63
