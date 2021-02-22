#include "../include/UILC.h"


/*
Pearson, J. W., Olver, S., & Porter, M. A. (2017). 
Numerical methods for the computation of the confluent and Gauss hypergeometric functions. 
Numerical Algorithms, 74(3), 821-866.

Transformation method

1.  -infty < z < -1     w = 1/(1-z) 
2.  -1 <= z < 0         w = z/(z-1)
3.  0 <= z <= 1/2       w = z
4.  1/2 <z <= 1         w = 1-z
5.  1 < z <= 2          w= 1-1/z
6.  2 < z < infty       w = 1/z
*/

inline double hyperg_2F1_aR(double a, double b, double c, double x)
{

}


1.0939790288090513083575272074666505603058911596320336858307124921
 