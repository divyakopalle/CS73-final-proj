#include "quilting.h"
#include "utils.h"
#include "transfer.h"

using namespace std;

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
    // FloatImage mat_simple = quilt_simple(mat_sample, 500, 100, 1, 0.05);
    // FloatImage planks_simple = quilt_simple(planks_sample, 500, 100, 5, 0.0001);
    // FloatImage apples_simple = quilt_simple(apples_sample, 500, 75, 5, 0.001);
    // FloatImage toast_simple = quilt_simple(toast_sample, 500, 100, 5, 0.01);
    // FloatImage green_simple = quilt_simple(green_sample, 300, 50, 1, 0.01);
    // FloatImage rice_simple = quilt_simple(rice_sample, 300, 60, 2, 0.001);

    stones_simple.write(DATA_DIR "/output/stones_simple.png");
    // mat_simple.write(DATA_DIR "/output/mat_simple.png");
    // planks_simple.write(DATA_DIR "/output/planks_simple.png");
    // apples_simple.write(DATA_DIR "/output/apples_simple.png");
    // toast_simple.write(DATA_DIR "/output/toast_simple.png");
    // green_simple.write(DATA_DIR "/output/green_simple.png");
    // rice_simple.write(DATA_DIR "/output/rice_simple.png");
}

void testQuiltCut()
{
    FloatImage green_sample(DATA_DIR "/input/green.png");
    FloatImage apples_sample(DATA_DIR "/input/apples.png");
    FloatImage rice_sample(DATA_DIR "/input/rice.png");
    FloatImage planks_sample(DATA_DIR "/input/planks.png");
    // FloatImage type_sample(DATA_DIR "/input/type.png");

    FloatImage green_cut = quilt_cut(green_sample, 300, 60, 10, 0.001);
    // FloatImage apples_cut = quilt_cut(apples_sample, 300, 75, 5, .001);
    // FloatImage rice_cut = quilt_cut(rice_sample, 400, 60, 10, 0.001);
    //FloatImage planks_cut = quilt_cut(planks_sample, 400, 60, 10, 0.001);

    green_cut.write(DATA_DIR "/output/green_cut.png");
    // apples_cut.write(DATA_DIR "/output/apples_cut.png");
    // rice_cut.write(DATA_DIR "/output/rice_cut.png");
    //planks_cut.write(DATA_DIR "/output/planks_cut.png");

    // type_cut.write(DATA_DIR "/output/type_cut.png");
}

void help()
{
    FloatImage test_error = FloatImage(20, 50, 1);
    for (int i = 0; i < test_error.size(); i++) {
        test_error(i) = 0;
    }
    for (int y = 0; y < test_error.height(); y++) {
        test_error(5, y) = .5;
    }
    test_error.write(DATA_DIR "/output/test_error.png");

    // FloatImage cost(error.width(), error.height(), error.channels());
    // for (int i = 0; i < cost.size(); i++)
    // {
    //     cost(i) = INFINITY; // initialize to infinty to set it up for Dikstra's alg
    // }

    //find_paths(error, cost, 20, 50, 20);
    FloatImage test_mask = min_boundary(test_error, 20, 3);

    test_mask.write(DATA_DIR "/output/test_mask.png");
    //FloatImage error_cut = quilt_cut(error, 50, 26, 10, 0.00001);
    //error_cut.write(DATA_DIR "/output/test_cut.png");

    
}

void testTransfer()
{
    // FloatImage green_sample(DATA_DIR "/input/green.png");
    // FloatImage apples_sample(DATA_DIR "/input/apples.png");
    FloatImage rice_sample(DATA_DIR "/input/rice.png");
    FloatImage face_sample(DATA_DIR "/input/face.png");

    // FloatImage green_cut =quilt(green_sample, 300, 50, 3, 0.001) ;
    // FloatImage apples_cut = quilt_cut(apples_sample, 500, 75, 5, .001);
    FloatImage rice_transfer = transfer(rice_sample, face_sample, 500, 40, 6, .001, .2f);

    // green_cut.write(DATA_DIR "/output/green_cut.png");
    // apples_cut.write(DATA_DIR "/output/apples_cut.png");
    rice_transfer.write(DATA_DIR "/output/rice_transfer.png");
}

int main()
{
    cout << "compiled" << endl;
    // testRandomQuilting();
    // testSimpleQuilting();
    testQuiltCut();
    // testTransfer();
    //help();
}
