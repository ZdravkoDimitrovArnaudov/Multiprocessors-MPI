#include <stdio.h>
#include "mpi.h"

//estructura codigo
typedef struct code{
        float num_code;
	char char_code;
	
}code_t;


int main(int argc, char** argv)  
{
  int i, j;
  int yo, p, size;
  

  
  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &yo);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  

  //Indicamos que type_columna será tipo de dato
  MPI_Datatype code;
  MPI_Datatype type;
  MPI_Aint extent;
  MPI_Aint lower_bound;
 
 
  //definimos el tipo de dato y hacemos commit.
  MPI_Type_get_extent (MPI_FLOAT, &lower_bound, &extent);
  
  //definimos el tipo de dato heterogeneo
  int count = 2;
  int blocklengths[2] = {1,1};
  MPI_Aint offset[2] = {0, extent};
  MPI_Datatype types[2] = {MPI_FLOAT, MPI_CHAR};
  
  MPI_Type_create_struct (count, blocklengths, offset, types, &code);
  MPI_Type_commit (&code);
  
  
  type = code;
  MPI_Type_size (type, &size);
  
  
  if (yo == 0){
  
  	//APARTADO C
  	printf ("(Apartado C) El tamaño del tipo de dato code es :%d bytes\n", size);
  
  	code_t struct_code2 = {2, *"A"};
  	printf ("P0 el código es:%.2f%c\n", struct_code2.num_code, struct_code2.char_code);
  	
  	//enviamos la estructura inicializada
  	MPI_Send(&struct_code2, 1, code, 1, 0, MPI_COMM_WORLD);
  	
  } else if (yo == 1){
  
  	//recibimos la estructura enviada 
  	code_t struct_code;
  	MPI_Recv(&struct_code, 1, code, 0, 0, MPI_COMM_WORLD, &status);
  	
  	printf ("P1 el código es:%.2f%c\n", struct_code.num_code, struct_code.char_code);
  	
  } 
 
  MPI_Type_free(&code);
  MPI_Finalize();
  return 0;
}
