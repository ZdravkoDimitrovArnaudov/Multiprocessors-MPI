#include <mpi.h>
#include <stdio.h>

int main(int argc, char* argv[] )
{
    MPI_Comm comunicador;
    int yo_global, p_global, yo_local, p_local;
    int color;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank( MPI_COMM_WORLD, &yo_global);
    MPI_Comm_size( MPI_COMM_WORLD, &p_global);

    /* determino cual será mi color de comunicador */
    if(yo_global % 2 == 0)
        color = 0;
        
    else
        color = 1;
        
    printf("Soy %d de color %d\n", yo_global,color);
    /* creo el nuevo comunicador cada tipo en uno */
    MPI_Comm_split(MPI_COMM_WORLD, color, yo_global, &comunicador);
    /* ahora determino mi orden en cada uno */
    MPI_Comm_rank(comunicador, &yo_local);
    MPI_Comm_size(comunicador, &p_local);
    printf("Soy %d en global y %d en local\n",yo_global, yo_local);
    printf("Los tamaños son: %d y %d \n", p_global, p_local);

    MPI_Finalize();
    return 0;
}
