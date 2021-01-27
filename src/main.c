#include "UILC.h"
#include <stdio.h>
#include <float.h>



int main(void)
{
    UILC_LamberLED led = {0.0,1}
    double Width = 0.0;
    double D = 0.0;
    unsigned int init_led = 1;
    int min_selector=MIN_GOLDENSECTION;
    int roo_selector=ROO_BISECTION;
    int iter = 1000;
    //

    printf("Enter the Intensity and the Lamber number:");
    scanf("%e %d",&(led.intensity), &(led.m));
    printf("Enter the line length and target distance:");
    scanf("%e %e", &Width,&D);
    printf("Enter the initial Morena led number(as many as possible):");
    scanf("%d",&init_led);
    printf("Select the min/root method sep with space, Default is 1,1\n   Root      Minimum\n1: BISECTION GOLDENSECTION\n2: FALSEPOS  BRENT_Min\n3: BRENT_Roo QUADGOLDEN\nExample, Root: FASLEPOS, Min: FOLDENSECTION: \"2 1\"\nenter: ");
    scanf("%d %d", &roo_selector, &min_selector);
    roo_selector = ( (roo_selector==ROO_BISECTION) || (roo_selector==ROO_FALSEPOS) || (roo_selector==ROO_BRENT) ) ? roo_selector: ROO_BISECTION;
    min_selector = ( (min_selector==MIN_GOLDENSECTION)||(min_selector==MIN_BRENT)||(min_selector==MIN_QUADGOLDEN)  ) ? min_selector : MIN_GOLDENSECTION;
    printf("Algorithm iteration (default 1000):");
    scanf("%d",&iter);
    iter = iter <0 ? 1000 : iter;


    int dm = UILC_f_MOrena_getdm_Linear();

    int i=0;
    while(i>=0)
    {
        printf("Discrete step:");
        scanf("%d",&i);

        
        
    }



}