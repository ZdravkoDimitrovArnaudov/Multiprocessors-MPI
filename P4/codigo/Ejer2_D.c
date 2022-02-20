#include <stdio.h>
#include "mpi.h"

//estructura codigo
typedef struct code{
        double num_code;
	char char_code;
	
}code_t;

typedef struct new_code{
        code_t codigo_antiguo;
	int int_code;
	
}new_code_t;



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
  MPI_Datatype new_code;
  MPI_Datatype type;
  MPI_Aint extent;
  MPI_Aint lower_bound;
 
 
  //definimos el tipo de dato y hacemos commit.
  MPI_Type_get_extent (MPI_DOUBLE, &lower_bound, &extent);
  
  //definimos el tipo de dato codigo antiguo
  int count = 2;
  int blocklengths[2] = {1,1};
  MPI_Aint offset[2] = {0, extent};
  MPI_Datatype types[2] = {MPI_DOUBLE, MPI_CHAR};
  
  MPI_Type_create_struct (count, blocklengths, offset, types, &code);
  MPI_Type_commit (&code);
  
  
  //definimos el tipo de dato codigo nuevo
  MPI_Type_get_extent (code, &lower_bound, &extent);
  
  int count2 = 2;
  int blocklengths2[2] = {1,1};
  MPI_Aint offset2[2] = {0, extent};
  MPI_Datatype types2[2] = {code, MPI_INT};
  
  MPI_Type_create_struct (count2, blocklengths2, offset2, types2, &new_code);
  MPI_Type_commit (&new_code);
  
  
  //para las dimensiones
  type = new_code;
  MPI_Type_size (type, &size);
  MPI_Type_get_extent (new_code, &lower_bound, &extent);
  
  
  
  if (yo == 0){
  
  	//veremos cuanto ocupa el nuevo tipo de dato derivado
  	printf ("Size: %d bytes\n", size);
  	printf ("Extent: %ld bytes\n", extent);
  
  
  	new_code_t struct_code2 = {.codigo_antiguo.num_code=2, .codigo_antiguo.char_code=*"A", .int_code=5}; //= {{2, *"A"}, 5};
  	printf ("P0 el código es:%.2f%c%d\n", struct_code2.codigo_antiguo.num_code, struct_code2.codigo_antiguo.char_code,  struct_code2.int_code);
  	
  	//enviamos la estructura inicializada
  	MPI_Send(&struct_code2, 1, new_code, 1, 0, MPI_COMM_WORLD);
  	
  } else if (yo == 1){
  
  	//recibimos la estructura enviada 
  	new_code_t struct_code;
  	MPI_Recv(&struct_code, 1, new_code, 0, 0, MPI_COMM_WORLD, &status);
  	
  	printf ("P1 el código es:%.2f%c%d\n", struct_code.codigo_antiguo.num_code, struct_code.codigo_antiguo.char_code,  struct_code.int_code);
  	
  	
  	//Comprobamos get elements y get count
  	MPI_Get_elements (&status, new_code, &count);
  	printf ("GET ELEMENTS: %d\n", count);
  	
  	MPI_Get_count(&status, new_code, &count2);
  	printf ("GET COUNT: %d\n", count2);
  	
  	
  	
  } 
 
  MPI_Type_free(&code);
  MPI_Finalize();
  return 0;
}
