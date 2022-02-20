/* A cada proceso se le da un conjunto de trapezoides n/p */

#include <stdio.h>
#include "mpi.h"
#include <math.h>

#define A 0
#define B 1000000000000000
#define N 1000000000

int main(int argc, char** argv)
{
  float a=0.0, h, x, integral;
  float b = 10.0;
  long int n=1000;
  int i;
  int yo, p, origen = 0., rodaja, destino=0, tag=0;
  float metricas[3];
  float metricas_r[3];
  float inicio , fin ;
  float total = 0.0;
  int res;
  MPI_Status status;
  double t1, t2;

  float trapez (float inicio, float fin, int rodaja, float h);

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &yo);
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  if (yo ==0) {
    //printf("Mete a ,b,n \n");
        //scanf("%f %f %d", &a, &b, &n);
        a = A;
        b = B;
        n = N;
    metricas[0] = a;
    metricas[1] = b;
    metricas[2] = n;

    //mandamos métricas al resto de procesos
    for (int i =1; i < p; i++ ){ // limites??
        MPI_Send(&metricas, 3, MPI_FLOAT, i, yo, MPI_COMM_WORLD);
    }
   }


   //Hay que poner una barrera si no los otros procesos siguen con valores sin inicializar en el scanf
  res = MPI_Barrier (MPI_COMM_WORLD);
  if (res != MPI_SUCCESS){
    fprintf (stderr, "MPI_Barrier failed.\n");
    return -1;
  }



  if (yo ==0) { //PROCES RECEPTOR

     t1 = MPI_Wtime(); //tiempo de inicio

     //asignamos valor
     h = (b-a)/n;
     rodaja = n/p;
     inicio = a + yo*rodaja*h;
     fin = inicio + rodaja * h;

     integral = trapez(inicio, fin, rodaja, h); //p0 calcula su parte

     printf("Soy %d inicio y fin son %f - %f integral ini es %f h %f \n", yo, inicio, fin, integral,h);

     total = integral;
     for(origen =1; origen < p; origen++)
     {
        MPI_Recv(&integral, 1, MPI_FLOAT, origen, origen, MPI_COMM_WORLD, &status);
        total += integral;
     }
     t2 = MPI_Wtime(); //tiempo final
     printf("Con %ld trapezoides la estimacion es de %f\n",n,total);
     printf ("El programa ha durado %f segundos\n", t2-t1);



  } else { //PROCESO EMISOR


    MPI_Recv(&metricas_r, 3, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &status); //recibimos métricas

    //asociamos las metricas
    a = metricas_r[0];
    b = metricas_r[1];
    n = metricas_r[2];

    printf ("A: %f, B: %f, N:%ld\n", a,b,n);

    //les asignamos valor
    h = (b-a)/n;
    rodaja = n/p;
    inicio = a + yo*rodaja*h;
    fin = inicio + rodaja * h;
    integral = trapez(inicio, fin, rodaja, h);

    printf("Soy %d inicio y fin son %f - %f integral ini es %f h %f \n", yo, inicio, fin, integral,h);

        MPI_Send(&integral, 1, MPI_FLOAT, 0, yo, MPI_COMM_WORLD); //mandamos los resultados
     }

 MPI_Finalize();
}


  /* definicion de la funcion */
  float f(float x){
  return(cos(x)*exp(sin(x)));
 }


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

