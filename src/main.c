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

    double dm = UILC_f_Morena_getdm_Linear(led,init_led,iter,min_selector,roo_selector,DBL_EPSILON);
    printf("%le \n",dm);

/*-----------------------------------------------------------------------*/
    //UILC_LED_Arr arr = UILC_f_Morena_get_Arr(dm,1,init_led);
    int N = 1;
    int M = init_led;
    gsl_vector * arr = gsl_vector_calloc( N * M *3);

    for(int i=0; i<N; i++)
    {
        for(int j=0; j<M ; j++)
        {
            
            gsl_vector_set(arr,i*3 + 3*j+0, (i-(N-1)/2)*dm) ;
            gsl_vector_set(arr,i*3 + 3*j+1, (j-(M-1)/2)*dm) ;
            gsl_vector_set(arr,i*3 + 3*j+2, 0.0);
            printf("%d, %d ,%d\n",i*3 + j+0, i*3 + j+1, i*3 + j+2 );
            printf("x:%le y:%le z:%le\n",(i-(N-1)/2)*dm,(j-(M-1)/2)*dm,0.0 );
            printf("x:%le y:%le z:%le\n", gsl_vector_get(arr,0),gsl_vector_get(arr,1),gsl_vector_get(arr,2));
            //printf("x:%le y:%le z:%le\n", gsl_vector_get(arr,i*3 + j+0),gsl_vector_get(arr,i*3 + j+1),gsl_vector_get(arr,i*3 + j+2));
        }
    }

    for(int i =0; i< 3*M;i++){
        printf("%d: %le \n", i,gsl_vector_get(arr,i) );

    }

    
/*-----------------------------------------------------------------------*/
    /*
    double df_dx_1 = UILC_f_find_derivative_Lamber(1.0,0,gsl_vector_get(vec,0)- dm,0.0,0.00001,arr,led);
    double df_dy_1 = UILC_f_find_derivative_Lamber(1.0,1,gsl_vector_get(vec,1)+ dm,0.0,0.00001,arr,led);
    
    double Morena_Bound = UILC_f_Morena_get_Morena_Boundary(arr,FDM,df_dx_1,df_dy_1);
    double area = UILC_f_get_arr_target_Area(arr,FDM);

    printf("    dm,     Area,   Morena_boundary \n");
    printf("%le, %le, %le \n", dm, area, Morena_Bound);
    */

 
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