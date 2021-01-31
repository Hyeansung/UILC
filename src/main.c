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

    // Tempo test code for odd number miniization.
    while(init_led !=-1)
    {

        UILC_fparams_linear l_params = {led.m,init_led};
        double min =UILC_f_Morena_Linear(0.0,&l_params);
        double itere =0.0;
        double k = 0;
        for(int i=0; i< 10000; i++)
        {   
            itere = UILC_f_Morena_Linear(0.0+i*0.0001,&l_params);
            if(min > itere){
                min = itere;
                k = 0.0+i*0.0001;
            }

        }

        printf("    x_low      f(x_low)\n");
        printf("%le %le\n",k,min );

        printf("Enter the initial Morena led number(as many as possible):");
        scanf("%d",&init_led);
    }


/*
    while(init_led !=-1){
    printf("Get dm \n")
    double dm = UILC_f_Morena_getdm_Linear(led,init_led,iter,min_selector,roo_selector,DBL_EPSILON);

    printf("Distance: %lf \n",dm);
    printf("Enter the initial Morena led number(as many as possible):");
    scanf("%d",&init_led);
    }
    
    int i=0;
    while(i>=0)
    {
        printf("Discrete step:");
        scanf("%d",&i);
        
        
        
    }
    */
   return(0);

}