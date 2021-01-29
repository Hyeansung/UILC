/*======================================================================================================
Licence: GPL_3 version

Author: Kim, Hyeon Sung
Date: 2021.01.13

This code is a library functions for the calculation the uniform illuination patteren with 
Ivan Morena's paper, Simulated Annealing Algorithm and other methods.


[library name]_[object type]_[Method name]_[Additional field]_...

[library name]
- UILC:  Uniform Illumination LED arrangement Calculation
[object type]
- f: function
- fparams: function parameters


This library provide the 

i) Morena's Analytic Solution
ii) Siemulated Annealing Solution
iii) Genetic Algorithm Solution
iV) Morena-Hyeon Solution

For Linear and Rectangle Grid Array

======================================================================================================*/
//Standard Library

#include <errno.h>
#include <math.h>

//GSL Library
#include <gsl/gsl_roots.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_min.h>
#include <gsl/gsl_block.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_siman.h>
#include <gsl/gsl_errno.h>
#include<gsl/gsl_integration.h>
#include<gsl/gsl_matrix.h>
#include<gsl/gsl_vector.h>
#include<gsl/gsl_linalg.h>
#include<gsl/gsl_blas.h>


#define Linear 'L' 
#define Square 'S'

#define Lamber 1
#define Poly 2

#define ROO_BISECTION 1
#define ROO_FALSEPOS 2
#define ROO_BRENT 3

#define MIN_GOLDENSECTION 1
#define MIN_BRENT 2
#define MIN_QUADGOLDEN 3

//LED struct---------------------------------------------------------------
typedef struct{
    double intensity;
    unsigned int m;
} UILC_Lamber_LED;

typedef struct{
    unsigned int n;
    double * param;
} UILC_Poly_LED;
//Morena Analytic solution function params----------------------------------
typedef struct{
    unsigned int m;
    unsigned int n;
} UILC_fparams_linear; 

typedef struct{
    unsigned int m;
    unsigned int N;
    unsigned int M;
} UILC_fparams_Rectangle;

//General LED Array---------------------------------------------------------
typedef struct{
    int N;
    int M;
    gsl_vector * coor;
}UILC_LED_Arr;

//==========================================================================
//Functions
//==========================================================================

//General_functions("UILC_Core.c")------------------------------------------

//=>LED function: The below functions will be called by other function frequently. So they are defined as inline function.
inline double UILC_f_get_intensity_Lamber(
    UILC_Lamber_LED led, 
    const double theta
);

inline double UILC_f_get_intensity_Poly(
    UILC_Poly_LED led, 
    const double theta
);

inline double UILC_f_get_intensity_Lamber_target(
    UILC_Lamber_LED ledmodel,
    gsl_vector led,
    gsl_vector target
);
inline double UILC_f_get_intensity_Poly_target(
    UILC_Poly_LED ledmodel,
    gsl_vector led,
    gsl_vector target
);
//=>LED array function
UILC_LED_Arr UILC_f_Arr_alloc(
    const unsigned int N, 
    const unsigned int M
);

int UILC_f_Arr_free(
    UILC_LED_Arr arr
);

gsl_vector UILC_f_get_ArrCoordinate(
    UILC_LED_Arr arr,
    const unsigned int i,
    const unsigned int j
);
int UILC_f_set_ArrCoordinate(
    UILC_LED_Arr arr,
    const unsigned int i,
    const unsigned int j,
    const gsl_vector * value
);

int UILC_f_set_AllArrCoordinate(
    UILC_LED_Arr arr,
    gsl_vector * (*fill)(unsigned int, unsigned int)
); // Be aware of the 'fill' function form.

double UILC_f_get_intensity_arr_Lamber_target(
    UILC_LED_Arr arr,
    UILC_Lamber_LED led,
    gsl_vector target
);
double UILC_f_get_intensity_arr_Poly_target(
    UILC_LED_Arr arr,
    UILC_Poly_LED led,
    gsl_vector target
);

double UILC_f_get_arr_target_Area(
    UILC_LED_Arr arr
);

// "UILC_Morena.c"----------------------------------------------------------

double UILC_f_Morena_Linear(
    const double x, 
    void* p
);
double UILC_f_Morena_SquareGrid(
    const double x, 
    void * p
);

double UILC_f_Morena_getdm_Linear(
    const UILC_LamberLED l, 
    const int led_n, 
    const unsigned int itetnum, 
    const unsigned int min_selector,
    const unsigned int roo_selector,
    const double precison);

double UILC_f_Morena_getdm_SquareGrid( // return the dm for Square Grid
    const UILC_LamberLED l, 
    const unsigned int led_n, 
    const unsigned int N, 
    const unsigned int M, 
    const unsigned int itetnum, 
    const unsigned int min_selector,
    const unsigned int roo_selector,
    const double precison);

UILC_LED_Arr  UILC_f_Morena_get_Arr(
    const double dm, 
    const char tp, //Linear, Square selctor
    const unsigned int N, 
    const unsigned int M);
// "UILC_Hyeon.c"----------------------------------------------------------
/*
UILC_LED_Arr UILC_f_Hyeon_get_Arr(

);
*/
// "UILC_Siman.c"----------------------------------------------------------
/*
UILC_LED_Arr UILC_f_Siman_get_Arr(

);
*/
// "UILC_Genetic.c"--------------------------------------------------------
/*
UILC_LED_Arr UILC_f_Genetic_get_Arr(

);
*/
