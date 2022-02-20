#include <stdio.h>
#include "mpi.h"

int main(int argc, char** argv)  
{
  int i, j;
  int yo, p;
  double M[10][10];
  double M2[10][10];
  int blocklengths[10] = {10,9,8,7,6,5,4,3,2,1};
  int displacements[10] = {0, 11,22,33,44,55,66,77,88,99};
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &yo);
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  //Indicamos que type_columna será tipo de dato
  MPI_Datatype matriz_triangular;
 
  
  //el proceso 0 inicializa la matriz A triangular superior
  if (yo == 0){
  	for (i = 0; i < 10; i++){
  		for (j = 0; j < 10-i; j++){
  			M[i][i+j] = (double)j;
  		}
  		
  	} 
  	
  }
  
  //el proceso 1 inicializa la matriz B 
  if (yo == 1){
  	for (i = 0; i < 10; i++){
  		for (j = 0; j < 10; j++){
  			M2[i][j] = 0.00;
  		}
  		
  	} 
  	
  }

	  
  
  //definimos el tipo de dato y hacemos commit.
  MPI_Type_indexed (10, blocklengths, displacements, MPI_DOUBLE, &matriz_triangular); 
  MPI_Type_commit (&matriz_triangular);
  
  if (yo == 0){
  	//enviamos la columna al proceso 1
  	MPI_Send(&M, 1, matriz_triangular, 1, 0, MPI_COMM_WORLD);
  	
  } else if (yo == 1){
  
  	//inicializamos matriz B
  	
  
  	//recibimso la columna del proceso 1
  	MPI_Recv(&M2, 1, matriz_triangular, 0, 0, MPI_COMM_WORLD, &status);
  	
  	printf ("Soy el proceso 1, imprimo matriz triangular: \n");
  	//comprobamos que la matriz se ha rellenado correctamente.
  	for (int i = 0; i < 10; i++){
  		for (int j = 0; j < 10; j++){
  			printf ("%.2f  ", M2[i][j]);
  		}
  		printf ("\n");
  
  	}
  } 
 
  MPI_Type_free(&matriz_triangular);
  MPI_Finalize();
  return 0;
}


