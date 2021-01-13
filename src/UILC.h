/*

*/

#include <errno.h>
#include <gsl/gsl_roots.h>
#include <gsl/gsl_math.h>
#include <math.h>

typedef struct{
    double intensity;
    unsigned int m;
} UILC_LamberLED;

typedef struct{
    unsigned int m;
    unsigned int n;
} UILC_fparams_linear;

typedef struct{
    unsigned int m;
    unsigned int N;
    unsigned int M;
} UILC_fparams_Rectangle;