#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"
#define N 23
#define DEBUG

/**
* Si N es múltiplo del número de procesos, entonces todos tendrán el mismo
* número de filas a computar. Si N no es múltiplo, entonces le asignaremos al 
* último proceso el resto que falta.
*
* Se ha decidido inicializar cada componente del vector con su propia posición. 
* De esta forma, podemos incluso simular la multiplicación sin definir los propios 
* vectores, es un sumatorio de numeros al cuadrado.
*
* Para ver los mensajes de depuración, descomentar DEBUG.
*
**/

int main(int argc, char** argv)  
{
     
    int i;
    int yo, p;
    float suma_parcial = 0.0;
    float suma_total = 0.0;
    int porcion_filas ; //numero de filas que cada proceso deberá computar
    int fila_proceso ; //primera fila de cada vector 
    MPI_Status status;
    
    //Inicio zona paralela
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &yo);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    
   
    if (N % p == 0){ //todos tienen la misma porción de trabajo.
  	porcion_filas = N/p;
  	fila_proceso = porcion_filas * yo;
  	
    } else { //el último proceso calcula el resto, los demás usan la parte entera
    	porcion_filas = (int) N/p; 
    	
    	if (yo == p-1){ //ultimo proceso
    		fila_proceso = porcion_filas * yo; //primera fila asignada
    		porcion_filas = N - fila_proceso; //porcion + resto
    	
    	} else {
    		fila_proceso = porcion_filas * yo;
    	
    	}
    	
    }
    
    #ifdef DEBUG
    	printf ("P%d: Filas %d - %d\n", yo, fila_proceso, fila_proceso+porcion_filas);
    	MPI_Barrier (MPI_COMM_WORLD); // para impirmir mejor los mensajes.
    #endif
    
     
    //inicialización y multiplicación
    for (i = 0; i < porcion_filas; i++){
    	suma_parcial += powf(fila_proceso+i, 2.0);
    }
    
    #ifdef DEBUG
    	printf ("Soy P%d, suma_parcial : %.2f\n", yo, suma_parcial);
    #endif
    
    //El proceso raiz hace reducción con todos los resultados
    MPI_Reduce (&suma_parcial, &suma_total, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if (yo == 0){
    	printf ("Resultado: %.2f\n", suma_total);
    }
    
    MPI_Finalize();
    
    return 0;
}  /* main */
