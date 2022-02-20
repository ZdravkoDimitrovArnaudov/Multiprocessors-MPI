#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <fcntl.h>

#define DEBUG
#define TAM 100


int main(int argc, char** argv)  
{
    int yo, p;
    int root = 0;
    int fichero, i, j;
    int size_proc_buff;
    double buffer[TAM];
    MPI_Status status;
    
    //Inicio zona paralela
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &yo);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    
    //comprobamos que N sea divisor de 100
    if (yo == 0){
    	if (100 % p != 0){
    		printf ("ERROR: El número de procesos no es divisor de 100.\n");
    		return -1;
    	}
    }
    
    //determinamos el tamano del buffer de cada proceso para el scatter
    size_proc_buff = 100/p;
    double *proc_buffer = malloc (size_proc_buff * sizeof(double));
    
    
    if (yo == 0){
        fichero = open(argv[1],O_RDONLY);
        //almacenamos los 100 dobles en el buffer
        read(fichero, buffer, TAM*sizeof(double));
  	
  	#ifdef DEBUG
  		printf ("Soy el proceso raiz, dobles leidos.\n");
  	#endif
    }
    
    //lo que hacemos ahora, es scatter de los valores al resto de procesos.
    MPI_Scatter(buffer, size_proc_buff, MPI_DOUBLE, proc_buffer, size_proc_buff, MPI_DOUBLE, root, MPI_COMM_WORLD);
     
    //cada proceso imprime los valores
    printf ("Soy %d: ", yo);
    for (j = 0; j < size_proc_buff; j++){
    	printf ("%2.0f ", proc_buffer[j]);
    	
    }
    printf ("\n");
   
    
   close(fichero);
   MPI_Finalize();
    
   
    return 0;
}  /* main */
