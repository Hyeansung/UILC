#include "../include/UILC.h"


/*
Reference
Robert C. Forrey,
Computing the Hypergeometric Function,
Journal of Computational Physics,
Volume 137, Issue 1,
1997,
Pages 79-100,
ISSN 0021-9991,

Transformation method

1.  -infty < z < -1     w = 1/(1-z) 
2.  -1 <= z < 0         w = z/(z-1)
3.  0 <= z <= 1/2       w = z
4.  1/2 <z <= 1         w = 1-z
5.  1 < z <= 2          w= 1-1/z
6.  2 < z < infty       w = 1/z
*/

inline double UILC_f_hyperg_2F1_aR(double a, double b, double c, double x, double * Im)
{
    double w = 0.0;
    double y =0.0;
    int region =0;
    int ab =0, cba =0;

    ab = (fabs(modf(b-a,&y)) < DBL_EPSILON) ? 1:0;
    cba = (fabs(modf(c-(b-a),&y)) < DBL_EPSILON) ? 1:0;

    if( fabs(x) < 1.0){
        return(gsl_sf_hyperg_2F1(a,b,c,x));
    }
    else{
        if(x < -1.0){ w = 1/(1-x); region = 1;}
        else if(-1/0 <= x || x <0){ w = x/(x-1); region = 2; }
        else if(0.5 < x || x <= 1.0){ w = 1-x; region = 4;}
        else if(1<x|| x <=2){ w = 1-1/x; region = 5;}
        else{ w =1/x; region =6;}
    }
    // Exceptional Case: case 1, 6: a-b: integer, case 4,5 c-a-b: integer

    y =0.0;
    switch(region)
    {
        case 1: 
            if(ab == 0)
            {
                y= pow(w,a) * (gsl_sf_gamma(c) * gsl_sf_gamma(b-a)/(gsl_sf_gamma(b)*gsl_sf_gamma(c-a))) * gsl_sf_hyperg_2F1(a,c-b,a-b+1,w) 
                + pow(w,b) * (gsl_sf_gamma(c) * gsl_sf_gamma(a-b)/(gsl_sf_gamma(c)*gsl_sf_gamma(c-b))) * gsl_sf_hyperg_2F1(b,c-a,b-a+1,w); 
            }
            else
            {

            }
        break;
        case 2: 
            y= pow(1-w,a) * gsl_sf_hyperg_2F1(a,c-b,c,w);
        break;
        case 4: 
            if(cba == 0)
            {
                y= (gsl_sf_gamma(c) * gsl_sf_gamma(c-a-b)/(gsl_sf_gamma(c-a)*gsl_sf_gamma(c-b)))* gsl_sf_hyperg_2F1(a,b,a+b-c+1,w)
                + pow(w, c-a-b) * (gsl_sf_gamma(c) * gsl_sf_gamma(a+b-c)/(gsl_sf_gamma(a)*gsl_sf_gamma(b)))* gsl_sf_hyperg_2F1(c-a,c-b,c-a-b+1,w);  
            }
            else
            {

            }
        break;
        case 5: 
            if(cba == 0)
            {

            }
            else
            {

            }
        break;
        case 6: 
            if(ab == 0)
            {

            }
            else
            {

            }
        break;
    }

    return(y);
    //gsl_sf_hyperg_2F1(a,b,c,x)
}

inline double UILC_f_Hyeon_matrix_value_Lamber(UILC_Lamber_LED led, int i, int j, double d, double H)
{
    double theta = arctan(abs(i-j)*d /H);
    return(UILC_f_get_intensity_Lamber(led, theta));
}

inline double UILC_f_Hyeon_matrix_value_Poly(UILC_Poly_LED led, int i, int j, double d, double H)
{
    double theta = arctan(abs(i-j)*d /H);
    return(UILC_f_get_intensity_Poly(led, theta));    
}

inline gsl_vector * UILC_f_Hyeon_ASM(gsl_matrix * A, gsl_vector * b)
{

}

inline gsl_vector * UILC_f_Hyeon_Disctetized(gsl_vector * sigma)
{

}
 
inline gsl_vector * UILC_f_Hyeon_Linear_Lamber(UILC_Lamber_LED led, double width, double height, int dimension, int ACM)
{
    double y =0.0;
    int n_max = (int) modf(1/UILC_f_hyperg_2F1_aR(0.5,led.m/2 +1,1.5,-pow(W/(2*H),2)), &y );

    gsl_matrix * F = gsl_matrix_calloc(dimension, dimenstion);
    gsl_vector * I = gsl_vector_calloc(dimension);
    gsl_vector * sigma = gsl_vector_calloc(dimension);

    UILC_matrix_setting_Lamber(F, &UILC_f_Hyeon_matrix_value_Lamber); 

    if(dimension <= n_max  ) //inverse
    {
        gsl_permutation p;
        int signum=0;
        gsl_linalg_LU_decomp(F,&p,&signum);
        gsl_linalg_LU_solve(F, p ,I,sigma);

    }
    else // active set method
    {

    }

    gsl_matrix_free(F);

    return(UILC_f_Hyeon_Discretized(sigma));
}





 /*
 
inline double hyperg_2F1_aR(double a, double b, double c, double x, double * Im)
{
    double w = 0.0;
    double y =0.0;
    int region =0;
    int ab =0, cba =0;

    ab = (fabs(modf(b-a,&y)) < DBL_EPSILON) ? 1:0;
    cba = (fabs(modf(c-(b-a),&y)) < DBL_EPSILON) ? 1:0;

    if( fabs(x) < 1.0){
        return(gsl_sf_hyperg_2F1(a,b,c,x));
    }
    else{
        if(x < -1.0){ w = 1/(1-x); region = 1;}
        else if(-1/0 <= x || x <0){ w = x/(x-1); region = 2; }
        else if(0.5 < x || x <= 1.0){ w = 1-x; region = 4;}
        else if(1<x|| x <=2){ w = 1-1/x; region = 5;}
        else{ w =1/x; region =6;}
    }
    // Exceptional Case: case 1, 6: a-b: integer, case 4,5 c-a-b: integer

    y =0.0;
    switch(region)
    {
        case 1: 
            if(ab == 0)
            {
                y= pow(w,a) * (gsl_sf_gamma(c) * gsl_sf_gamma(b-a)/(gsl_sf_gamma(b)*gsl_sf_gamma(c-a))) * gsl_sf_hyperg_2F1(a,c-b,a-b+1,w) 
                + pow(w,b) * (gsl_sf_gamma(c) * gsl_sf_gamma(a-b)/(gsl_sf_gamma(c)*gsl_sf_gamma(c-b))) * gsl_sf_hyperg_2F1(b,c-a,b-a+1,w); 
            }
            else
            {

            }
        break;
        case 2: 
            y= pow(1-w,a) * gsl_sf_hyperg_2F1(a,c-b,c,w);
        break;
        case 4: 
            if(cba == 0)
            {
                y= (gsl_sf_gamma(c) * gsl_sf_gamma(c-a-b)/(gsl_sf_gamma(c-a)*gsl_sf_gamma(c-b)))* gsl_sf_hyperg_2F1(a,b,a+b-c+1,w)
                + pow(w, c-a-b) * (gsl_sf_gamma(c) * gsl_sf_gamma(a+b-c)/(gsl_sf_gamma(a)*gsl_sf_gamma(b)))* gsl_sf_hyperg_2F1(c-a,c-b,c-a-b+1,w);  
            }
            else
            {

            }
        break;
        case 5: 
            if(cba == 0)
            {

            }
            else
            {

            }
        break;
        case 6: 
            if(ab == 0)
            {

            }
            else
            {

            }
        break;
    }

    return(y);
    //gsl_sf_hyperg_2F1(a,b,c,x)
}
 
 */