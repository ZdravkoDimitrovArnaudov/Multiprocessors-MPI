/* A cada proceso se le da un conjunto de trapezoides n/p */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "mpi.h"

#define DEBUG

/*Cuanto mayor, mas decimales y precision*/
#define ACCUR 10000000

int main(int argc, char** argv)  {
  int i;
  int yo, p;
  float inicio, fin;
  float PI; //aproximación de Pi
  float p_y, p_x;
  float dist = 0.0;
  int n; //Nº de puntos aleatorios para cada proceso 
  int puntos_dentro = 0;
  int total_puntos_dentro = 0;
  MPI_Status status;
  srand(clock()); //inicializamos seed numeros aleatorios
  
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &yo);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  
  
  //proceso raiz recibe dimension N y lo transmite al resto de procesos
  if (yo == 0){
    printf("Introduce dimensión de N aleatorios para cada proceso:\n");
    scanf("%d",&n);
  }
  
  //broadcast
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
  
  //Una vez los procesos sepan cual es la dimensión de puntos, operan
  for (i = 0; i < n; i++){
  	//aleatorios entre 0 y 1. 
  	p_x = (float)(rand() % (ACCUR + 1)) / ACCUR; 
  	p_y = (float)(rand() % (ACCUR + 1)) / ACCUR;
  		
  	//Pitagoras: X² + y²<1
  	if ((pow(p_x, 2) + pow(p_y, 2)) <= 1){
  		puntos_dentro++;
  	}  	
  }
  
  #ifdef DEBUG
  	printf ("P%d: puntos dentro: %d\n", yo, puntos_dentro);
  	MPI_Barrier (MPI_COMM_WORLD);
  #endif

  
  //cada proceso, le envía el número de puntos dentro del cuadrante obtenidos al master
  MPI_Reduce (&puntos_dentro, &total_puntos_dentro, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  
  //el proceso padre, calcula la estimación e imprime por pantalla
  if (yo == 0){
  	PI = (float)total_puntos_dentro / n;
  	printf ("PI: %.4f\n",PI);
  }
  	

  
 MPI_Finalize();
}



