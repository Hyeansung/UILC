/*
Programmer: Kim, Hyeon Sung
Date: 2021.01.13

This code is a library functions for the calculation the uniform illuination patteren with 
Ivan Morena's paper, Simulated Annealing Algorithm and other methods.


[library name]_[object type]_[Method name]_[Additional field]_...

[library name]
- UILC:  Uniform Illumination LED arrangement Calculation
[object type]
- f: function
- fparams: function parameters


*/

#include "UILC.h"

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

#define Linear 'L' 
#define Square 'S'

// LED optimization arrays parts -----------------------------------------------------------------------------------------
// These are the functions for the Root or minimum point finding in the Morena's analytic solution.
// There are two case exist Linear, Square grid arrangement.
double UILC_f_Morena_Linear(const double x, void* p){

    UILC_fparams_linear * params = (UILC_fparams_linear *)p;
    const unsigned int m =(params->m);
    const unsigned int n =(params->n);
    double y =0.00;
    for(int i=1; i<n+1;i++){
        y += (1-(m+3)*gsl_pow_2(((n+1-2*(i))*(x/2)))) / pow(( 1+ gsl_pow_2((x/2)*(N+1-2*(i)))),(m/2+3));
    }
    return y;
}
double UILC_f_Morena_SquareGrid(const double x, void * p){

    UILC_fparams_Rectangle * params = (UILC_fparams_Rectangle *)p;
    const unsigned int m = (params->m);
    const unsigned int N = (params->N);
    const unsigned int M = (params->M);

    double y =0.00;
    for(int i =1; i < N+1; i++ ){
        for(int j=1; j< M+1; j++){
            y+= (1-((m+3)*gsl_pow_2(N+1-2*i)-gsl_pow_2(M+1-2*j))*gsl_pow_2(x/2))/pow((gsl_pow_2(N+1-2*i)+gsl_pow_2(M+1-2*j))*gsl_pow_2(x/2)+1,(m/2+6));
        }    
    }
    return y;
}
//--------------------------------------------------------------------------------

// These functions will return the Morena's analytic solution in the single double number type as the distance of the give LEDs.
// Two cases are here, Linear and Rectangle.
double UILC_f_Morena_getdm_linear(const UILC_LamberLED l, const int led_n, const unsigned int itetnum, const unsigned double precison){
    
    double dm =0.0;
    double l_x_lower = 0.0;
    double l_x_upper = 1.0;
    int status =0;
    int iter =0, max_iter = itetnum;
    UILC_fparams_linear l_params = {l.m,led_n};
    gsl_function F;
    F.function = &UILC_f_Morena_Linear;
    F.params = &l_params;

    if(GSL_IS_ODD(n)){ // n = odd case we need to find the local minimum case

        /*
        There are 3 algorithms are provided in GSL library for find minimization of the function.
        - gsl_min_fminimizer_goldensection : The simplest method of bracketing the minimum of a function. It is the slowest algorithm provided by the library, with linear convergence.
        - gsl_min_fminimizer_brent : Using parabolic interpolation with the golden section algorithm. This produces a fast algorithm which is still robust.
        - gsl_min_fminimizer_quad_golden : This is a variant of Brent’s algorithm which uses the safeguarded step-length algorithm of Gill and Murray
        */
        const gsl_min_fminimizer_type * T = gsl_min_fminimizer_goldensection;
        gsl_min_fminimizer * s = gsl_min_fminimizer_alloc (T);
        gsl_min_fminimizer_set(s,&F,dm,l_x_lower,l_x_upper);

        do{
            iter++;
            status = gsl_min_fminimizer_iterate (s);
            dm = gsl_min_fminimizer_x_minimum (s);
            l_x_lower = gsl_root_fsolver_x_lower (s);
            l_x_upper = gsl_root_fsolver_x_upper (s);
            status = gsl_root_test_interval(l_x_lower, l_x_upper, 0, precison )
        }
        while(status == GSL_CONTINUE && iter < max_iter)
        
        gsl_min_fminnizmizer_free(s);
    }
    else{ // n = even then we need to find the root of the function.
        /*
        There are 3 algorithms are provided in GSL library for find root without derivative.
        - gsl_root_fsolver_bisection : simplest method, slowest algorithms with linear convergence.
        - gsl_root_fsolver_falsepos : using linear interpolation, linear convergence, faster than bisection.
        - gsl_root_fsolver_brent : combines the interpolation strategy with the bisection algorithm. This produces a fast alorithm which is still robust.
        */
        const gsl_root_fsolver_type * T = gsl_root_fsolver_bisection;
        gsl_root_fsolver * s = gsl_root_fsolver_alloc(T);
        gsl_root_fsolver_set(s,&F,l_x_lower,l_x_upper);

        do{
            iter++;
            status = gsl_root_fsolver_iterate (s);
            dm = gsl_root_fsolver_root (s);
            l_x_lower = gsl_root_fsolver_x_lower (s);
            l_x_upper = gsl_root_fsolver_x_upper (s);
            status = gsl_root_test_interval(l_x_lower, l_x_upper, 0, precison )
        }
        while(status == GSL_CONTINUE && iter < max_iter)

        gsl_root_fsolver_free(s);
        
    }

    return dm;
    

}
double UILC_f_Morena_getdm_SquareGrid(const UILC_LamberLED l, const unsigned int led_n, const unsigned int N, const unsigned int M, const unsigned int itetnum, const unsigned double precison){
    // this function won't return the negative double vlaue unless there is an error
    if(led_n != N*M){
        return -1.0
    }

    double dm =0.0;
    double l_x_lower = 0.0;
    double l_x_upper = 1.0;
    int status =0;
    int iter =0, max_iter = itetnum;
    UILC_fparams_Rectangle R_param = {l.m, N, M};
    gsl_function F;
    F.function = &UILC_f_Morena_SquareGrid;
    F.params = &l_params;

    if( GSL_IS_ODD(N) && GSL_IS_ODD(M)){ // both odd Minimization
        const gsl_min_fminimizer_type * T = gsl_min_fminimizer_goldensection;
        gsl_min_fminimizer * s = gsl_min_fminimizer_alloc (T);
        gsl_min_fminimizer_set(s,&F,dm,l_x_lower,l_x_upper);

        do{
            iter++;
            status = gsl_min_fminimizer_iterate (s);
            dm = gsl_min_fminimizer_x_minimum (s);
            l_x_lower = gsl_root_fsolver_x_lower (s);
            l_x_upper = gsl_root_fsolver_x_upper (s);
            status = gsl_root_test_interval(l_x_lower, l_x_upper, 0, precison )
        }
        while(status == GSL_CONTINUE && iter < max_iter)
        
        gsl_min_fminnizmizer_free(s);
    } 
    else if(GSL_IS_EVEN(N) && GSL_IS_EVEN(M)){ // both even: Root
        const gsl_root_fsolver_type * T = gsl_root_fsolver_bisection;
        gsl_root_fsolver * s = gsl_root_fsolver_alloc(T);
        gsl_root_fsolver_set(s,&F,l_x_lower,l_x_upper);

        do{
            iter++;
            status = gsl_root_fsolver_iterate (s);
            dm = gsl_root_fsolver_root (s);
            l_x_lower = gsl_root_fsolver_x_lower (s);
            l_x_upper = gsl_root_fsolver_x_upper (s);
            status = gsl_root_test_interval(l_x_lower, l_x_upper, 0, precison )
        }
        while(status == GSL_CONTINUE && iter < max_iter)

        gsl_root_fsolver_free(s);

    }
    else{ // Minimum point or the root
        const gsl_min_fminimizer_type * T = gsl_min_fminimizer_goldensection;
        gsl_min_fminimizer * s = gsl_min_fminimizer_alloc (T);
        gsl_min_fminimizer_set(s,&F,dm,l_x_lower,l_x_upper);

        do{
            iter++;
            status = gsl_min_fminimizer_iterate (s);
            dm = gsl_min_fminimizer_x_minimum (s);
            l_x_lower = gsl_root_fsolver_x_lower (s);
            l_x_upper = gsl_root_fsolver_x_upper (s);
            status = gsl_root_test_interval(l_x_lower, l_x_upper, 0, precison )
        }
        while(status == GSL_CONTINUE && iter < max_iter)
        
        gsl_min_fminnizmizer_free(s);

        if(dm <0.0){
            const gsl_root_fsolver_type * T = gsl_root_fsolver_bisection;
            gsl_root_fsolver * s = gsl_root_fsolver_alloc(T);
            gsl_root_fsolver_set(s,&F,l_x_lower,l_x_upper);
               
            l_x_upper = dm; // if dm <0.0 then there is a first root on interval and it is enought to serach 0 to dm;minium point.
            
            do{
                iter++;
                status = gsl_root_fsolver_iterate (s);
                dm = gsl_root_fsolver_root (s);
                l_x_lower = gsl_root_fsolver_x_lower (s);
                l_x_upper = gsl_root_fsolver_x_upper (s);
                status = gsl_root_test_interval(l_x_lower, l_x_upper, 0, precison )
            }
            while(status == GSL_CONTINUE && iter < max_iter)

            gsl_root_fsolver_free(s);

        }
    }
}


// Return the optimized arrangement of the linear and Rectangle region as the vector or the marix
// Given led parameters and the number of the LED, shape.


// Return the optimized arrangement of the given linear and rectangle region with the # of the LED
// Givem led parameters and the width, area of the region, shape.



// LED optimization arrays parts end-----------------------------------------------------------------------------------------

double UILC_f_SingleLED_intensity(const UILC_LamberLED l, const double led_location,const double led_height, const double target_location, const double target_distance){
    double H = abs(target_distance - led_height);
    double d = abs(led_location - target_location);

    return( l.intensity/pow((1+ gsl_pow_2(d/H)),l.m/2+1) );
}

gsl_matrix * UILC_f_target_intensity(const UILC_LamberLED l, const gsl_block * arrangemennt, const double dm, const double distance ){

}

gsl_matrix * UILC_f_get_arrangement(const unsigned double dm, const char tp, const unsigned int N, const unsigend int M){
    if(tp == Linear){
        gsl_matrix* arr = gsl_matrix_calloc(1,N);
    }
    else if(tp == Square){
        gsl_matrix* arr = gsl_matrix_calloc(N,M);

    }

}
