#include <stdio.h>
#include "mpi.h"

int main(int argc, char** argv)  
{
  int i, j;
  int yo, p;
  double M[10][10];
  double M2[10];
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &yo);
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  //Indicamos que type_columna será tipo de dato
  MPI_Datatype type_columna;
 
  
  //el proceso 0 inicializa la matriz
  if (yo == 0){
  	for (i = 0; i < 10; i++){
  		for (j = 0; j < 10; j++){
  			if (j == 2){ // si está en la columna 2, escribe su posición
  				M[i][j] = (double)i;
  			} else {
  				M[i][j] = 0; //nulo
  			}
  		}
  	} 
  }
  
  //definimos el tipo de dato y hacemos commit.
  MPI_Type_vector (10, 1, 10, MPI_DOUBLE, &type_columna);
  MPI_Type_commit (&type_columna);
  
  if (yo == 0){
  	//enviamos la columna al proceso 1
  	for (i = 0; i < p; i++){
  	MPI_Send(&M[0][2], 1, type_columna, i, 0, MPI_COMM_WORLD);
  	}
  }
  
  //recibimso la columna del proceso 1
  MPI_Recv(M2, 10, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
  	
  //averiguar que la columna se ha recibido correctamente 
  if (yo == 1){
  	printf ("Soy P1, imprimo columa 2 de M\n");
  	for (i = 0; i < 10; i++){
  		printf ("%.2f\n", M2[i]);
  	
  	}
  		
  }
  	
  
  
  
  MPI_Type_free(&type_columna);
  MPI_Finalize();
  return 0;
}


