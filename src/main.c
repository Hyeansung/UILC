#include "../include/UILC.h"



int main(void)
{
    UILC_Lamber_LED led = {0.0,1};
    double Width = 0.0;
    double D = 0.0;
    unsigned int init_led = 1;
    int min_selector=MIN_GOLDENSECTION;
    int roo_selector=ROO_BISECTION;
    int iter = 1000;
    //

/*
    printf("Enter the Intensity and the Lamber number:");
    scanf("%le %le",&(led.intensity), &(led.m));
    // printf("Enter the line length and target distance:");
    // scanf("%le %le", &Width, &D);
    printf("Enter the initial Morena led number(as many as possible):");
    scanf("%d",&init_led);
    printf("Select the min/root method sep with space, Default is 1,1\n   Root      Minimum\n1: BISECTION GOLDENSECTION\n2: FALSEPOS  BRENT_Min\n3: BRENT_Roo QUADGOLDEN\nExample, Root: FASLEPOS, Min: FOLDENSECTION: \"2 1\"\nenter: ");
    scanf("%d %d", &roo_selector, &min_selector);
    roo_selector = ( (roo_selector==ROO_BISECTION) || (roo_selector==ROO_FALSEPOS) || (roo_selector==ROO_BRENT) ) ? roo_selector: ROO_BISECTION;
    min_selector = ( (min_selector==MIN_GOLDENSECTION)||(min_selector==MIN_BRENT)||(min_selector==MIN_QUADGOLDEN)  ) ? min_selector : MIN_GOLDENSECTION;
    printf("Algorithm iteration (default 1000):");
    scanf("%d",&iter);
    iter = iter <0 ? 1000 : iter;
*/
    led.intensity =1000000;
    led.m =80.7;
    init_led = 7;
    roo_selector =1;
    min_selector =1;
    iter =1000;

    double dm = UILC_f_Morena_getdm_Linear(led,init_led,iter,min_selector,roo_selector,DBL_EPSILON);
    printf("%le \n",dm);

/*-----------------------------------------------------------------------*/
    UILC_LED_Arr arr = UILC_f_Morena_get_Arr(dm,1,init_led);
    //gsl_vector * vec = UILC_f_get_ArrCoordinate(arr,1,1); <-불러오는 과정에서 세그먼트 오류 있음 고쳐야함.
    int i = 1;
    int j = 1;
    int index =3*((i-1)*arr.M+j-1);
    gsl_vector_view v = gsl_vector_subvector(arr.coor,index,3);
    gsl_vector * vec =&(v.vector); 
    printf("x: %le \n y: %le \n z: %le \n", gsl_vector_get(vec,0), gsl_vector_get(vec,1), gsl_vector_get(vec,2));
    printf("%le \n",UILC_f_get_intensity_arr_Lamber_target(arr,led,vec));
    // 미분계수 1인 함수 찾는 과정이 제대로 안됨 확인 필요.
    //double df_dx_1 = UILC_f_find_derivative_Lamber(1.0,0,gsl_vector_get(vec,0)- dm,0.0,0.00001,arr,led);
    //double df_dy_1 = UILC_f_find_derivative_Lamber(1.0,1,gsl_vector_get(vec,1)+ dm,0.0,0.00001,arr,led);
    
    //printf("df/dx=1: %le, df/dy =1: %le \n",df_dx_1,df_dy_1);
    
    //double Morena_Bound = UILC_f_Morena_get_Morena_Boundary(arr,FDM,df_dx_1,df_dy_1);
    double area = UILC_f_get_arr_target_Area(arr,FDM);

    printf("    dm,     Area,   Morena_boundary \n");
    printf("%le, %le, \n", dm, area);

 
 /*  
    int i=0;
    while(i>=0)
    {
        printf("Discrete step:");
        scanf("%d",&i);
        
        
        
    }
    */
   return(0);

}