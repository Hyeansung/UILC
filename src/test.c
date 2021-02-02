#include "../include/UILC.h"

#define CLEARINBUFFER while(getchar() != '\n');

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
    init_led = 4;
    roo_selector =1;
    min_selector =1;
    iter =1000;

    

    while(init_led>0){
        double dm = UILC_f_Morena_getdm_Linear(led,init_led,iter,min_selector,roo_selector,DBL_EPSILON);
        printf("\n\ndm: %le ",dm);
/*-----------------------------------------------------------------------*/
        UILC_LED_Arr arr = UILC_f_Morena_get_Arr(dm,1,init_led);
        //gsl_vector * vec = UILC_f_get_ArrCoordinate(arr,1,1); <-불러오는 과정에서 세그먼트 오류 있음 고쳐야함.
        int i = 1;
        int j = 1;
        int index =3*((i-1)*arr.M+j-1);
        gsl_vector_view v = gsl_vector_subvector(arr.coor,index,3); //이건 새로운 벡터를 만드는게 아니라 그냥 원래 백터에서 index 부터 + strid 까지를 가르킨다고ㅓ 주소만 가져오는듯
        gsl_vector * ve = gsl_vector_calloc(3);
        gsl_vector_memcpy(ve, &v.vector);
        gsl_vector_set(ve,2,0.5);
        gsl_vector_set(ve,1,0.0);
//-----------------------------------------
        printf("Intensity: %le\n\n",UILC_f_get_intensity_arr_Lamber_target(arr,led,ve) );
        
        double area = UILC_f_get_arr_target_Area(arr,HDM);
        double morena = fabs( 2*UILC_f_find_derivative_Lamber(1.0, 1, gsl_vector_get(arr.coor,1)-1,0.0, 0.00001,arr,led,0.03) );
        printf("    dm,     Area,   Morena_boundary, percentage \n");
        printf("%le, %le, %le, %le\n", dm, area, 2.720578e+00, 100*(area-morena)/area);
        
        printf("set led number:");
        CLEARINBUFFER
        scanf("%d ",&init_led);
    }
/*
    gsl_vector* vec = gsl_vector_calloc(3); // location of each led
    double y =0.0;
     index =0;
    for(int i=0;i< arr.N; i++)
    {
        for(int j=0; j< arr.M; j++)
        {
            index =3*((i)*arr.M+j);
            gsl_vector_set(vec,0,gsl_vector_get(arr.coor,index+0));
            gsl_vector_set(vec,1,gsl_vector_get(arr.coor,index+1));
            gsl_vector_set(vec,2,gsl_vector_get(arr.coor,index+2));

            double vv[3] = {
                gsl_vector_get(vec,0),
                gsl_vector_get(vec,1),
                gsl_vector_get(vec,2)};
            double vvv[3] = {
                gsl_vector_get(ve,0),
                gsl_vector_get(ve,1),
                gsl_vector_get(ve,2)
            };   

            printf("(x,y,z) \n1: (%le, %le, %le)\n2: (%le, %le, %le)\n",
                vv[0],vv[1],vv[2],
                vvv[0], vvv[1],vvv[2]);

            double vect[3] = {
                (gsl_vector_get(vec,0)-gsl_vector_get(ve,0)),
                (gsl_vector_get(vec,1)-gsl_vector_get(ve,1)),
                (gsl_vector_get(vec,2)-gsl_vector_get(ve,2))};

            printf("Vect[i]: %le %le %le\n", vect[0], vect[1], vect[2]);
            double r= gsl_hypot3(vect[0], vect[1], vect[2]);
            double pr = gsl_hypot(vect[0],vect[1]);
            double theta = atan2(pr,fabs(vect[2]));
            
            double p= pow(cos(theta),led.m);
            double r2 = gsl_pow_2(r);
            y += led.intensity * p/r2;
            printf("inf test \n p = %le\n r^2 = %le\n", p, r2);
            printf("LED: (%le,%le,%le) Target: (%le,%le,%le) y= %le \n",
                    gsl_vector_get(vec,0),
                    gsl_vector_get(vec,1),
                    gsl_vector_get(vec,2),
                    gsl_vector_get(ve,0),
                    gsl_vector_get(ve,1),
                    gsl_vector_get(ve,2),
                    y
                    );
        }
        
    }
    gsl_vector_free(vec);
*/
    //printf("%le \n",y);
    // 미분계수 1인 함수 찾는 과정이 제대로 안됨 확인 필요.
    //double df_dx_1 = UILC_f_find_derivative_Lamber(1.0,0,gsl_vector_get(vec,0)- dm,0.0,0.00001,arr,led);
    //double df_dy_1 = UILC_f_find_derivative_Lamber(1.0,1,gsl_vector_get(vec,1)+ dm,0.0,0.00001,arr,led);
    
    //printf("df/dx=1: %le, df/dy =1: %le \n",df_dx_1,df_dy_1);
    
    //double Morena_Bound = UILC_f_Morena_get_Morena_Boundary(arr,FDM,df_dx_1,df_dy_1);


 
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