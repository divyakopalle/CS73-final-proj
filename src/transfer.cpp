#include "quilting.h"
#include "a2.h"
#include <math.h>
#include "utils.h"
#include <assert.h>
#include <iostream>
#include <cstdlib>
#include <deque>

using namespace std;

// generate correspondence map

FloatImage map(FloatImage source){
    //transform source image
    vector<FloatImage> output= lumiChromi(source);

    return output[0];

}
    

//texutre transfer
FloatImage texture_transfer(const FloatImage &sample, const FloatImage &target, int out_size, int patch_size, int overlap, float tol, float alpha)
{

    FloatImage target_corr= map(target);
    FloatImage sample_corr= map(sample);
    FloatImage output(out_size, out_size, sample.channels()); // initialize output image

    srand(time(NULL));

    // Make sure patch_size is smaller than sample dimensions
    if (patch_size > sample.width() || patch_size > sample.height())
    {
        cout << "Error: Patch size must be smaller the sample image dimensions." << endl;
        exit(1);
    }

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
                            // CORRESPONDANCE ERROR //
                            patch_SSD += (1-alpha)* pow(sample_corr(patch_x + x1, patch_y + y1) - target_corr(out_x - overlap + x1, out_y + y1), 2);
                            for (int c = 0; c < sample.channels(); c++)
                            {
                                patch_SSD += alpha* pow(sample(patch_x + x1, patch_y + y1, c) - output(out_x - overlap + x1, out_y + y1, c), 2);
                                
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
                            // CORRESPONDANCE ERROR //
                            patch_SSD += (1-alpha)* pow(sample_corr(patch_x + x1, patch_y + y1) - target_corr(out_x - overlap + x1, out_y + y1), 2);                           // for each channel
                            for (int c = 0; c < sample.channels(); c++)
                            {
                                patch_SSD += alpha * pow(sample(patch_x + x1, patch_y + y1, c) - output(out_x + x1, out_y - overlap + y1, c), 2);
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
                    }
                }
                best_patches.push_back(patches[min_idx]);
                //cout << "Considering patch with SSD " << SSD_values[min_idx] << endl;
                SSD_values[min_idx] = 1e7; // remove it from consideration
            }

            // cout << "line 371" << endl;
            // randomly pick the index of ONE of the best patches
            int rand_idx = rand() % (int)(best_patches.size());
            int chosen_patch_x = best_patches[rand_idx][0];
            int chosen_patch_y = best_patches[rand_idx][1];



             // START OF MINIMUM-ERROR BOUNDARY CUT//
             //////////////////////////////////////
             // CHANGES FOR TEXTURE TRANSFER //

            // SAVE ERROR PATCH
            // stores SSD Value at each pixel in overlap region
            FloatImage error(patch_size, patch_size, 1); 
            for (int i=0; i<error.size(); i++){
                error(i)=0;
            } // initialize to all black pixels
            
            // Compute SSD at each pixel in overlap region
            if (out_x > 0) {
                for (int x1=0; x1<overlap; x1++){
                    for (int y1=0; y1<patch_size; y1++){
                        // CORRESPONDANCE ERROR //
                        error(x1,y1) += (1-alpha)* pow(sample_corr(chosen_patch_x + x1, chosen_patch_y + y1) - target_corr(out_x - overlap + x1, out_y + y1), 2);
                        for (int c=0; c<sample.channels(); c++){
                            error(x1, y1, 0)+=alpha * pow( sample(chosen_patch_x + x1, chosen_patch_y + y1, c) - output(out_x-overlap+x1, out_y+y1, c), 2);
                        }
                    }
                }
            } 
            if (out_y > 0){        
                for (int x1=0; x1<patch_size; x1++){
                    for (int y1=0; y1<overlap; y1++){
                        // CORRESPONDANCE ERROR //
                        error(x1,y1) += (1-alpha)* pow(sample_corr(chosen_patch_x + x1, chosen_patch_y + y1) - target_corr(out_x - overlap + x1, out_y + y1), 2);
                        for (int c=0; c<sample.channels(); c++){ 
                            error(x1, y1, 0)+= alpha * pow( sample(chosen_patch_x + x1, chosen_patch_y + y1, c) - output(out_x-overlap+x1, out_y+y1, c), 2);
                        }
                    }
                }
            } 

            // error patches stored in FloatImage error, error patch stores the SSD values of each pixel in the overlap region
            // Use error patch to find the min-error boundary cut (returned as a binary mask)
            FloatImage cut_mask; 
            int edge_case;
            if (out_x == 0) edge_case = 1;      // case where patch goes in first column
            else if (out_y == 0) edge_case = 2; // case where patch goes in first row
            else edge_case = 3;                 // case where patch borders other patches above and to the left
            cut_mask = min_boundary(error, overlap, edge_case);

            // Write this patch to the output image
            if (debug) {
            if (out_x > 0 || out_y > 0) {
            //if (out_x > 0)
                for (int x = 0; x < patch_size; x++){
                    for (int y = 0; y < patch_size; y++){
                        for (int c = 0; c < output.channels(); c++) {
                            if(cut_mask(x,y,0)==1){
                                output(out_x + x, out_y + y, c) = sample(chosen_patch_x + x, chosen_patch_y + y, c);
                            }
                            
                            //debug = false;
                        }
                    }
                }
            }
        }
    }
    cout << "output is " << output.size() << endl;
    return output;
}

