

// // random placement of blocks
// FloatImage quilt_random(FloatImage sample, int outsize, int patchsize) {
//     FloatImage output(outsize, outsize, sample.channels());
//     srand(time(NULL)); // seed the random number generator

//     // loop over output image by intervals of the patch size
//     for (int x = 0; x <= output.width() - patchsize; x += patchsize) {
//         for (int y = 0; y <= output.height() - patchsize; y += patchsize) {
//             for (int c = 0; c < output.channels(); c++) {

//                 // randomly pick a patch from sample image by defining its origin (top-left) pixel (making sure the patch won't go out of bands)
//                 int patch_x0 = rand() % (sample.width() - patchsize + 1);
//                 int patch_y0 = rand() % (sample.height() - patchsize + 1);

//                 // loop over the patch area in the output image and populate it with the corresponding pixels of the randomly sampled patch
//                 for (int x1 = x; x1 < x + patchsize; x1++) {
//                     for (int y1 = y; y1 < y + patchsize; y1++) {
//                         output(x1, y1, c) = sample(patch_x0 + x1 - x, patch_y0 + y1 - y, c);
//                     }
//                 }
//             }
//         }
//     }
//     return output;
// }

// simple quilting- overlapping method

// tol = 10 %, threshold of best results to pick from
// FloatImage quilt_simple(sample, out_size, patch_size, overlap, tol)
// create vector V of vectors X,Y Pairs
// loop from 0 to sample.size
// pick a unique random x and random y
// add random x and y to V
// create float image output
// insert random pair from V and insert corresponding patch into top left of output
// start comparison
// create vector V of floats that hold SSD values
// loop through overlap pixels
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
