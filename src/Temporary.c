double UILC_fdf_get_intensity_arr_Lamber_target(double x, void * param){
    gsl_vector * vec = gsl_vector_calloc(3);

    if(param->selector ==1){

    }
    else{
        
    }
    gsl_vector_set(vec,2,param->height);

}




extern inline double UILC_f_find_derivative_Lamber(
    const double df_dx,
    const int axis,
    const double instep,
    const double precison,
    UILC_LED_Arr arr,
    UILC_Lamber_LED led,
    const double height
)
{
    gsl_function F;
    Param = {arr, }
    F.function = &UILC_fdf_get_intensity_arr_Lamber_target;
    F.param = &params;
    gsl_deriv_central(&F, x, precison, &result, &abserr);

}