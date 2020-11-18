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
    FloatImage apples_sample(DATA_DIR "/input/apples.png");
    FloatImage green_sample(DATA_DIR "/input/green.png");
    
    int outsize = 500;
    int patchsize = 50;

    FloatImage stones_random = quilt_random(stones_sample, outsize, patchsize);
    FloatImage mat_random = quilt_random(mat_sample, outsize, patchsize);
    // FloatImage text_random = quilt_random(text_sample, outsize, patchsize);
    FloatImage apples_random = quilt_random(apples_sample, outsize, patchsize);
    FloatImage green_random = quilt_random(green_sample, outsize, patchsize);

    // stones_random.write(DATA_DIR "/output/stones_random_synthesis.png");
    mat_random.write(DATA_DIR "/output/mat_random_synthesis.png");
    // text_random.write(DATA_DIR "/output/text_random_synthesis.png");
    apples_random.write(DATA_DIR "/output/apples_random_synthesis.png");
    green_random.write(DATA_DIR "/output/green_random_synthesis.png");
}

// //test texture synthesis with simple quilting (overlapping method)
void testSimpleQuilting()
{
    FloatImage stones_sample(DATA_DIR "/input/stones.png");
    FloatImage mat_sample(DATA_DIR "/input/mat.png");
    FloatImage text_sample(DATA_DIR "/input/text.png");
    FloatImage apples_sample(DATA_DIR "/input/apples.png");
    //FloatImage green_sample(DATA_DIR "/input/green.png");

    int outsize=500;
    int patchsize=50;
    float tol = .001;
    int overlap = 5;


    //FloatImage stones_simple = quilt_simple(stones_sample, outsize, patchsize, overlap, tol);
    FloatImage mat_simple = quilt_simple(mat_sample, outsize, patchsize, overlap, tol);
    //FloatImage text_simple = quilt_simple(text_sample, outsize, patchsize, overlap, tol);
    FloatImage apples_simple = quilt_simple(apples_sample, outsize, patchsize, overlap, tol);
    //FloatImage green_simple = quilt_simple(green_sample, outsize, patchsize, overlap, tol);

    //stones_simple.write(DATA_DIR "/output/stones_simple.png");
    mat_simple.write(DATA_DIR "/output/mat_simple.png");
    //text_simple.write(DATA_DIR "/output/text_simple.png");
    apples_simple.write(DATA_DIR "/output/apples_simple.png");
    //green_simple.write(DATA_DIR "/output/green_simple.png");
}

int main()
{
    cout << "compiled" << endl;
    testRandomQuilting();
    testSimpleQuilting();
    
}
