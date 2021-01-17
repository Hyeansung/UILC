/*

*/

#include <errno.h>

#include <math.h>
#include <gsl/gsl_roots.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_min.h>
#include <gsl/gsl_block.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_siman.h>
#include <gsl/gsl_errno.h>


#define Linear 'L' 
#define Square 'S'

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


typedef struct{
    int N;
    int M;
    gsl_vector * coor;
}UILC_LED_Arr;


double UILC_f_Morena_Linear(const double x, void* p);
double UILC_f_Morena_SquareGrid(const double x, void * p);

double UILC_f_Morena_getdm_linear(const UILC_LamberLED l, const int led_n, const unsigned int itetnum, const unsigned double precison);
double UILC_f_Morena_getdm_SquareGrid(const UILC_LamberLED l, const unsigned int led_n, const unsigned int N, const unsigned int M, const unsigned int itetnum, const unsigned double precison);


double UILC_f_SingleLED_intensity(const UILC_LamberLED l, const double led_location,const double led_height, const double target_location, const double target_distance);

inline double  UILC_f_target_intensity(const UILC_LamberLED l, const UILC_LED_Arr arr[], const double target_location, const double target_distance );

UILC_LED_Arr * UILC_f_get_arrangement(const unsigned double dm, const char tp, const unsigned int N, const unsigend int M);
void UILC_f_Free_LED_Arr(UILC_LED_Arr * arr);

