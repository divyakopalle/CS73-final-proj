#include "quilting.h"
#include "utils.h"
//#include <math.h>
using namespace std;

//test planksure synthesis with random quilting
void testRandomQuilting()
{
    FloatImage stones_sample(DATA_DIR "/input/stones.png");
    FloatImage mat_sample(DATA_DIR "/input/mat.png");
    FloatImage planks_sample(DATA_DIR "/input/planks.png");
    FloatImage apples_sample(DATA_DIR "/input/apples.png");
    FloatImage toast_sample(DATA_DIR "/input/toast.png");
    FloatImage green_sample(DATA_DIR "/input/green.png");
    FloatImage rice_sample(DATA_DIR "/input/rice.png");
    

    FloatImage stones_random = quilt_random(stones_sample, 500, 100);
    FloatImage mat_random = quilt_random(mat_sample, 500, 100);
    FloatImage planks_random = quilt_random(planks_sample, 500, 100);
    FloatImage apples_random = quilt_random(apples_sample, 500, 100);
    FloatImage toast_random = quilt_random(toast_sample, 500, 100);
    FloatImage green_random = quilt_random(green_sample, 300, 50);
    FloatImage rice_random = quilt_random(rice_sample, 300, 60);

    stones_random.write(DATA_DIR "/output/stones_random_synthesis.png");
    mat_random.write(DATA_DIR "/output/mat_random_synthesis.png");
    planks_random.write(DATA_DIR "/output/planks_random_synthesis.png");
    apples_random.write(DATA_DIR "/output/apples_random_synthesis.png");
    toast_random.write(DATA_DIR "/output/toast_random_synthesis.png");
    green_random.write(DATA_DIR "/output/green_random_synthesis.png");
    rice_random.write(DATA_DIR "/output/rice_random_synthesis.png");
}

// //test planksure synthesis with simple quilting (overlapping method)
void testSimpleQuilting()
{
    FloatImage stones_sample(DATA_DIR "/input/stones.png");
    FloatImage mat_sample(DATA_DIR "/input/mat.png");
    FloatImage planks_sample(DATA_DIR "/input/planks.png");
    FloatImage apples_sample(DATA_DIR "/input/apples.png");
    FloatImage toast_sample(DATA_DIR "/input/toast.png");
    FloatImage green_sample(DATA_DIR "/input/green.png");
    FloatImage rice_sample(DATA_DIR "/input/rice.png");

    // int outsize=500;
    // int patchsize=100;
    // float tol = .001;
    // int overlap = 20;


    FloatImage stones_simple = quilt_simple(stones_sample, 500, 100, 1, 0.2);
    FloatImage mat_simple = quilt_simple(mat_sample, 500, 100, 1, 0.05);
    FloatImage planks_simple = quilt_simple(planks_sample, 500, 100, 5, 0.0001);
    FloatImage apples_simple = quilt_simple(apples_sample, 500, 75, 5, 0.001);
    FloatImage toast_simple = quilt_simple(toast_sample, 500, 100, 5, 0.01);
    FloatImage green_simple = quilt_simple(green_sample, 300, 50, 1, 0.01);
    FloatImage rice_simple = quilt_simple(rice_sample, 300, 60, 2, 0.001);

    stones_simple.write(DATA_DIR "/output/stones_simple.png");
    mat_simple.write(DATA_DIR "/output/mat_simple.png");
    planks_simple.write(DATA_DIR "/output/planks_simple.png");
    apples_simple.write(DATA_DIR "/output/apples_simple.png");
    toast_simple.write(DATA_DIR "/output/toast_simple.png");
    green_simple.write(DATA_DIR "/output/green_simple.png");
    rice_simple.write(DATA_DIR "/output/rice_simple.png");
}

void testQuiltCut()
{
    FloatImage green_sample(DATA_DIR "/input/green.png");
    //FloatImage apples_sample(DATA_DIR "/input/apples.png");

    FloatImage green_cut = quilt_cut(green_sample, 300, 50, 10, 0.01);
    //FloatImage apples_cut = quilt_cut(apples_sample, 500, 75, 15, .001);

    green_cut.write(DATA_DIR "/output/green_cut.png");
    //apples_cut.write(DATA_DIR "/output/apples_cut_15.png");
}

int main()
{
    cout << "compiled" << endl;
    // testRandomQuilting();
    // testSimpleQuilting();
    testQuiltCut();
    
}
