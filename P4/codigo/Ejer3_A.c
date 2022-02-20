#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"	



int main(int argc, char** argv)  
{
     
    int i, j, total;
    MPI_Status status;
   
    
    MPI_Comm par, impar;
    MPI_Group grupo_global, grupo_par, grupo_impar;
    int yo_global, p_global, yo_par, yo_impar, p_par, p_impar; 
    int quitar_par[2] = {1,3};  
    int quitar_impar[2]= {0,2};
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &yo_global);
    MPI_Comm_size(MPI_COMM_WORLD, &p_global);
    
   if(p_global < 4)
    {
        printf("Tiene que ejecutar con al menos 4, no %d \n", p_global);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    
    //creamos grupo global
    MPI_Comm_group(MPI_COMM_WORLD, &grupo_global);
    
    //Creamos los grupos pares e impares, excluyendo los procesos convenientes
    MPI_Group_excl(grupo_global, 2, quitar_par, &grupo_par);
    MPI_Group_excl(grupo_global, 2, quitar_impar, &grupo_impar);
    
    //creamos los comunicadores a raiz de los grupos 
    MPI_Comm_create(MPI_COMM_WORLD, grupo_par, &par);
    MPI_Comm_create(MPI_COMM_WORLD, grupo_impar, &impar);
    
    //obtengo orden en el comunicador que me corresponde
    if (par != MPI_COMM_NULL) {
    	MPI_Comm_rank(par, &yo_par);
    	MPI_Comm_size(par, &p_par);
	printf("Soy %d en global y %d en par local.\n", yo_global, yo_par);
	
	MPI_Allreduce (&yo_global, &total, 1, MPI_INT, MPI_SUM, par);
	
	if (yo_par == 0){
    		printf ("La suma en PAR es: %d\n", total);
    	}
    	
    } else {
    	MPI_Comm_rank(impar, &yo_impar);
    	MPI_Comm_size(impar, &p_impar);
    	printf("Soy %d en global y %d en impar local.\n", yo_global, yo_impar);
    	
    	MPI_Allreduce (&yo_global, &total, 1, MPI_INT, MPI_SUM, impar);
    	
    	if (yo_impar == 0){
    		printf ("La suma en IMPAR es: %d\n", total);
    	}
    	
    }
	
    
    
   
    MPI_Finalize();
    
   
    return 0;
}  /* main */
