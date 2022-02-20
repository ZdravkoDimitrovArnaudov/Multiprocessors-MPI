#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"	

/**
* Planteamiento:
* El objetivo es multiplicar una matriz por un vector y paralelizar la operación.
* A cada proceso, se le asignarán uno o varios paneles de la matriz, así como del vector si existen mas columnas que procesos.
* A continuación, detallaremos el flujo que debe seguir el programa.
*
* Para empezar, deben conocerse las dimensiones de la matriz a multiplicar. Para esta tarea se encargará el proceso raiz 0.
* Cuando este transmita las métricas al resto de procesos, podrán continuar los demás.
* Seguidamente, cada proceso debe determinar la porción de trabajo que debe hacer. En esta fase, se adquiere la porción de la matriz que corresponde,
* inicializando con los valores adecuados y también la parte del vector.
*
* Una vez los procesos hayan inicializado sus porciones, deben transmitir su parte del vector, para que todo el mundo tenga el vector completo 
 y pueda realizar las multipliciones de los paneles que tiene asignados en la matriz.
* El siguiente paso consiste en hacer la multiplicación en sí, donde los procesos almacenarán los resultados en un vector de solución propio.
* Finalmente, imprimimos el vector solución completo.
*
* Para poder ver la matriz que tiene asignada cada proceso, descomentar la constante DEBUG.
*
**/
int main(int argc, char** argv)  
{
     
    int m, n;
    int i, j;
    int dim_filas = 0; //porcion de filas para cada proceso
    int dim_columnas = 0;//porcion de columnas para cada proceso
    int fila_proceso = 0; //primera fila de cada proceso
    int columna_proceso = 0;//primer panel del vector para cada proceso
    float **MAT, *VEC_RES,  *VEC_GLOB; 
    int yo, p;
    MPI_Status status;
    
    //Inicio zona paralela
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &yo);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    
   
    /*El proceso raiz lee las dimensiones de la matriz y las
    manda al resto de procesos.
    */
   
    if (yo == 0){ //root
    	printf ("Escribe las dimensiones de la matriz (MxN):\n");
    	scanf ("%d %d", &m, &n);
    	
    	//si ambos parámetros no son múltiplos del número de procesos, indicarlo y terminar
    	if (!((m % p == 0) || (n % p == 0))){
    		printf ("ERROR: Las dimensiones no son válidas para el número de procesos.\n");
    		return -1;
    	}
    }
   
    //Se transmiten las dimensiones a todos. Como las llamadas son bloqueantes, nos aseguramos de que después todos las tienen.
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    

    dim_filas = m/p; //para los paneles de matriz
    dim_columnas = n/p; //para los paneles de vector
    fila_proceso = dim_filas * yo; //para saber en que fila empieza
    columna_proceso = dim_columnas*yo; //para saber en que parte del vector se empieza
    
    
    //Definimos las porciones de matrices y vector
    MAT = malloc (dim_filas*sizeof(float*));
    float VEC[dim_columnas];
    VEC_GLOB = malloc (n*sizeof(float)); //vector total de multiplicación
    VEC_RES = malloc (dim_filas*sizeof(float)); //vector propio solución
    
    for (i = 0; i < dim_filas; i++){
    	MAT[i] = malloc (n*sizeof(float));
    	
    }
    
    //inicializamos matriz
    for (i = 0; i < dim_filas; i++ ){
    	for (j = 0; j < n; j++){
    		MAT[i][j] = (fila_proceso + i)*10 +j;
    	}
    }
    
       
    //inicializamos solo las posiciones correspondientes del vector, el resto se completarán después del allgather.
    for (i = 0; i < dim_columnas; i++){
    	VEC[i] = columna_proceso+i;
    }   
    
    //Se agrupa el vector que usarán los procesos para multiplicar
    MPI_Allgather(VEC, dim_columnas, MPI_FLOAT, VEC_GLOB, dim_columnas, MPI_FLOAT, MPI_COMM_WORLD); 
    
   
    //realizar la multiplicación 
    for (i = 0; i < dim_filas; i++){
    	VEC_RES[i] = 0.0;
    	for (j = 0; j < n; j++){
    		VEC_RES[i] = VEC_RES[i] + MAT[i][j]*VEC_GLOB[j];
    	}
    }
    
    
    //Imprimir el vector de resultado
    for (int proc = 0; proc < p; proc++){
    	if (yo == proc){
    		for (i = 0; i < dim_filas; i++){
    			printf ("PROC%d-VEC_RES[%d]: %.2f\n",yo ,columna_proceso+i, VEC_RES[i]);
    		}
    	}
    	
    }
    
     MPI_Finalize();
    
   
    return 0;
}  /* main */
