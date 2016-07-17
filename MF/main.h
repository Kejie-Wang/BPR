#ifndef TESTLOOP
#define TESTLOOP 10
#endif

// for looking into matrix (debug)
void debugRatingMatrix(PREVIEW_ON_ITEM* rm);

// return a point array, each element of the array is a list, which saves the rating history of the corresponding user
PREVIEW_ON_ITEM* initialRatingMatrix(char* filename, PREVIEW_ON_ITEM* rating_matrix);
// initialize feature vector with a random value between [0,1]
void initialFeatureVector(double (*featureV)[D], int num);
// initialize bias vector with a random value between [0,1]
void initialBiasVector(double* biasV, int num);