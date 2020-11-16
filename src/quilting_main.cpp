#include "quilting.h"
#include "utils.h"
//#include <math.h>
using namespace std;

//test texture synthesis with random quilting
void testRandomQuilting()
{
    FloatImage stones_sample(DATA_DIR "/input/stones.png");
    FloatImage mat_sample(DATA_DIR "/input/mat.png");
    FloatImage text_sample(DATA_DIR "/input/text.png");
    
    int outsize = 700;
    int patchsize = 100;

    FloatImage stones_random = quilt_random(stones_sample, outsize, patchsize);
    FloatImage mat_random = quilt_random(mat_sample, outsize, patchsize);
    FloatImage text_random = quilt_random(text_sample, outsize, patchsize);

    stones_random.write(DATA_DIR "/output/stones_random_synthesis.png");
    mat_random.write(DATA_DIR "/output/mat_random_synthesis.png");
    text_random.write(DATA_DIR "/output/text_random_synthesis.png");
}

// //test texture synthesis with simple quilting (overlapping method)
// void testSimpleQuilting()
// {
//     FloatImage sample(DATA_DIR "/input/sample-texture.png");
//     int outsize=
//     int patchsize=
//     tol = 
//     overlap =
//     FloatImage random_output=  quilt_simple(sample, out_size, patch_size, overlap, tol)
//     random_output.write(DATA_DIR "/output/sample-random.png");
// }

int main()
{
    cout << "compiled" << endl;
    testRandomQuilting();
    // testSimpleQuilting();
    
}
