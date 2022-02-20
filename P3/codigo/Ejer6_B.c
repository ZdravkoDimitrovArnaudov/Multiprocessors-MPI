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
    
    t_pasajero pasajeros2[4];
    t_pasajero pasajeros1[4] = {{.clase = 1, .nombre = "A"}, 
    				{.clase = 2, .nombre = "B"},
    				{.clase = 3, .nombre = "C"},
    				{.clase = 4, .nombre = "D"}};
    				
    //Probar con malloc..
    				
    				
    //Mostramos por pantalla el resultado
    printf ("Soy %d: ", yo);
    for (j = 0; j < 4; j++){
    	printf ("%d ", pasajeros1[j].clase);
    	
    }
    printf ("\n");
    				
    
    
    //realizamos alltoall para redistribuir a los viajantes
    MPI_Alltoall ((char*)pasajeros1, sizeof(t_pasajero), MPI_BYTE, (char*)pasajeros2, sizeof(t_pasajero) , MPI_BYTE, MPI_COMM_WORLD);
    
    //Mostramos por pantalla el resultado
    printf ("Soy %d: ", yo);
    for (j = 0; j < 4; j++){
    	printf ("%d ", pasajeros2[j].clase);
    	
    }
    printf ("\n");
    
    
   
     MPI_Finalize();
    
   
    return 0;
}  /* main */
