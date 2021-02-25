#include "../include/UILC.h"


/*
Pearson, J. W., Olver, S., & Porter, M. A. (2017). 
Numerical methods for the computation of the confluent and Gauss hypergeometric functions. 
Numerical Algorithms, 74(3), 821-866.

Transformation method

1.  -infty < z < -1     w = 1/(1-z) 
2.  -1 <= z < 0         w = z/(z-1)
3.  0 <= z <= 1/2       w = z
4.  1/2 <z <= 1         w = 1-z
5.  1 < z <= 2          w= 1-1/z
6.  2 < z < infty       w = 1/z
*/

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


    y =0.0;
    switch(region){
        case 1: 
        y= pow(w,a) * (gsl_sf_gamma(c) * gsl_sf_gamma(b-a)/(gsl_sf_gamma(b)*gsl_sf_gamma(c-a))) * gsl_sf_hyperg_2F1(a,c-b,a-b+1,w) 
        + pow(w,b) * (gsl_sf_gamma(c) * gsl_sf_gamma(a-b)/(gsl_sf_gamma(c)*gsl_sf_gamma(c-b))) * gsl_sf_hyperg_2F1(b,c-a,b-a+1,w); 
        break;
        case 2: 
        y= pow(1-w,a) * gsl_sf_hyperg_2F1(a,c-b,c,w);
        break;
        case 4: 
        y= (gsl_sf_gamma(c) * gsl_sf_gamma(c-a-b)/(gsl_sf_gamma(c-a)*gsl_sf_gamma(c-b)))* gsl_sf_hyperg_2F1(a,b,a+b-c+1,w)
        + pow(w, c-a-b) * (gsl_sf_gamma(c) * gsl_sf_gamma(a+b-c)/(gsl_sf_gamma(a)*gsl_sf_gamma(b)))* gsl_sf_hyperg_2F1(c-a,c-b,c-a-b+1,w);  
        break;
        case 5: 
        
        break;
        case 6: 
        
        break;
    }

    return(y);
    //gsl_sf_hyperg_2F1(a,b,c,x)
}
 