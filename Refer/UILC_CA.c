
#include "UILC.h"


double func_g_for_integral  (double t, void *p){
    struct f_params * params = (struct f_params *)p;
    double h= (params->h);
    double x= (params->x);

    return gsl_pow_int(gsl_hypot(h,x-t),-2*M);
}

double func_f (double x, double t, double height){
    return gsl_pow_int(gsl_hypot(height,x-t),-2*M);
}

double integration_g(const double x, const double wide, const double height,const int tf){
    double params[] = {height,x};
    gsl_integration_workspace *w = gsl_integration_workspace_alloc(1000);
    gsl_function F;
    F.function = &func_g_for_integral;
    F.params = params;
    double result, error;
    if(tf==1){
        gsl_integration_qagiu(&F,wide/2,0,1e-8, 1000,w,&result,&error);
    }
    else{
        gsl_integration_qag(&F,wide/2,wide/2,0,1e-8, 1000,GSL_INTEG_GAUSS21,w,&result,&error);
    }

    gsl_integration_workspace_free(w);
    return result;

}

double func_If(const double x, const double height,const double dx, const gsl_vector * a, const int Dim){
    double result = 0.0;
    for(int i=0;i<Dim; i++){
        result += gsl_vector_get(a,i)*(func_f(x,i*dx,height) + func_f(x,-i*dx,height)); 
    }
    return result;

}

int set_Matrix_f(gsl_matrix * f, double dx, const int Dim){
    for(int i=0; i<Dim ;i++){
        for(int j=0; j<Dim;j++){
            gsl_matrix_set(f, i,j,func_f(i*dx,j*dx,HEIGHT));
        }
    }
    return 0;
}
int set_Vector_g( gsl_vector * v, double dx, const int Dim){
    for(int i=0;i<Dim;i++){
        gsl_vector_set(v,i,integration_g(i*dx,WIDTH,HEIGHT,1));
    }
    return 0;
}
int set_Vector_Cab(gsl_vector * Cab, const gsl_vector * C, const int Dim){
    double d=0.0;
    for(int i=0; i<Dim;i++){
        d =gsl_vector_get(C,i);
        if(d >0.0){
            gsl_vector_set(Cab,i,d);
        }
        else{
            gsl_vector_set(Cab,i,0.0);
        }
    }
    return 0;
}
int get_Intensity_Distribution( gsl_vector * intense,  gsl_vector * a, const double height, const double width, const double dx, const int Dim){
    for(int i=0;i<Dim;i++){
        //gsl_vector_set(intense,i,integration_Width(i*dx-Dim,width, height) + func_If(i*dx-Dim,height,dx,a,Dim));
        gsl_vector_set(intense,i,integration_g(i*dx,width, height,0));
    }
}