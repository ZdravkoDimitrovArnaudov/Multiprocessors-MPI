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
  int yo, p;
  

  
  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &yo);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  

  //Indicamos que type_columna será tipo de dato
  MPI_Datatype code;
  MPI_Aint extent;
  MPI_Aint lower_bound;
 
 
  //creamos el tipo de dato y los desplazamientos con direcciones 
  MPI_Aint offset[2];
  code_t code1;
  MPI_Aint dir_base;
  
  MPI_Get_address (&code1, &dir_base);
  MPI_Get_address (&code1.num_code, &offset[0]);
  MPI_Get_address (&code1.char_code, &offset[1]);
  
  offset[0] = MPI_Aint_diff (offset[0], dir_base);
  offset[1] = MPI_Aint_diff (offset[1], dir_base);
  
   
  int count = 2;
  int blocklengths[2] = {1,1};
  MPI_Datatype types[2] = {MPI_FLOAT, MPI_CHAR};
  
  MPI_Type_create_struct (count, blocklengths, offset, types, &code);
  MPI_Type_commit (&code);
  
  if (yo == 0){
  
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
