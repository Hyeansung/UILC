#include "UILC.h"

inline double UILC_f_get_intensity_Lamber(
    UILC_Lamber_LED led, 
    const double theta
)
{
    return led.intensity * pow(cos(theta),led.m);
}

inline double UILC_f_get_intensity_Poly(
    UILC_Poly_LED led, 
    const double theta
)
{
    double y=0.0;
    for(int i =0, i < led.n+1;i++)
    {
        y+= *(led.param+i)*pow(theta,i);
    }
}

inline double UILC_f_get_intensity_Lamber_target(
    UILC_Lamber_LED ledmodel,
    gsl_vector * led,
    gsl_vector * target
)
{
    double vec[3] = {
        fabs(gsl_vector_get(led,0)-gsl_vector_get(target,0)),
        fabs(gsl_vector_get(led,1)-gsl_vector_get(target,1)),
        fabs(gsl_vector_get(led,2)-gsl_vector_get(target,2))}
    double r=gsl_hypot3(vec[0], vec[1], vec[2]);
    double pr = gsl_hypo2(vec[0],vec[1]);
    double theta = atan2(pr,vecp[2]);
    return (UILC_f_get_intensity_Lambe(ledmodel,theta) )/gsl_pow_2(r);
}

inline double UILC_f_get_intensity_Poly_target(
    UILC_Poly_LED ledmodel,
    gsl_vector * led,
    gsl_vector * target
){
    double vec[3] = {
        fabs(gsl_vector_get(led,0)-gsl_vector_get(target,0)),
        fabs(gsl_vector_get(led,1)-gsl_vector_get(target,1)),
        fabs(gsl_vector_get(led,2)-gsl_vector_get(target,2))}
    double r=gsl_hypot3(vec[0], vec[1], vec[2]);
    double pr = gsl_hypo2(vec[0],vec[1]);
    double theta = atan2(pr,vecp[2]);
    return (UILC_f_get_intensity_Poly(ledmodel,theta) )/gsl_pow_2(r);
}

//=>LED array function
UILC_LED_Arr UILC_f_Arr_alloc(
    const unsigned int N, 
    const unsigned int M
)
{
    UILC_LED_Arr arr = {N,M, gsl_vector_alloc(3*N*M)};
    return(arr);
}

int UILC_f_Arr_free(
    UILC_LED_Arr arr
)
{
    gsl_vector_free(arr.coor);
    return(0);
}

gsl_vector * UILC_f_get_ArrCoordinate(
    UILC_LED_Arr arr,
    const unsigned int i,
    const unsigned int j
)
{
    gsl_vector *v = gsl_vector_calloc(3);
    int index =;
    gsl_vector_set(v,0, gsl_vector_get(arr.coor,index));
    gsl_vector_set(v,1, gsl_vector_get(arr.coor,index+1));
    gsl_vector_set(v,2, gsl_vector_get(arr.coor,index+2));

    return v;
}
int UILC_f_set_ArrCoordinate(
    UILC_LED_Arr arr,
    const unsigned int i,
    const unsigned int j,
    const gsl_vector * value
){
    int index =;
    gsl_vector_set(arr.coor,index, gsl_vector_get(value,0));
    gsl_vector_set(arr.coor,index+1, gsl_vector_get(value,1));
    gsl_vector_set(arr.coor,index+2, gsl_vector_get(value,2));
    
    return(0);
}

int UILC_f_set_AllArrCoordinate(
    UILC_LED_Arr arr,
    double * (*fill)(unsigned int, unsigned int)
) // Be aware of the 'fill' function form.
{
    int index=0;
    double * data =NULL;

    for(int i=0; i< arr.N; i++ )
    {
        for(int j=0; j < arr.M; j++)
        {
            index=;
            data = fill(i,j)
            if(data == NULL){ return 1;}

            gsl_vector_set(arr.coor,index, *(data+0));
            gsl_vector_set(arr.coor,index+1, *(data+1));
            gsl_vector_set(arr.coor,index+2, *(data+2));
        }
    }
    return 0;
}
double UILC_f_get_intensity_arr_Lamber_target(
    UILC_LED_Arr arr,
    UILC_Lamber_LED led,
    gsl_vector * target
);
double UILC_f_get_intensity_arr_Poly_target(
    UILC_LED_Arr arr,
    UILC_Poly_LED led,
    gsl_vector * target
);

double UILC_f_get_arr_target_Area(
    UILC_LED_Arr arr
);