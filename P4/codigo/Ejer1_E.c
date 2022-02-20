#include <stdio.h>
#include "mpi.h"

int main(int argc, char** argv)  
{
  int i, j;
  int yo, p;
  double M[8][8];
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &yo);
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  //Indicamos los nuevos tipos de datos
  MPI_Datatype fila;
  MPI_Datatype tablero_ajedrez;
  MPI_Aint extent;
  
  MPI_Aint lower_bound;
  

  
  //ambos procesos inicializan su tablero
  for (i = 0; i < 8; i++){
  	for (j = 0; j < 8; j++){
  		M[i][j] = 0.0; //nulo
  		
  	}
  } 
 
 //El proceso 0 completa las casillas blancas
  if (yo == 0){
  
  	//inicializar tablero con casillas blancas
  	for (i = 0; i < 8; i++){
  		for (j = 0; j < 8; j++){
  		
  			if (i % 2 == 0){ //fila par
  				if (j % 2 != 0){ //columna impar
  					M[i][j] = 1.0; //casilla blanca
  					
  				}
  				
  			
  			} else { //fila impar
  				if (j % 2 == 0){ //columna par
  					M[i][j] = 1.0; //casilla blanca
  				
  				}
  			
  			}
  			
  		}
  			
  	}
  	
  	printf ("P0: imprime matriz prueba.\n");
  	for (int i = 0; i < 8; i++){
  		for (int j = 0; j < 8; j++){
  			printf ("%.2f  ", M[i][j]);
  		}
  		printf ("\n");
  
  	}
  	
  }
   
  
  
  int count = 4;
  int blocklengths[4] = {1,1,1,1};
  int displacements[4] = {0,2,4,6};
  MPI_Type_indexed (count, blocklengths, displacements, MPI_DOUBLE, &fila);
  //MPI_Type_vector (count, blocklength, displacements, MPI_DOUBLE, &fila);
  MPI_Type_commit (&fila);
  
  
  
  if (yo == 0){
  	//enviamos el tablero al proceso 1.
  	for (int i = 0; i < 8; i++){
  		if (i % 2 == 0){
  			MPI_Send(&M[i][1], 1, fila, 1, i, MPI_COMM_WORLD);
  			
  		} else {
  			MPI_Send(&M[i][0], 1, fila, 1, i, MPI_COMM_WORLD);
  			
  		}
  	}
  
  
  } else if (yo == 1){
  	//recibimos el tablero enviado del proceso 0ç
  	for (int i = 0; i < 8; i++){
  		if (i % 2 == 0){
  			MPI_Recv(&M[i][1], 1, fila, 0, i,MPI_COMM_WORLD, &status);
  			
  		} else {
  			MPI_Recv(&M[i][0], 1, fila, 0, i,MPI_COMM_WORLD, &status);
  			
  		}
  	}
  	//MPI_Recv(&M, 1, tablero_ajedrez, 0, 0,MPI_COMM_WORLD, &status);
  	
  	printf ("P1: Imprime tablero de ajedrez\n");
  	//averiguar que la columna se ha recibido correctamente 
  	for (int i = 0; i < 8; i++){
  		for (int j = 0; j < 8; j++){
  			printf ("%.2f  ", M[i][j]);
  		}
  		printf ("\n");
  
  	}
  	
  }
  
  
  MPI_Type_free(&fila);
  //MPI_Type_free(&tablero_ajedrez);
  MPI_Finalize();
  return 0;
}

