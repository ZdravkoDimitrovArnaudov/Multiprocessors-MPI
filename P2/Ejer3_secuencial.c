#include <stdio.h>
#include <math.h>

int main()  {
  float a, b, h, x, integral;
  int n, i;

  /* definicion de la funcion */
 float f(float x){ 
  return(cos(x)*exp(sin(x))); 
 }

  printf("Mete a ,b, n \n");
  scanf("%f %f %d", &a, &b, &n);

 
  h = (b-a)/n;
  integral = (f(a)+f(b))/2.0;  // los extremos 

  for(i=1,x=a; i<=n-1; i++)
  {
    x = x + h;
    integral = integral +f(x);
  }
  integral = integral * h;
  printf("Con %d trapezoides la estimacion es de %f\n", n, integral);
 
}
