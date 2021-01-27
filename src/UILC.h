/*

*/

#include <errno.h>

#include <math.h>
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

#define ROO_BISECTION 1
#define ROO_FALSEPOS 2
#define ROO_BRENT 3

#define MIN_GOLDENSECTION 1
#define MIN_BRENT 2
#define MIN_QUADGOLDEN 3


typedef struct{
    double intensity;
    unsigned int m;
} UILC_LamberLED;

typedef struct{
    unsigned int m;
    unsigned int n;
} UILC_fparams_linear; // function paramaters for Morena Linear analytic function.

typedef struct{
    unsigned int m;
    unsigned int N;
    unsigned int M;
} UILC_fparams_Rectangle;


typedef struct{
    int N;
    int M;
    gsl_vector * coor;
}UILC_LED_Arr;


double UILC_f_Morena_Linear(const double x, void* p);
double UILC_f_Morena_SquareGrid(const double x, void * p);

double UILC_f_Morena_getdm_Linear(const UILC_LamberLED l, const int led_n, const unsigned int itetnum, const unsigned int min_selector, const unsigned int roo_selector, const unsigned double precison);
double UILC_f_Morena_getdm_SquareGrid(const UILC_LamberLED l, const unsigned int led_n, const unsigned int N, const unsigned int M, const unsigned int itetnum, const unsigned double precison);


double UILC_f_SingleLED_intensity(const UILC_LamberLED l, const double led_location,const double led_height, const double target_location, const double target_distance);

inline double  UILC_f_target_intensity(const UILC_LamberLED l, const UILC_LED_Arr arr[], const double target_location, const double target_distance );

UILC_LED_Arr * UILC_f_get_arrangement(const unsigned double dm, const char tp, const unsigned int N, const unsigend int M);
void UILC_f_Free_LED_Arr(UILC_LED_Arr * arr);

// For CA CASE 

double func_g_for_integral (double t, void *p);
double func_f (double x, double t, double height);
double integration_g(const double x, const double wide, const double height,const int tf);
double func_If(const double x, const double height,const double dx, const gsl_vector * a, const int Dim);
int set_Matrix_f( gsl_matrix * f, double dx, const int Dim);
int set_Vector_g( gsl_vector * v, double dx, const int Dim);
int set_Vector_Cab(gsl_vector * Cab, const_gsl_vector * C, const int Dim);
int get_Intensity_Distribution( gsl_vector * intense,  gsl_vector * a, const double height, const double width,const double dx, const int Dim);
