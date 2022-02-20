/* A cada proceso se le da un conjunto de trapezoides n/p */

#include <stdio.h>
#include "mpi.h"

int main(int argc, char** argv)  
{
  float a=0.0, b=10.0, h, x, integral;
  int n=1000, i;
  int yo, p, origen, rodaja, destino=0, tag=0;
  float inicio, fin;
  float total = 0.0;
  MPI_Status status;

  float trapez (float inicio, float fin, int rodaja, float h);

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &yo);
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  
  //antes de la llamada
  printf ("Soy el proceso: %d. Tengo A: %f, B: %f y N: %d\n", yo, a, b, n);
  
  if (yo ==0){
    printf("Mete a , b, n \n");
    scanf("%f %f %d", &a, &b, &n);
  }
  
  MPI_Bcast(&a, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&b, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
  
  //despues de la llamada
  printf ("Soy el proceso: %d. Tengo A: %f, B: %f y N: %d\n", yo, a, b, n);


// Hay que poner una barrera si no los otros procesos siguen con valores sin inicializar en el scanf

  h = (b-a)/n;
  rodaja = n/p;
  inicio = a + yo*rodaja*h;
  fin = inicio + rodaja * h;
  integral = trapez(inicio, fin, rodaja, h);

  printf("Soy %d inicio y fin son %f - %f integral ini es %f h %f \n", yo, inicio, fin, integral,h);

  if (yo ==0)
  {
     total = integral;
     for(origen =1;origen < p; origen++) 
     {
        MPI_Recv(&integral, 1, MPI_FLOAT, origen, tag, MPI_COMM_WORLD, &status);
        total += integral;
     }
     printf("Con %d trapezoides la estimacion es de %f\n",n,total);
  } else
     MPI_Send(&integral, 1, MPI_FLOAT, destino, tag, MPI_COMM_WORLD);

 MPI_Finalize();
}


  /* definicion de la funcion */
  float f(float x)
  { return(x); }


float trapez (float inicio, float fin, int rodaja, float h)
{
  float integral, x, f(float x);
  int i;
  integral = (f(inicio)+f(fin))/2.0;
  for(x=inicio,i=1;i<=rodaja-1;i++)
  {
    x += h;
    integral += f(x);
  }
  integral = integral * h;
  return integral;
}

