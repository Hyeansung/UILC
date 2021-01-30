#include "../include/UILC.h"

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
    for(int i =0; i < led.n+1;i++)
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
        fabs(gsl_vector_get(led,2)-gsl_vector_get(target,2))};
    double r= gsl_hypot3(vec[0], vec[1], vec[2]);
    double pr = gsl_hypot(vec[0],vec[1]);
    double theta = atan2(pr,vec[2]);
    return (UILC_f_get_intensity_Lamber(ledmodel,theta) )/gsl_pow_2(r);
}

inline double UILC_f_get_intensity_Poly_target(
    UILC_Poly_LED ledmodel,
    gsl_vector * led,
    gsl_vector * target
)
{
    double vec[3] = {
        fabs(gsl_vector_get(led,0)-gsl_vector_get(target,0)),
        fabs(gsl_vector_get(led,1)-gsl_vector_get(target,1)),
        fabs(gsl_vector_get(led,2)-gsl_vector_get(target,2))};
    double r= gsl_hypot3(vec[0], vec[1], vec[2]);
    double pr = gsl_hypot(vec[0],vec[1]);
    double theta = atan2(pr,vec[2]);
    return (UILC_f_get_intensity_Poly(ledmodel,theta) )/gsl_pow_2(r);
}

//=>LED array function-------------------------------------------
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

gsl_vector UILC_f_get_ArrCoordinate(
    UILC_LED_Arr arr,
    const unsigned int i,
    const unsigned int j
)
{
    int index =3*((i-1)*arr.M+j-1);
    gsl_vector_view vec = gsl_vector_subvector(arr.coor,index,3);
    return(vec.vector);
}

int UILC_f_set_ArrCoordinate(
    UILC_LED_Arr arr,
    const unsigned int i,
    const unsigned int j,
    const gsl_vector * value
)
{

    int index =3*((i-1)*arr.M+j-1);
    gsl_vector_set(arr.coor,index, gsl_vector_get(value,0));
    gsl_vector_set(arr.coor,index+1, gsl_vector_get(value,1));
    gsl_vector_set(arr.coor,index+2, gsl_vector_get(value,2));
    
    return(0);
}

int UILC_f_set_AllArrCoordinate(
    UILC_LED_Arr arr,
    gsl_vector_view (*fill)(unsigned int, unsigned int)
) // Be aware of the 'fill' function form.
{
    int index=0;
    gsl_vector_view vec;
    for(int i=0; i< arr.N; i++ )
    {
        for(int j=0; j < arr.M; j++)
        {
            index =3*((i-1)*arr.M+j-1);
            vec = fill(i,j);
            if(vec.vector.data == NULL){ return(1);}

            gsl_vector_set(arr.coor,index, *(vec.vector.data));
            gsl_vector_set(arr.coor,index+1, *(vec.vector.data+1));
            gsl_vector_set(arr.coor,index+2, *(vec.vector.data+2));
        }
    }
    return(0);
}
double UILC_f_get_intensity_arr_Lamber_target(
    UILC_LED_Arr arr,
    UILC_Lamber_LED led,
    gsl_vector * target
)
{
    double y =0;
    gsl_vector_view vec;
    int index =0;
    for(int i=0;i< arr.N; i++)
    {
        for(int j=0; j< arr.M; j++)
        {
            index =3*((i-1)*arr.M+j-1);
            vec = gsl_vector_subvector(arr.coor,index,3);
            y += UILC_f_get_intensity_Lamber_target(led,&vec.vector, target);
        }
    }
    gsl_vector_free(&vec.vector);
    return(y);
}

double UILC_f_get_intensity_arr_Poly_target(
    UILC_LED_Arr arr,
    UILC_Poly_LED led,
    gsl_vector * target
)
{
    double y =0;
    gsl_vector_view vec;
    int index =0;
    for(int i=0;i< arr.N; i++)
    {
        for(int j=0; j< arr.M; j++)
        {
            index =3*((i-1)*arr.M+j-1);
            vec = gsl_vector_subvector(arr.coor,index,3);
            y += UILC_f_get_intensity_Poly_target(led,&vec.vector, target);
        }
    }
    gsl_vector_free(&vec.vector);
    return(y);
}

double  UILC_f_get_arr_target_Area(
    UILC_LED_Arr arr,
    const int selector
)
{
    double h=0.0;
    double w=0.0;
    double a=0.0;
    double dm=fabs(gsl_vector_get(arr.coor,0)-gsl_vector_get(arr.coor,3));
    switch(selector)
    {
        case BC: // Set the edge LED location as the boundary edge
        w = fabs(gsl_vector_get(arr.coor,0)-gsl_vector_get(arr.coor,arr.M-1)); 
        h = fabs(gsl_vector_get(arr.coor,1)-gsl_vector_get(arr.coor,3*arr.M*(arr.N-1)+1)); 
        break;
        case HDM: // add edge square which side is 1/2 dm
        w= fabs(gsl_vector_get(arr.coor,0)-gsl_vector_get(arr.coor,arr.M-1))+dm; 
        h = fabs(gsl_vector_get(arr.coor,1)-gsl_vector_get(arr.coor,3*arr.M*(arr.N-1)+1))+dm; 
        break;
        case FDM: // add edge square which side is dm
        w= fabs(gsl_vector_get(arr.coor,0)-gsl_vector_get(arr.coor,arr.M-1))+2*dm; 
        h = fabs(gsl_vector_get(arr.coor,1)-gsl_vector_get(arr.coor,3*arr.M*(arr.N-1)+1))+2*dm; 
    }
    a= h*w;
    if(arr.N==1) // Linear case
    { 
        a=w;
    }
    return(a);
}

double UILC_f_find_derivative_Lamber(
    const double df_dx,
    const int axis,
    const double initialpoint, 
    const double endpoint,
    const double step,
    UILC_LED_Arr arr,
    UILC_Lamber_LED led
)
{
    int N = (int)(endpoint - initialpoint) / step;
    double h = step/2;
    double fm1 = 0.0;
    double fp1 = 0.0;
    double fmh = 0.0;
    double fph = 0.0;
    double x =0.0;
    double d=0.0;
    gsl_vector * vec = gsl_vector_calloc(3);

    if(axis == 0)
    {// get x axis case
        for(int i=0; i<N;i++) 
        {   
            x = initialpoint+i*step;
            gsl_vector_set(vec,0, x-h);
            fm1 = UILC_f_get_intensity_arr_Lamber_target(arr,led,vec);
            gsl_vector_set(vec,0, x+h);
            fp1 = UILC_f_get_intensity_arr_Lamber_target(arr,led,vec);
            gsl_vector_set(vec,0, x-h/2);
            fmh = UILC_f_get_intensity_arr_Lamber_target(arr,led,vec);
            gsl_vector_set(vec,0, x+h/2);
            fph = UILC_f_get_intensity_arr_Lamber_target(arr,led,vec);
        
            double result = (4.0/3.0) *(fph - fmh) - (1.0 / 3.0) *(0.5 * (fp1 - fm1));

            if(fabs(result - df_dx) < DBL_EPSILON){
                d = result;
                break;
            }
        }

    }
    else if(axis == 1)
    {// get y axis case
        for(int i=0; i<N;i++) 
        {   
            x = initialpoint+i*step;
            gsl_vector_set(vec,1, x-h);
            fm1 = UILC_f_get_intensity_arr_Lamber_target(arr,led,vec);
            gsl_vector_set(vec,1, x+h);
            fp1 = UILC_f_get_intensity_arr_Lamber_target(arr,led,vec);
            gsl_vector_set(vec,1, x-h/2);
            fmh = UILC_f_get_intensity_arr_Lamber_target(arr,led,vec);
            gsl_vector_set(vec,1, x+h/2);
            fph = UILC_f_get_intensity_arr_Lamber_target(arr,led,vec);
        
            double result = (4.0/3.0) *(fph - fmh) - (1.0 / 3.0) *(0.5 * (fp1 - fm1));

            if(fabs(result - df_dx) < DBL_EPSILON){
                d = result;
                break;
            }
        }
    }

    return(d);
}
double UILC_f_find_derivative_Poly(
    const double df_dx,
    const int axis,
    const double initialpoint, 
    const double endpoint,
    const double step,
    UILC_LED_Arr arr,
    UILC_Poly_LED led
)
{
    int N = (int)(endpoint - initialpoint) / step;
    double h = step/2;
    double fm1 = 0.0;
    double fp1 = 0.0;
    double fmh = 0.0;
    double fph = 0.0;
    double x =0.0;
    double d=0.0;
    gsl_vector * vec = gsl_vector_calloc(3);

    if(axis == 0)
    {// get x axis case
        for(int i=0; i<N;i++) 
        {   
            x = initialpoint+i*step;
            gsl_vector_set(vec,0, x-h);
            fm1 = UILC_f_get_intensity_arr_Poly_target(arr,led,vec);
            gsl_vector_set(vec,0, x+h);
            fp1 = UILC_f_get_intensity_arr_Poly_target(arr,led,vec);
            gsl_vector_set(vec,0, x-h/2);
            fmh = UILC_f_get_intensity_arr_Poly_target(arr,led,vec);
            gsl_vector_set(vec,0, x+h/2);
            fph = UILC_f_get_intensity_arr_Poly_target(arr,led,vec);
        
            double result = (4.0/3.0) *(fph - fmh) - (1.0 / 3.0) *(0.5 * (fp1 - fm1));

            if(fabs(result - df_dx) < DBL_EPSILON){
                d = result;
                break;
            }
        }

    }
    else if(axis == 1)
    {// get y axis case
        for(int i=0; i<N;i++) 
        {   
            x = initialpoint+i*step;
            gsl_vector_set(vec,1, x-h);
            fm1 = UILC_f_get_intensity_arr_Poly_target(arr,led,vec);
            gsl_vector_set(vec,1, x+h);
            fp1 = UILC_f_get_intensity_arr_Poly_target(arr,led,vec);
            gsl_vector_set(vec,1, x-h/2);
            fmh = UILC_f_get_intensity_arr_Poly_target(arr,led,vec);
            gsl_vector_set(vec,1, x+h/2);
            fph = UILC_f_get_intensity_arr_Poly_target(arr,led,vec);
        
            double result = (4.0/3.0) *(fph - fmh) - (1.0 / 3.0) *(0.5 * (fp1 - fm1));

            if(fabs(result - df_dx) < DBL_EPSILON){
                d = result;
                break;
            }
        }
    }

    return(d);
}