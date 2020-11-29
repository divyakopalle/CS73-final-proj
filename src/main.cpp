#include "quilting.h"
#include "utils.h"
#include "transfer.h"

using namespace std;

// INITIALIZE INPUT IMAGES
FloatImage green_sample(DATA_DIR "/input/green.png");
FloatImage apples_sample(DATA_DIR "/input/apples.png");
FloatImage rice_sample(DATA_DIR "/input/rice.png");
FloatImage toast_sample(DATA_DIR "/input/toast.png");
FloatImage planks_sample(DATA_DIR "/input/planks.png");
FloatImage text_sample(DATA_DIR "/input/text.png");
FloatImage mat_sample(DATA_DIR "/input/mat.png");
FloatImage weave_sample(DATA_DIR "/input/weave.png");
FloatImage stones_sample(DATA_DIR "/input/stones.png");

FloatImage face_sample(DATA_DIR "/input/face.png");
FloatImage face_small(DATA_DIR "/input/face_small.png");
FloatImage rice_small(DATA_DIR "/input/rice_small.png");

void testRandomQuilting()
{
    // FloatImage stones_random = quilt_random(stones_sample, 500, 100);
    FloatImage mat_random = quilt_random(mat_sample, 600, 100);
    // FloatImage planks_random = quilt_random(planks_sample, 500, 100);
    // FloatImage apples_random = quilt_random(apples_sample, 500, 100);
    // FloatImage toast_random = quilt_random(toast_sample, 500, 100);
    // FloatImage green_random = quilt_random(green_sample, 300, 50);
    // FloatImage rice_random = quilt_random(rice_sample, 300, 60);

    // stones_random.write(DATA_DIR "/output/stones_random_synthesis.png");
    mat_random.write(DATA_DIR "/output/mat_random_synthesis.png");
    // planks_random.write(DATA_DIR "/output/planks_random_synthesis.png");
    // apples_random.write(DATA_DIR "/output/apples_random_synthesis.png");
    // toast_random.write(DATA_DIR "/output/toast_random_synthesis.png");
    // green_random.write(DATA_DIR "/output/green_random_synthesis.png");
    // rice_random.write(DATA_DIR "/output/rice_random_synthesis.png");
}

// //test texture synthesis with simple quilting (overlapping method)
void testSimpleQuilting()
{
    //FloatImage stones_simple = quilt_simple(stones_sample, 500, 100, 1, 0.2);
    FloatImage mat_simple = quilt_simple(mat_sample, 600, 100, 15, 0.01);
    // FloatImage planks_simple = quilt_simple(planks_sample, 500, 100, 5, 0.0001);
    // FloatImage apples_simple = quilt_simple(apples_sample, 500, 75, 5, 0.001);
    // FloatImage toast_simple = quilt_simple(toast_sample, 500, 100, 5, 0.01);
    // FloatImage green_simple = quilt_simple(green_sample, 300, 60, 10, 0.001);
    // FloatImage rice_simple = quilt_simple(rice_sample, 300, 60, 2, 0.001);

    //stones_simple.write(DATA_DIR "/output/stones_simple.png");
    mat_simple.write(DATA_DIR "/output/mat_simple.png");
    // planks_simple.write(DATA_DIR "/output/planks_simple.png");
    // apples_simple.write(DATA_DIR "/output/apples_simple.png");
    // toast_simple.write(DATA_DIR "/output/toast_simple.png");
    // green_simple.write(DATA_DIR "/output/green_simple.png");
    // rice_simple.write(DATA_DIR "/output/rice_simple.png");
}

void testQuiltCut()
{
    FloatImage green_cut = quilt_cut(green_sample, 300, 60, 10, 0.001);
    //FloatImage apples_cut = quilt_cut(apples_sample, 500, 75, 10, .001);
    //FloatImage rice_cut = quilt_cut(rice_sample, 300, 90, 15, 0.001);
    // FloatImage toast_cut = quilt_cut(toast_sample, 400, 100, 15, 0.01);
    // FloatImage planks_cut = quilt_cut(planks_sample, 300, 60, 10, 0.001);
    //FloatImage text_cut = quilt_cut(text_sample, 300, 60, 10, 0.0001);
    //FloatImage mat_cut = quilt_cut(mat_sample, 300, 150, 30, 0.001);
    //FloatImage weave_cut = quilt_cut(weave_sample, 300, 60, 10, 0.001);

    green_cut.write(DATA_DIR "/output/green_cut.png");
    // apples_cut.write(DATA_DIR "/output/apples_cut.png");
    //rice_cut.write(DATA_DIR "/output/rice_cut.png");
    // toast_cut.write(DATA_DIR "/output/toast_cut.png");
    // planks_cut.write(DATA_DIR "/output/planks_cut.png");
    //text_cut.write(DATA_DIR "/output/text_cut.png");
    //mat_cut.write(DATA_DIR "/output/mat_cut.png");
    //weave_cut.write(DATA_DIR "/output/weave_cut.png");
}



void testTransfer()
{
    // FloatImage apples_cut = quilt_cut(apples_sample, 500, 75, 5, .001);
    FloatImage rice_transfer = transfer(rice_sample, face_sample, 60, 10, .001, .2f, 3);
    //FloatImage rice_transfer_test = transfer(rice_small, face_small, 10, 2, .001, .3f, 3);

    // green_cut.write(DATA_DIR "/output/green_cut.png");
    // apples_cut.write(DATA_DIR "/output/apples_cut.png");
    rice_transfer.write(DATA_DIR "/output/rice_transfer.png");
    //rice_transfer_test.write(DATA_DIR "/output/rice_transfer_test.png");
}

int main()
{
    cout << "compiled" << endl;
    // testRandomQuilting();
    // testSimpleQuilting();
    //testQuiltCut();
    testTransfer();
}
