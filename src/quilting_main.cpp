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
void testSimpleQuilting()
{
    //FloatImage stones_sample(DATA_DIR "/input/stones.png");
    FloatImage mat_sample(DATA_DIR "/input/mat.png");
    //FloatImage text_sample(DATA_DIR "/input/text.png");

    int outsize=700;
    int patchsize=100;
    float tol = .05;
    int overlap = 20;


    //FloatImage stones_simple = quilt_simple(stones_sample, outsize, patchsize, overlap, tol);
    FloatImage mat_simple = quilt_simple(mat_sample, outsize, patchsize, overlap, tol);
    //FloatImage text_simple = quilt_simple(text_sample, outsize, patchsize, overlap, tol);

    //stones_simple.write(DATA_DIR "/output/stones_simple.png");
    mat_simple.write(DATA_DIR "/output/mat_simple.png");
}

int main()
{
    cout << "compiled" << endl;
    testRandomQuilting();
    testSimpleQuilting();
    
}
