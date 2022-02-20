#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"	

struct pasajero{
	int  clase;
	char *nombre;
}typedef t_pasajero;




int main(int argc, char** argv)  
{
     
    int i, j;
    int yo, p;
    MPI_Status status;
    int size_bytes = 4 * sizeof(t_pasajero);
    
    //Inicio zona paralela
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &yo);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    
    //tipo de dato pasajero
    MPI_Datatype type_pasajero;
    
    
    MPI_Aint extent;
    MPI_Aint lower_bound;
    
    
    t_pasajero pasajeros2[4];
    t_pasajero pasajeros1[4] = {{.clase = 1, .nombre = "A"}, 
    				{.clase = 2, .nombre = "B"},
    				{.clase = 3, .nombre = "C"},
    				{.clase = 4, .nombre = "D"}};
    				
    
    				
    				
    //Mostramos por pantalla el resultado
    printf ("Soy %d: ", yo);
    for (i = 0; i < 4; i++){
    	printf ("%d ", pasajeros1[i].clase);
    	
    }
    printf ("\n");
    
    
    //definimos el tipo de dato y hacemos commit
    MPI_Type_get_extent (MPI_INT, &lower_bound, &extent);
  
    int count = 2;
    int blocklengths[2] = {1,1};
    MPI_Aint offset[2] = {0, extent};
    MPI_Datatype types[2] = {MPI_INT, MPI_CHAR};
  
    MPI_Type_create_struct (count, blocklengths, offset, types, &type_pasajero);
    MPI_Type_commit (&type_pasajero);
    
    //enviamos al resto de procesos los pasajeros
    for (i = 0; i < 4; i++){
    	MPI_Send(&pasajeros1[i], 1, type_pasajero, i, 0, MPI_COMM_WORLD);	
    }
    
    //recibimos del resto de procesos los pasajeos
    for (i = 0; i < 4; i++){
    	MPI_Recv(&pasajeros2[i], 1, type_pasajero, i, 0, MPI_COMM_WORLD, &status);	
    }

    
    //Mostramos por pantalla el resultado
    printf ("Soy %d: ", yo);
    for (i = 0; i < 4; i++){
    	printf ("%d ", pasajeros2[i].clase);
    	
    }
    printf ("\n");
    
    
   
     MPI_Finalize();
    
   
    return 0;
}  /* main */
