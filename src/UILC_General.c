#include "../include/UILC.h"



/*-------------------------------------------------*/
typedef struct{
    UILC_LED_Arr arr;
    UILC_Lamber_LED led;
    double height;
    int axis;

} UILC_fdf_Lamber_param;

typedef struct{
    double df_dx;
    double height;
    double h;
    int axis;
    UILC_LED_Arr arr;
    UILC_Lamber_LED led;
    

} UILC_df_Lamber_param;


/*-------------------------------------------------*/
extern inline double UILC_f_get_intensity_Lamber(
    UILC_Lamber_LED led, 
    const double theta
)
{
    return led.intensity * pow(cos(theta),led.m);
}

extern inline double UILC_f_get_intensity_Poly(
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

extern inline double UILC_f_get_intensity_Lamber_target(
    UILC_Lamber_LED ledmodel,
    gsl_vector * led,
    gsl_vector * target
)
{
    double vec[3] = {
        gsl_vector_get(led,0)-gsl_vector_get(target,0),
        gsl_vector_get(led,1)-gsl_vector_get(target,1),
        gsl_vector_get(led,2)-gsl_vector_get(target,2)};
    double r= gsl_hypot3(vec[0], vec[1], vec[2]);
    double pr = gsl_hypot(vec[0],vec[1]);
    double theta = atan2(pr,fabs(vec[2]));
    return (UILC_f_get_intensity_Lamber(ledmodel,theta) )/gsl_pow_2(r);
}

extern inline double UILC_f_get_intensity_Poly_target(
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

gsl_vector * UILC_f_get_ArrCoordinate(
    UILC_LED_Arr arr,
    const unsigned int i,
    const unsigned int j
)
{
    int index =3*((i-1)*arr.M+j-1);
    gsl_vector_view vec = gsl_vector_subvector(arr.coor,index,3);

    return(&vec.vector);
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
extern inline double UILC_f_get_intensity_arr_Lamber_target(
    UILC_LED_Arr arr,
    UILC_Lamber_LED led,
    gsl_vector * target
)
{
    gsl_vector * vec = gsl_vector_calloc(3);
    double y =0.0;
    int index =0;
    for(int i=0;i< arr.N; i++)
    {
        for(int j=0; j< arr.M; j++)
        {
            index =3*((i)*arr.M+j);
            gsl_vector_set(vec,0,gsl_vector_get(arr.coor,index+0));
            gsl_vector_set(vec,1,gsl_vector_get(arr.coor,index+1));
            gsl_vector_set(vec,2,gsl_vector_get(arr.coor,index+2));

            y += UILC_f_get_intensity_Lamber_target(led,vec, target);
            /*
            printf("LED: (%le,%le,%le) Target: (%le,%le,%le) y= %le \n",
                    gsl_vector_get(vec,0),
                    gsl_vector_get(vec,1),
                    gsl_vector_get(vec,2),
                    gsl_vector_get(target,0),
                    gsl_vector_get(target,1), 
                    gsl_vector_get(target,2),
                    y
                    );
            */
        }
        
    }
    gsl_vector_free(vec);

    
    return(y);
}

extern inline double UILC_f_get_intensity_arr_Poly_target(
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
    double h0 = gsl_vector_get(arr.coor,3*arr.N*(arr.M-1)+1);
    double h1 = gsl_vector_get(arr.coor,1);
    double h=fabs(h0-h1); 
    //double h=gsl_vector_get(arr.coor,1); 
    //double w=fabs(gsl_vector_get(arr.coor,0)-gsl_vector_get(arr.coor,3*(arr.M-1)));
    double w=fabs(gsl_vector_get(arr.coor,0));
    double dm=fabs( gsl_vector_get(arr.coor,1)-gsl_vector_get(arr.coor,4) );
    w = 1.0;

    if(fabs(h -0.0) < DBL_EPSILON){
        h =1.0;
    }
    if(fabs(w -0.0) < DBL_EPSILON){
        w = 1.0;
    }

    printf("%d \n h0: %le h1: %le h: %le w: %le dm: %le\n",3*arr.M*(arr.N-1)+1,h0,h1, h , w ,dm);
    switch(selector)
    {
        case BC: // Set the edge LED location as the boundary edge
        return(h*w);
        break;
        case HDM: // add edge square which side is 1/2 dm
        return((h+dm/2)*(w+dm/2));
        break;
        case FDM: // add edge square which side is dm
        return((h+dm)*(w+dm));
    }

    
}
/*-------------------------------------------------*/
inline double UILC_fdf_get_intensity_arr_Lamber_target(double x, void * param){
    UILC_fdf_Lamber_param * p = (UILC_fdf_Lamber_param *) param;
    gsl_vector * vec = gsl_vector_calloc(3);
    gsl_vector_set(vec,2,p->height);
    if(p->axis ==1){
        gsl_vector_set(vec,1,x);
    }
    else{
        gsl_vector_set(vec,0,x);
    }
    double  result = UILC_f_get_intensity_arr_Lamber_target(p->arr,p->led,vec);
    
    gsl_vector_free(vec);
    return(result);

}

inline double UILC_df_get_intensity_arr_Lamber_target(double x, void * param){
    UILC_df_Lamber_param * p = (UILC_df_Lamber_param*)param;
    UILC_fdf_Lamber_param params = {p->arr,p->led,p->height,p->axis};
    gsl_function F;
    F.function = &UILC_fdf_get_intensity_arr_Lamber_target;
    F.param = &params;
    double result = 0.0;
    double abserr = 0.0;
    gsl_deriv_central(&F,x, p->h, &result,&abserr);
    return(p->df_dx - result);
}




extern inline double UILC_f_find_derivative_Lamber(
    const double df_dx,
    const int axis,
    const double instep,
    const double precison,
    UILC_LED_Arr arr,
    UILC_Lamber_LED led,
    const unsigned int root_selector,
    const double height
)
{

    double l_x_lower = 0.0;//arr에 따라서 바꾸어야함
    double l_x_upper = 0.0; //<- max 주어진 기울기에 따라 좀 바꾸어야 함 1보다 작으면 0.0이 의미 있지만 1보다 크면 전체 길이의 0.5로 해도 괜찮
    double initial_x = (l_x_upper-l_x_lower)/4 +l_x_lower ;
    int status =0;
    int iter =0, max_iter = itetnum;
    UILC_df_Lamber_param l_param = {df_dx,height,instep,axis,arr,led};
    gsl_function F;
    F.function = &UILC_df_get_intensity_arr_Lamber_target;
    F.params = &l_params;

    const gsl_root_fsolver_type * T = gsl_root_fsolver_bisection;
    switch(root_selector)
        {
            case 1: break;
            case 2: T = gsl_root_fsolver_falsepos; break;
            case 3: T = gsl_root_fsolver_brent; break;
        }
    gsl_root_fsolver * s = gsl_root_fsolver_alloc(T);
    gsl_root_fsolver_set(s,&F,l_x_lower,l_x_upper);
    do
    {
        iter++;
        status = gsl_root_fsolver_iterate (s);
        initial_x = gsl_root_fsolver_root (s);
        l_x_lower = gsl_root_fsolver_x_lower (s);
        l_x_upper = gsl_root_fsolver_x_upper (s);
        status = gsl_root_test_interval(l_x_lower, l_x_upper, 0, precison );
    }
    while(status == GSL_CONTINUE && iter < max_iter);
    gsl_root_fsolver_free(s);

    return(initial_x);

}



/*-------------------------------------------------*/
/*
extern inline double UILC_f_find_derivative_Lamber(
    const double df_dx,
    const int axis,
    const double initialpoint, 
    const double endpoint,
    double step,
    UILC_LED_Arr arr,
    UILC_Lamber_LED led,
    const double height
)
{
    int N = (int)fabs(endpoint - initialpoint) / step;
    double h = step/2;
    step = (endpoint - initialpoint)< 0.0 ? -step: step;
    double fm1 = 0.0;
    double fp1 = 0.0;
    double fmh = 0.0;
    double fph = 0.0;
    double x =0.0;
    double d=0.0;
    gsl_vector * vec = gsl_vector_calloc(3);
    gsl_vector_set(vec,2, height);
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

            if(result >df_dx){
                d = x;
                break;
            }
        }

    }
    else if(axis == 1)
    {// get y axis case
        for(int i=0; i<N;i++) 
        {   
            
            x = initialpoint+i*step;
            printf("%d, %le \n",i+1,x);
            gsl_vector_set(vec,1, x-h);
            fm1 = UILC_f_get_intensity_arr_Lamber_target(arr,led,vec);
            gsl_vector_set(vec,1, x+h);
            fp1 = UILC_f_get_intensity_arr_Lamber_target(arr,led,vec);
            gsl_vector_set(vec,1, x-h/2);
            fmh = UILC_f_get_intensity_arr_Lamber_target(arr,led,vec);
            gsl_vector_set(vec,1, x+h/2);
            fph = UILC_f_get_intensity_arr_Lamber_target(arr,led,vec);
            printf("fm1: %le\nfp1: %le\nfmh: %le\nfph: %le\n", fm1,fp1,fmh,fph);
            double result = (4.0/3.0) *(fph - fmh) - (1.0 / 3.0) *(0.5 * (fp1 - fm1));
            printf("result: %le\n",result);
            if(result >df_dx){
                d = x;
                break;
            }
        }
    }

    return(d);
}
*/
inline double UILC_f_find_derivative_Poly(
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