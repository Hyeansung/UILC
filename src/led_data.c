
#include <errno.h>
#include <gsl/gsl_roots.h>
#include <gsl/gsl_math.h>
#include <math.h>

typedef struct{
    double intensity;
    unsigned int m;
} UILC_LamberLED;

// need to fix for the gsl function ( variable and params )
double UILC_f_Morena_Linear(const unsigned int m,const unsigned int n, const double x){
    double y =0.00;
    for(int i=1; i<n+1;i++){
        y += (1-(m+3)*gsl_pow_2(((n+1-2*(i))*(x/2)))) / pow(( 1+ gsl_pow_2((x/2)*(N+1-2*(i)))),(m/2+3));
    }
    return y;
}
double UILC_f_Morena_Rectange(const unsigned int m,const unsigned int n, const double x){
    double y =0.00;
    for(int i =1; i < N+1; i++ ){
        for(int j=1; j< M+1; j++){
            y+= (1-((m+3)*gsl_pow_2(N+1-2*i)-gsl_pow_2(M+1-2*j))*gsl_pow_2(x/2))/pow((gsl_pow_2(N+1-2*i)+gsl_pow_2(M+1-2*j))*gsl_pow_2(x/2)+1,(m/2+6));
        }    
    }
    return y;
}



double UILC_get_Morena_dm_linear(UILC_LamberLED l, const int n){
    if(GSL_IS_ODD(n)){

    }
    else{
        
    }
    

}

double UILC_get_Morena_dm_Rectangle(UILC_LamberLED l, const unsigned int n, const unsigned int N, const unsigned int M){
    // this function won't return the negative double vlaue unless there is an error
    if(n != N*M){
        return -1.0
    }

    if( GSL_IS_ODD(N) && GSL_IS_ODD(M)){

    } 
    else if(GSL_IS_EVEN(N) && GSL_IS_EVEN(M)){

    }
    else{
        
    }
}