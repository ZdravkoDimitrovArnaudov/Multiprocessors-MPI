#include <stdio.h>
#include "mpi.h"

int main(int argc, char** argv)  
{
  int i, j;
  int yo, p;
  double M[6][5];
  double M2[6][5];
  int blocklengths[10] = {1,1,1,1,1,1,1,1,1};
  int displacements[10] = {0, 2, 4 ,10 ,12 ,14 ,20 ,22 ,24};
  int count = 9;
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &yo);
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  //Indicamos que type_columna será tipo de dato
  MPI_Datatype matriz_dispersa;
 
  
  //todos los procesos inicializan su copia
  for (i = 0; i < 6; i++){
  		for (j = 0; j < 5 ; j++){
  			M[i][j] = (double) 0;
  			
  		}
  		
  } 
  
  
  //el proceso 0 inicializa las posiciones correspondientes
  if (yo == 0){
  	
  	//casillas negras
  	M[0][0] = 1.0;
  	M[0][2] = 1.0;
  	M[0][4] = 1.0;
  	M[2][0] = 2.0;
  	M[2][2] = 2.0;
  	M[2][4] = 2.0;
  	M[4][0] = 3.0;
  	M[4][2] = 3.0;
  	M[4][4] = 3.0;
  	
  }
  
  /*el proceso 1 inicializa la matriz B 
  if (yo == 1){
  	for (i = 0; i < 10; i++){
  		for (j = 0; j < 10; j++){
  			M2[i][j] = 0.00;
  		}
  		
  	} 
  	
  }*/

	  
  
  //definimos el tipo de dato y hacemos commit.
  MPI_Type_indexed (count, blocklengths, displacements, MPI_DOUBLE, &matriz_dispersa); 
  
  MPI_Type_commit (&matriz_dispersa);
  
  if (yo == 0){
  	//enviamos la columna al proceso 1
  	MPI_Send(&M, 1, matriz_dispersa, 1, 0, MPI_COMM_WORLD);
  	
  } else if (yo == 1){
  
  	//recibimos la matriz inicializada como proceso derivado
  	MPI_Recv(&M, 1, matriz_dispersa, 0, 0, MPI_COMM_WORLD, &status);
  	
  	printf ("Soy el proceso 1, imprimo matriz triangular: \n");
  	//comprobamos que la matriz se ha rellenado correctamente.
  	for (int i = 0; i < 6; i++){
  		for (int j = 0; j < 5; j++){
  			printf ("%.2f  ", M[i][j]);
  		}
  		printf ("\n");
  
  	}
  } 
 
  MPI_Type_free(&matriz_dispersa);
  MPI_Finalize();
  return 0;
}
