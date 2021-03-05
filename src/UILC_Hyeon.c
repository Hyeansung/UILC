#include "../include/UILC.h"


typedef struct{
    double a;
    double b;
    double c;
    int k;
    double w;
}Hyper_param;

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
    and its exceptional case a-b : integer
*/
//Calculate the Gauss Hypergeometric function 2F1 for negative value--------------------NEED TEST----------------------------------
inline static double UILC_f_s_Pochhammer(double a, int n) // (a)_n
{ 
    if(n ==0){return(1);}

    double result =1.0;
    for(int i =0; i<n;i++)
    {
        result = result * (a+i)
    }
    return(result);
}
inline static double UILC_f_s_fd(int i,double epsilon, int n,Hyper_param *p)// f_i(epsilon;n)
{
    double result = 0.0;
    switch(i)
    {
        case 1: result = UILC_f_s_Pochhammer(p->a-p->k-epsilon,n+p->k) / gsl_sf_gamma(p->a);          
        break;
        case 2: result = UILC_f_s_Pochhammer(p->c-p->a+p->k+epsilon,n) / gsl_sf_gamma(p->c-p->a);        
        break;
        case 3: result = ((n+p->k)%2 ? -1:1) * epsilon * gsl_sf_gamma(-(p->k+n+epsilon));     
        break;
        case 4: result = ((n)%2 ? -1:1) * gsl_sf_gamma(epsilon-n);                      
        break;
        case 5: result = pow(w, p->a+n-epsilon);                                           
        break;
        case 6: result = 1/gsl_sf_gamma(p->a-p->k-epsilon);                                   
        break;
        case 7: result = 1/gsl_sf_gamma(p->c-p->a+p->k+epsilon);
        break;

    }
    return(result);
}
inline static double UILC_f_s_gd(int i, double epsilon, int n, Hyper_param * p)// g_i(epsilon;n)
{
    return((UILC_f_s_fd(i,epsilon,n,p) -UILC_f_s_fd(i,0.0,n,p) )/epsilon);
}

inline static double UILC_f_s_hSum1(Hyper_param* p)
{
    double result = 0.0;
    for(int i=0; i<p->k ;i++)
    {
        result += ((i%2)?-1:1)*UILC_f_s_Pochhammer(p->b,i) * UILC_f_s_Pochhammer(p->c - p->a,i) * 
                gsl_sf_gamma(p->k-i+epsilon)*pow(p->w,p->b+i) / UILC_f_s_Pochhammer(1,i);
    }
    return(result);
}
inline static double UILC_f_s_hSum2(double epsilon, Hyper_param* p)
{
    double result =0.0;
    int i=0;

    do
    {
        result +=
        UILC_f_s_Pochhammer(p->a,i) * UILC_f_s_fd(2,epsilon,i,p) * UILC_f_s_fd(3,epsilon,i,p) * UILC_f_s_fd(4,0,i,p) * UILC_f_s_gd(6,epsilon,i,p)
        -UILC_f_s_Pochhammer(p->c-p->a,p->k+i)*UILC_f_s_fd(1,epsilon,i,p) * UILC_f_s_fd(3,0,i,p) * UILC_f_s_fd(4,epsilon,i,p) * UILC_f_s_fd(5,epsilon,i,p) * UILC_f_s_gd(7,epsilon,i,p)
        -UILC_f_s_gd(1,epsilon,i,p) * UILC_f_s_fd(2,epsilon,i,p) * UILC_f_s_fd(3,epsilon,i,p) * UILC_f_s_fd(4,0,i,p) * UILC_f_s_fd(5,0,i,p)
        +UILC_f_s_gd(2,epsilon,i,p) * UILC_f_s_fd(1,epsilon,i,p) * UILC_f_s_fd(3,epsilon,i,p) * UILC_f_s_fd(4,0,i,p) * UILC_f_s_fd(5,0,i,p)
        +UILC_f_s_gd(3,epsilon,i,p) * UILC_f_s_fd(1,epsilon,i,p) * UILC_f_s_fd(2,0,i,p) * UILC_f_s_fd(4,0,i,p) * UILC_f_s_fd(5,0,i,p)
        -UILC_f_s_gd(4,epsilon,i,p) * UILC_f_s_fd(1,epsilon,i,p) * UILC_f_s_fd(2,0,i,p) * UILC_f_s_fd(3,0,i,p) * UILC_f_s_fd(5,0,i,p)
        -UILC_f_s_gd(5,epsilon,i,p) * UILC_f_s_fd(1,epsilon,i,p) * UILC_f_s_fd(2,0,i,p) * UILC_f_s_fd(3,0,i,p) * UILC_f_s_fd(4,epsilon,i,p);
    } while ( i++ < 30001);

    return(result);
}

    
inline static double UILC_f_s_hyperg_2F1_aR(double a, double b, double c, double x)
{
    if(a <b)
    {
        y =b; 
        b =a;
        a =y;
    }

    Hyper_param p = {a, b, c, modf(a-b, &y), 1/(1-x)};
    double y =0.0;
    int region =0;
    int ab =0, cba =0;
    ab = (y < DBL_EPSILON) ? 1:0;

    y =0;

    if( fabs(x) < 1.0)
    {
        y = (gsl_sf_hyperg_2F1(a,b,c,x));
    }
    else if(fabs(x -1.0) < DBL_EPSILON)
    {
        p.w= 0.5;
        y= pow(1-p.w,a) * gsl_sf_hyperg_2F1(a,c-b,c,p.w);
    }
    else if(ab == 0)
    {
        y = pow(p.w,a) * ( gsl_sf_gamma(c)*gsl_sf_gamma(b-a) / (gsl_sf_gamma(b) * gsl_sf_gamma(c-a)) ) * gsl_sf_hyperg_2F1(a,c-b,a-b+1,p.w)
          + pow(p.w,b) * ( gsl_sf_gamma(c)*gsl_sf_gamma(a-b) / (gsl_sf_gamma(a) * gsl_sf_gamma(c-b)) ) * gsl_sf_hyperg_2F1(a,c-a,b-a+1,p.w);

    }
    else
    {
        y = 
        (gsl_sf_gamma(c)/(gsl_sf_gamma(a) * gsl_sf_gamma(c-b))) * (UILC_f_s_hSum1(&p))
         + pow(-1,k) gsl_sf_gamma(c) * (UILC_f_s_hSum2(100*DBL_EPSILON,&p)) ;

    return(y);
}
//----------------------------------------------------------------------------------------------------------
// Matrix setting ---------NEED FIXED-----------------------------------------------------------------------
inline double UILC_f_s_matrix_value_Lamber(UILC_Lamber_LED led, int i, int j, double d, double H)
{
    double theta = arctan(abs(i-j)*d /H);
    return(UILC_f_get_intensity_Lamber(led, theta));
}

inline double UILC_f_s_matrix_value_Poly(UILC_Poly_LED led, int i, int j, double d, double H)
{
    double theta = arctan(abs(i-j)*d /H);
    return(UILC_f_get_intensity_Poly(led, theta));    
}

extern inline int UILC_f_s_matrix_setting_Lamber(gsl_matrix * A,  double (*f)(UILC_Lamber_LED led, int i, int j,))
{
    int m = A->size1;
    int n = A->size2;

    for(int i=0; i<m, ;i++)
    {
        for(int j=0; j <n; j++)
        {
            gsl_matrix_set(A,i, j, f(led,i,j));
        }
    }
    return(GSL_SUCESS);
}

extern inline int UILC_f_s_matrix_setting_Poly(gsl_matrix * A,  double (*f)(UILC_Lamber_LED led, int i, int j))
{
    int m = A->size1;
    int n = A->size2;

    for(int i=0; i<m, ;i++)
    {
        for(int j=0; j <n; j++)
        {
            gsl_matrix_set(A,i, j, f(led,i,j));
        }
    }
    return(GSL_SUCESS);
}
//----------------------------------------------------------------------------------------------------------

//Discretize the sigma to real arrangement------------------------------------------------------------------

inline gsl_vector * UILC_f_Hyeon_Disctetized(gsl_vector * sigma)
{
    //initialize:
    gsl_matrix * P = gsl_matrix_calloc(A->size2);
    gsl_matrix * R = gsl_matrix_calloc(A->size2);
    gsl_vector * x = gsl_vector_calloc(A->size2);

}
//----------------------------------------------------------------------------------------------------------

inline gsl_vector * UILC_f_Hyeon_Linear_Lamber(UILC_Lamber_LED led, double width, double height, int dimension, int ACM)
{
    double y =0.0;

    int n_max = (int) modf(1/UILC_f_s_hyperg_2F1_aR(0.5,led.m/2 +1,1.5,-pow(W/(2*H),2)), &y);

    gsl_matrix * F = gsl_matrix_calloc(dimension, dimension);
    gsl_vector * I = gsl_vector_calloc(dimension);
    gsl_vector * sigma = gsl_vector_calloc(dimension);

    UILC_f_s_matrix_setting_Lamber(F, &UILC_f_Hyeon_matrix_value_Lamber); 

    if(dimension <= n_max && ACM == 0 ) //inverse
    {
        gsl_permutation p;
        int signum=0;
        gsl_linalg_LU_decomp(F,&p,&signum);
        gsl_linalg_LU_solve(F, p ,I,sigma);

    }
    else // active set method
    {
        UILC_f_Hyeon_s_ASM(F, I, sigma);
    }

    gsl_matrix_free(F);

    return(UILC_f_Hyeon_Discretized(sigma));
}





 