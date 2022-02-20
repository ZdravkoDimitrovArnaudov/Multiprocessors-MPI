#include <mpi.h>
#include <stdio.h>


/*
	Colors	Numbers
	yellow	0
	purple	1
	orange	2
	greeen	3
	blue	4
	
 Procesos ordenados según orden global.
	
*/

int main(int argc, char* argv[] )
{
    MPI_Comm comunicador_horizontal, comunicador_vertical;
    int yo_global, p_global, yo_local_h, p_local_h, yo_local_v, p_local_v;
    int color_horizontal, color_vertical;
    int results[5] = {0};

    MPI_Init(&argc, &argv);
    MPI_Comm_rank( MPI_COMM_WORLD, &yo_global);
    MPI_Comm_size( MPI_COMM_WORLD, &p_global);

    /* determino cual será mi color de mis comunicadores */    
    switch (yo_global)
    {
    	case 0:
    	{
    		color_horizontal = 0;
    		color_vertical = 2;
    		break;
    	}
    	case 1:
    	{
    		color_horizontal = 0;
    		color_vertical = 3;
    		break;
    	}
    	case 2:
    	{
    		color_horizontal = 0;
    		color_vertical = 4;
    		break;
    	}
    	case 3:
    	{
    		color_horizontal = 1;
    		color_vertical = 2;
    		break;
    	}
    	case 4:
    	{
    		color_horizontal = 1;
    		color_vertical = 3;
    		break;
    	}
    	case 5:
    	{
    		color_horizontal = 1;
    		color_vertical = 4;
    		break;
    	}

    }
        
    printf("Soy %d de color horizontal %d y vertical %d\n", yo_global,color_horizontal, color_vertical);
   
    
    /*Creamos los comunicadores verticales y globales*/
    MPI_Comm_split(MPI_COMM_WORLD, color_horizontal, yo_global, &comunicador_horizontal);
    
    MPI_Comm_split(MPI_COMM_WORLD, color_vertical, yo_global, &comunicador_vertical);
    
    /*Se determina el orden en cada uno de ellos*/
    MPI_Comm_rank(comunicador_horizontal, &yo_local_h);
    MPI_Comm_size(comunicador_horizontal, &p_local_h);
    
    MPI_Comm_rank(comunicador_vertical, &yo_local_v);
    MPI_Comm_size(comunicador_vertical, &p_local_v);
    
    printf("Soy %d en global y %d en local horizontal\n",yo_global, yo_local_h);
    printf("Soy %d en global y %d en local vertical\n",yo_global, yo_local_v);
    //printf("Los tamaños son: %d y %d \n", p_global, p_local);
    
    
    //Usaremos la llamada Allreduce
    
    //comunicador horizontal
    MPI_Allreduce (&yo_global, &results[color_horizontal], 1, MPI_INT, MPI_SUM, comunicador_horizontal);
    
    //comunicador vertical
    MPI_Allreduce (&yo_global, &results[color_vertical], 1, MPI_INT, MPI_SUM, comunicador_vertical);
    
    
    printf ("Soy P%d global: Color Horizontal: %d, Valor: %d\n", yo_global, color_horizontal, results[color_horizontal]);
    printf ("Soy P%d global: Color Vertical: %d, Valor: %d\n", yo_global, color_vertical, results[color_vertical]);
    
    

    MPI_Finalize();
    return 0;
}
