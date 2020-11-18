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

// tol = 10 %, threshold of best results to pick from
// FloatImage quilt_simple(sample, out_size, patch_size, overlap, tol)
// create vector V of vectors X,Y Pairs
// loop from 0 to sample.size
// pick a unique random x and random y
// add random x and y to V
// create float image output
// insert random pair from V and insert corresponding patch into top left of output

// start comparing SSDs to construct output image

// helper function to get a random sample of 10% of all possible patches, stored as x,y coordinates of the patch's origin
vector <vector<int>> find_patches(const FloatImage &sample, int patch_size) {
    
    vector <vector<int>> all_coords; // stores all possible x,y coordinates possible
    vector <vector<int>> out_coords;   // stores only the output x,y coordinates which are randomly selected

	// number of patches/pairs that we need to find (10% of all possible)
    int num_patches = (sample.width() - patch_size) * (sample.height() - patch_size) * 0.10;  
    
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
    }
    
    return out_coords;
}

// struct pairs
// {
//     vector<float> values;
//     vector<int> coords;
// };
            
// Perform simple quilting by overlapping patches to minimize SSD in the overlapping regions
FloatImage quilt_simple(const FloatImage &sample, int out_size, int patch_size, int overlap, int tol) {
    
    FloatImage output(out_size, out_size, sample.channels()); // initialize output image
    
    srand(time(NULL));

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
    
    // Loop over output image in increments of (patch size - overlap) (making sure patches don't go out of bounds)
    for (int out_x = patch_size-overlap; out_x < output.width() - patch_size; out_x += patch_size) {
        for (int out_y = patch_size-overlap; out_y < output.height() - patch_size; out_y += patch_size) {

            // use find_patches helper function to get a randomized list (x,y) patch coordinates
            vector<vector<int>> patches = find_patches(sample, patch_size);
            
            // Vector that holds SSD values of all selected patches
            vector<float> SSD_values;

			//pairs patch_SSD_values; // struct that maps patch coordinates to their corresponding SSD values

            // Loop over all selected patches
            for (int i = 0; i < patches.size(); i++){
            
                // initialize SSD of the current patch to store running sum of its SSD
                float patch_SSD = 0;

                // If we're not bordering the image's left-edge then loop over the overlap of patch-left with output-right
                if (out_x > 0) {
                    // for x1 from 0 to overlap
                    for (int x1=0; x1<overlap; x1++){
                        // for y1 from 0 to patchsize
                        for (int y1=0; y1<patch_size; y1++){
                            // for each channel
                            for (int c=0; c<sample.channels(); c++){
                                patch_SSD += pow( sample(x1+patches[i][0], y1+patches[i][1], c) - output(out_x+x1, out_y+y1, c), 2);
                            }
                        }
                    }
                }

                // If we're not bordering the image's top-edge, then loop over the overlap of patch-top with out-bottom
                if (out_y > 0){        
                    // for x1 from 0 to patchsize
                    for (int x1=0; x1<patch_size; x1++){
                        // for y1 from 0 to overlap
                        for (int y1; y1<overlap; y1++){
                            // for each channel
                            for (int c=0; c<sample.channels(); c++){
                                patch_SSD += pow( sample(x1+patches[i][0], y1+patches[i][1], c) - output(out_x+x1, out_y+y1, c), 2);
                            }
                        }
                    }
                }
                
                // Record SSD for this patch
                SSD_values[i] = patch_SSD;
				// patch_SSD_values.coords = patches[i];
				// patch_SSD_values.value = patch_SSD;
				
            }
            
            // note: link each SSD value to the corresponding patch?? (done with patch_values)
            // find the lowest 10% values in SSD_values
            // randomly select one of those values, store the corresponding (x, y) in variables best_pat_x, best_pat_y
            
			// Sort the SSD_values and patch vectors simultaneously
			int min = SSD_values[0];
			int min_idx = 0;
			for (int i = 0; i < SSD_values.size(); i++) {
				if (SSD_values[i] < min) {
					int swap = SSD_values[min_idx];
					SSD_values[min_idx] = SSD_values[i];
					SSD_values[i] = swap;
					
					vector<int> temp = patches[min_idx];
					patches[min_idx] = patches[i];
					patches[i] = temp;

					min_idx = i;
				}
			}

			// IDEA FOR OPTIMIZATION: take global min of SSD and put it into a new vector, then set it in the original vector to be a large number, then grab global min again, repeat
			vector<vector<int>> best_pairs; 

			for (int i = 0; i < SSD_values.size() * tol; i++) {
				float min_idx = 0;
				for (int j = 0; j < SSD_values.size(); j++) {
					if (SSD_values[j] >= 0 && SSD_values[j] < SSD_values[min_idx]) {
						min_idx = j;
					}
				}
				best_pairs.push_back(patches[min_idx]);
				SSD_values[min_idx] = -1; // remove it from consideration
			}


			// randomly pick a best patch with lowest SSD limited by tolerance
			int rand_idx = rand() % (SSD_values.size() * tol);

            // for pixels from out_x to out_x + patchsize
            for (int x = 0; x < patch_size; x++){
                // for pixels from out_y to out_y + patchsize
                for (int y = 0; y < patch_size; y++){
					for (int c = 0; c < output.channels(); c++) {
                     	// output(out_x + x, out_y + y, c) = sample(best_pat_x + x, best_pat_y + x, c)
                     	output(out_x + x, out_y + y, c) = sample(patches[rand_idx][0] + x, patches[rand_idx][1] + x, c);
					}
                }
            }
        }
    }
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
