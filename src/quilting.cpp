#include "quilting.h"
#include <math.h>
#include "utils.h"
#include <assert.h>
#include <iostream>
#include <cstdlib>

using namespace std;

// random placement of blocks
FloatImage quilt_random(const FloatImage &sample, int outsize, int patchsize) {

    // Make sure patchsize is smaller than sample dimensions
    if (patchsize > sample.width() || patchsize > sample.height()) {
      cout << "Error: Patch size must be smaller the sample image dimensions." << endl;
      exit (1);
    }

    FloatImage output(outsize, outsize, sample.channels()); // initialize output image
    srand(time(NULL)); // seed the random number generator

    // loop over output image by intervals of the patch size
    for (int x = 0; x <= output.width() - patchsize; x += patchsize) {
        for (int y = 0; y <= output.height() - patchsize; y += patchsize) {

            // randomly pick a patch from sample image by defining its origin (top-left) pixel (making sure the patch won't go out of bands)
            int patch_x0 = rand() % (sample.width() - patchsize + 1);
            int patch_y0 = rand() % (sample.height() - patchsize + 1);

            // loop over the patch area in the output image and populate it with the corresponding pixels of the randomly sampled patch
            for (int x1 = x; x1 < x + patchsize; x1++) {
                for (int y1 = y; y1 < y + patchsize; y1++) {
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

// helper fucntion to find a random sample of 10% of all possible patches, stored as x,y coordinates of the origin
vector <vector<int>> find_patches(const FloatImage &sample, int patch_size, int tol) {
    
    vector <vector<int>> all_pairs; // stores all possible x,y pairs possible
    vector <vector<int>>> out_pairs;   // stores only the output x,y pairs which are randomly selected
    int num_patches = floor(sample.height*sample.width)*0.10);  // int that stores the number of patches/pairs that we need to find
    
    // loop over every x,y coordinate and save the pair as a vector in all_pairs
    for (int x=0; x<sample.width()-patch_size; x++){
        for (int y=0; y<sample.height()-patch.size; y++){
            vector<int> v {x, y};
            all_pairs.push_back(v);
        }
    }
    
    // randomly shuffle the order of all possible pairs of x,y coordinates
    // not sure if this is the appropriate command, we may need to write a helper function for random shuffling instead
    std::random_shuffle(all_pairs);
    
    // select the first "num_patches number" of x,y pairs to be saved to out_pairs
    for (int i=0; i<num_patches; i++){
        out_pairs.push_back(all_pairs[i]);
    }
    
    return out_pairs;
}
            
            

FloatImage quilt_simple(const FloatImage &sample, int out_size, int patch_size, int overlap, int tol=0.10) {
    
    FloatImage output(outsize, outsize, sample.channels()); // initialize output image
    
    srand(time(NULL));
    
    // for all pixels out_x in increments of (patch size - overlap) until out.width() - patch_size
    for (int out_x = patch_size-overlap; out_x < output.width(); out_x += patch_size) {

        // for all pixels out_y in increments of (patch size - overlap) until out.height() - patch_size
        for (int out_y = patch_size-overlap; out_y < output.height(); out_y += patch_size) {

            // use find_patches helper function to find x,y coordinates of patches
            vector<vector<int>> pairs = find_patches(sample, patch_size, tol);
            
            // create vector<float> called SSD_values that holds SSD values of all patches
            vector<float> SSD_values;

            // for all (pat.x,pat.y) pairs in our vector of patches
            for (int i = 0; i < pairs.size(); i++){
            
                // initialize float patch_SSD to store running sum of SSD for this patch
                float patch_SSD = 0;

                    // if out_x > 0 then loop over the overlap of patch left with out right
                if (out_x > 0) {
                    // for x1 from 0 to overlap
                    for (int x1=0; x1<overlap; x++){
                        // for y1 from 0 to patchsize
                        for (int y1=0; y1<patch_size; y++){
                            // for each channel
                            for (int c=0; c<sample.channels(); c++){
                                // patch_SSD += pow( sample(x1+pat.x, y1+pat.y, c) - output(out_x+x, out_y+y, c), 2)
                                patch_SSD += pow( sample(x1+pairs[i][0], y1+pairs[i][1], c) - output(out_x+x, out_y+y, c), 2);
                            }
                        }
                    }
                }

                // if out_y > 0 then loop over the overlap of patch top with out bottom
                if (out_y > 0){        
                    // for x1 from 0 to patchsize
                    for (int x1=0; x1<patch_size; x1++){
                        // for y1 from 0 to overlap
                        for (int y1; y1<overlap; y1++){
                            // for each channel
                            for (int c=0; c<sample.channles(); c++){
                                // patch_SSD += pow( sample(x1+pat.x, y1+pat.y, c) - output(out_x+x, out_y+y, c), 2)
                                patch_SSD += pow( sample(x1+pairs[i][0], y1+pairs[i][1], c) - output(out_x+x, out_y+y, c), 2);
                            }
                        }
                    }
                }
                
                // put patch_SSD into  SSD_values
                SSD_values[i] = patch_SSD;
            }
            
            // note: link each SSD value to the corresponding patch??
            // find the lowest 10% values in SSD_values
            // randomly select one of those values, store the corresponding (x, y) in variables best_pat_x, best_pat_y
            
                
            // for pixels from out_x to out_x + patchsize
            for (int x = 0; x < patch_size; x++){
                // for pixels from out_y to out_y + patchsize
                for (int y = 0; y < patch_size: y++){
                     // output(out_x + x, out_y + y, c) = sample(best_pat_x + x, best_pat_y + x, c)
                     output(out_x + x, out_y + y, c) = sample(best_pat_x + x, best_pat_y + x, c);
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
