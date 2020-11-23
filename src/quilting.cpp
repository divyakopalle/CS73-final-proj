#include "quilting.h"
#include <math.h>
#include "utils.h"
#include <assert.h>
#include <iostream>
#include <cstdlib>

using namespace std;

// random placement of blocks
FloatImage quilt_random(const FloatImage &sample, int outsize, int patch_size) {

    // Make sure patch_size is smaller than sample dimensions
    if (patch_size > sample.width() || patch_size > sample.height()) {
      cout << "Error: Patch size must be smaller the sample image dimensions." << endl;
      exit (1);
    }

    FloatImage output(outsize, outsize, sample.channels()); // initialize output image
    srand(time(NULL)); // seed the random number generator

    // loop over output image by intervals of the patch size
    for (int x = 0; x <= output.width() - patch_size; x += patch_size) {
        for (int y = 0; y <= output.height() - patch_size; y += patch_size) {

            // randomly pick a patch from sample image by defining its origin (top-left) pixel (making sure the patch won't go out of bands)
            int patch_x0 = rand() % (sample.width() - patch_size + 1);
            int patch_y0 = rand() % (sample.height() - patch_size + 1);

            // loop over the patch area in the output image and populate it with the corresponding pixels of the randomly sampled patch
            for (int x1 = x; x1 < x + patch_size; x1++) {
                for (int y1 = y; y1 < y + patch_size; y1++) {
                    for (int c = 0; c < output.channels(); c++) {
                      output(x1, y1, c) = sample(patch_x0 + x1 - x, patch_y0 + y1 - y, c);
                    }
                }
            }
        }
    }
    return output;
}

// simple quilting- overlapping method


// helper function to get a random sample of 10% of all possible patches, stored as x,y coordinates of the patch's origin
vector <vector<int>> find_patches(const FloatImage &sample, int patch_size) {
    
    vector <vector<int>> all_coords; // stores all possible x,y coordinates possible
    vector <vector<int>> out_coords;   // stores only the output x,y coordinates which are randomly selected

	// number of patches/pairs that we need to find (10% of all possible)
    int num_patches = (sample.width() - patch_size) * (sample.height() - patch_size) * 0.1f;  
    
    // Save every possible coordinate as a vector in all_pairs
    for (int x = 0; x < sample.width() - patch_size; x++){
        for (int y = 0; y < sample.height() - patch_size; y++){
            vector<int> v {x, y};
            all_coords.push_back(v);
        }
    }

    // randomly shuffle the order of all possible pairs of x,y coordinates
    // not sure if this is the appropriate command, we may need to write a helper function for random shuffling instead
    random_shuffle(all_coords.begin(), all_coords.end());
    
    // select the first "num_patches number" of x,y pairs to be saved to out_pairs
    for (int i=0; i<num_patches; i++){
        out_coords.push_back(all_coords[i]);
       // cout << out_coords[i][0] << "," << out_coords[i][1] << endl;
    }
    
    return out_coords;
}


            
// Perform simple quilting by overlapping patches to minimize SSD in the overlapping regions
FloatImage quilt_simple(const FloatImage &sample, int out_size, int patch_size, int overlap, float tol) {
    
    FloatImage output(out_size, out_size, sample.channels()); // initialize output image
    
    srand(time(NULL));

    // Make sure patch_size is smaller than sample dimensions
    if (patch_size > sample.width() || patch_size > sample.height()) {
      cout << "Error: Patch size must be smaller the sample image dimensions." << endl;
      exit (1);
    }

	// Randomly pick an initial patch from sample image and place it in top-left corner of output image
	int first_patch_x = rand() % (sample.width() - patch_size + 1);
    int first_patch_y = rand() % (sample.height() - patch_size + 1);

	for (int x = 0; x < patch_size; x++) {
        for (int y = 0; y < patch_size; y++) {
            for (int c = 0; c < output.channels(); c++) {
                output(x, y, c) = sample(first_patch_x + x, first_patch_y + y, c);
			}
		}
	}

    // use find_patches helper function to get a randomized list (x,y) patch coordinates
    vector<vector<int>> patches = find_patches(sample, patch_size);
 
    bool debug = true;
    // Loop over output image in increments of (patch size - overlap) (making sure patches don't go out of bounds)
    for (int out_x = 0; out_x < output.width()-patch_size; out_x += patch_size - overlap) {
       for (int out_y = 0; out_y < output.height()-patch_size; out_y += patch_size - overlap) {
            cout << "Current position in output image is " << "(" << out_x << ", " << out_y << ")" << endl;

            // Vector that holds SSD values of all selected patches
            vector<float> SSD_values;

            // Loop over all selected patches
            for (int i = 0; i < patches.size(); i++){
                int patch_x = patches[i][0];
                int patch_y = patches[i][1];
            
                // initialize SSD of the current patch to store running sum of its SSD
                float patch_SSD = 0.f;

                // If we're not bordering the image's left-edge then loop over the overlap of patch-left with output-right
                if (out_x > 0) {
                    // for x1 from 0 to overlap
                    for (int x1=0; x1<overlap; x1++){
                        // for y1 from 0 to patchsize
                        for (int y1=0; y1<patch_size; y1++){
                            // for each channel
                            for (int c=0; c<sample.channels(); c++){
                                patch_SSD += pow( sample(patch_x+x1, patch_y+y1, c) - output(out_x-overlap+x1, out_y+y1, c), 2);
                            }
                        }
                    }
                }

                // If we're not bordering the image's top-edge, then loop over the overlap of patch-top with out-bottom
                if (out_y > 0){        
                    // for x1 from 0 to patchsize
                    for (int x1=0; x1<patch_size; x1++){
                        // for y1 from 0 to overlap
                        for (int y1=0; y1<overlap; y1++){
                            // for each channel
                            for (int c=0; c<sample.channels(); c++){
                                patch_SSD += pow( sample(patch_x+x1, patch_y+y1, c) - output(out_x+x1, out_y-overlap+y1, c), 2);
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

            for (int i = 0; i < SSD_values.size() * tol; i++) {
                int min_idx = 0;
                //cout << "START" << endl;
                for (int j = 0; j < SSD_values.size(); j++) {
                    if (SSD_values[j] < SSD_values[min_idx]) {
                        min_idx = j;
                        //cout << "updated global min=" << SSD_values[min_idx] << endl;
                    }
                }
                best_patches.push_back(patches[min_idx]);
                //cout << "Considering patch with SSD " << SSD_values[min_idx] << endl;
                SSD_values[min_idx] = 1e7; // remove it from consideration
            }

            // randomly pick the index of ONE of the best patches 
            int rand_idx = rand() % (int)(best_patches.size());
            
            // Write this patch to the output image
            if (debug) {
            if (out_x > 0 || out_y > 0) {
                for (int x = 0; x < patch_size; x++){
                    for (int y = 0; y < patch_size; y++){
                        for (int c = 0; c < output.channels(); c++) {
                            output(out_x + x, out_y + y, c) = sample(best_patches[rand_idx][0] + x, best_patches[rand_idx][1] + y, c);
                            //debug = false;
                        }
                    }
                }
            }
            }
        }
    }
    return output;
}
  
  


// compute SSD (intensity of a - intensity of b)squared

//neighboring blocks constrained by overlap

//mininmum error boundary cut

// Dijkstra's

//Dyanmic Programming
// want to make cut between overlapping blocks on pixels where the two textures match best
// Allocate space to store error surfaces F[1:n,1:n]
//traverse over all Blocks B2-BN (i=2...N) (why does paper say 2..N?)
//for(int i=0;i<=n;i++){
// for(int j=0;j<=n;j++){
//if B1 and B2 are two blocks that overlap along their vertical edge
// error_ij=(Biov-Bjov)^2
// F[i][j]= error_ij+min(F[i-1][j-1],F[i-1][j],F[i-1][j+1])

// }
// }
//minimum value of the last rown in F will indicate the end of the minimal vertical path through the surface
//trace back to find the path of best cut
//int j=n int i=1
// vector
FloatImage quilt_cut(const FloatImage &sample, int out_size, int patch_size, int overlap, float tol) {
    
    FloatImage output(out_size, out_size, sample.channels()); // initialize output image
    
    srand(time(NULL));

    // Make sure patch_size is smaller than sample dimensions
    if (patch_size > sample.width() || patch_size > sample.height()) {
      cout << "Error: Patch size must be smaller the sample image dimensions." << endl;
      exit (1);
    }

	// Randomly pick an initial patch from sample image and place it in top-left corner of output image
	int first_patch_x = rand() % (sample.width() - patch_size + 1);
    int first_patch_y = rand() % (sample.height() - patch_size + 1);

	for (int x = 0; x < patch_size; x++) {
        for (int y = 0; y < patch_size; y++) {
            for (int c = 0; c < output.channels(); c++) {
                output(x, y, c) = sample(first_patch_x + x, first_patch_y + y, c);
			}
		}
	}

    // use find_patches helper function to get a randomized list (x,y) patch coordinates
    vector<vector<int>> patches = find_patches(sample, patch_size);
 
    bool debug = true;
    // Loop over output image in increments of (patch size - overlap) (making sure patches don't go out of bounds)
    for (int out_x = 0; out_x < output.width()-patch_size; out_x += patch_size - overlap) {
       for (int out_y = 0; out_y < output.height()-patch_size; out_y += patch_size - overlap) {
            cout << "Current position in output image is " << "(" << out_x << ", " << out_y << ")" << endl;

            // Vector that holds SSD values of all selected patches
            vector<float> SSD_values;

            // Loop over all selected patches
            for (int i = 0; i < patches.size(); i++){
                int patch_x = patches[i][0];
                int patch_y = patches[i][1];
            
                // initialize SSD of the current patch to store running sum of its SSD
                float patch_SSD = 0.f;

                // If we're not bordering the image's left-edge then loop over the overlap of patch-left with output-right
                if (out_x > 0) {
                    // for x1 from 0 to overlap
                    for (int x1=0; x1<overlap; x1++){
                        // for y1 from 0 to patchsize
                        for (int y1=0; y1<patch_size; y1++){
                            // for each channel
                            for (int c=0; c<sample.channels(); c++){
                                patch_SSD += pow( sample(patch_x+x1, patch_y+y1, c) - output(out_x-overlap+x1, out_y+y1, c), 2);
                            }
                        }
                    }
                }

                // If we're not bordering the image's top-edge, then loop over the overlap of patch-top with out-bottom
                if (out_y > 0){        
                    // for x1 from 0 to patchsize
                    for (int x1=0; x1<patch_size; x1++){
                        // for y1 from 0 to overlap
                        for (int y1=0; y1<overlap; y1++){
                            // for each channel
                            for (int c=0; c<sample.channels(); c++){
                                patch_SSD += pow( sample(patch_x+x1, patch_y+y1, c) - output(out_x+x1, out_y-overlap+y1, c), 2);
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

            for (int i = 0; i < SSD_values.size() * tol; i++) {
                int min_idx = 0;
                //cout << "START" << endl;
                for (int j = 0; j < SSD_values.size(); j++) {
                    if (SSD_values[j] < SSD_values[min_idx]) {
                        min_idx = j;
                        //cout << "updated global min=" << SSD_values[min_idx] << endl;
                    }
                }
                best_patches.push_back(patches[min_idx]);
                //cout << "Considering patch with SSD " << SSD_values[min_idx] << endl;
                SSD_values[min_idx] = 1e7; // remove it from consideration
            }

            // randomly pick the index of ONE of the best patches 
            int rand_idx = rand() % (int)(best_patches.size());
             // CHANGES WITH DYNAMIC PROGRAMMING //

             //save error patch 
                //patch- existing outputimage
                //error patch stores SSD Value at each pixel
                FloatImage error(patch_size , patch_size, 1); 
                for (int i=0; i<error.size(); i++){
                    error(i)=0;
                }
                if (out_x > 0) {
                    // for x1 from 0 to overlap
                    for (int x1=0; x1<overlap; x1++){
                        // for y1 from 0 to patchsize
                        for (int y1=0; y1<patch_size; y1++){
                            // for each channel
                            for (int c=0; c<sample.channels(); c++){
                                error(x1, y1, 0)+=pow( sample(best_patches[rand_idx][0] +x1, best_patches[rand_idx][1] +y1, c) - output(out_x-overlap+x1, out_y+y1, c), 2);
                            }
                        }
                    }
                } 
                 if (out_y > 0){        
                    // for x1 from 0 to patchsize
                    for (int x1=0; x1<patch_size; x1++){
                        // for y1 from 0 to overlap
                        for (int y1=0; y1<overlap; y1++){
                            // for each channel
                            for (int c=0; c<sample.channels(); c++){ 
                                error(x1, y1, 0)+= pow( sample(best_patches[rand_idx][0] +x1, best_patches[rand_idx][1] +y1, c) - output(out_x-overlap+x1, out_y+y1, c), 2);
                            }
                        }
                    }
                } 

            // error patches stored in FloatImage error, error patch stores the SSD values of each pixel in the overlap region
            // use error patches to find min boundary cut  
            // min_boundary(error)
                //FloatImage cost_image = error;
                //FloatImage path_image= error;
                //FloatImage best_image= error;
                //error patch used to create cost image
                //for (int x=0; x<overlap; x++){
                    //for(int y=0; y<patch_size;y++){
                        //path(x,y,0)=min(cost_image(x-1,y-1,0),cost_image(x-1,y,0),cost_image(x-1,y+1,0);
                        // cost_image(x,y,0)= error(x,y,0)+path(x,y,0)
                        
                    //}
                //for (int x=0; x<patch_size; x++){
                    //for(int y=0l y<overlap;y++){
                        // cost_image(x,y,0)= error(x,y,0)+min(cost_image(x-1,y-1,0),cost_image(x-1,y,0),cost_image(x-1,y+1,0))
                        //cost_image(x,y,0)= error(x,y,0)+path(x,y,0)
                    //}
                    
               // }
                // cost patch used to create path image 
                // best path image is binary and what is returned
                //return path image                  
            // Write this patch to the output image
            if (debug) {
            if (out_x > 0 || out_y > 0) {
                for (int x = 0; x < patch_size; x++){
                    for (int y = 0; y < patch_size; y++){
                        for (int c = 0; c < output.channels(); c++) {
                            output(out_x + x, out_y + y, c) = sample(best_patches[rand_idx][0] + x, best_patches[rand_idx][1] + y, c);
                            //debug = false;
                        }
                    }
                }
            }
            }
        }
    }
    return output;
}
