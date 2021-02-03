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