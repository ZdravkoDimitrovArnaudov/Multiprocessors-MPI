#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "mpi.h"

/**
*Copia de fichero Ejer1_D_Bloqueo.c
*Esta version es la que se bloquea porque ambos procesos se quedan a la espera de recibir mensaje.
*Se intentará solucionar el bloqueo con llamadas no bloqueantes.
*
**/
int main(int argc, char* argv[]) {
    int         yo;            /* rank of process      */
    int         p;             /* number of processes  */
    int         source;        /* rank of sender       */
    int         dest;          /* rank of receiver     */
    int         tag = 0;       /* tag for messages     */
    int	*array_int;
    int 	*array_receive;
    int 	num_int;
    MPI_Status  status;        /* return status for    */
    MPI_Request request;
                         
    double start_time, end_time;
   
    /* Start up MPI */
    MPI_Init(&argc, &argv);

    /* Find out process rank  */
    MPI_Comm_rank(MPI_COMM_WORLD, &yo);

    /* Find out number of processes */
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    
    
    if (argc == 2){
    	num_int = atoi (argv[1]);
    } else {
    	num_int = 10;
    }
   
    //creamos el array a enviar
    array_int = malloc (num_int * sizeof (int));
    array_receive = malloc (num_int * sizeof (int));
    

    if (yo == 0) //receiver
    {
    	start_time = MPI_Wtime();
        sleep(6);
        MPI_Irecv(array_receive, num_int, MPI_INT, 1, tag, MPI_COMM_WORLD, &request);
        MPI_Wait (&request, &status);
        sleep(1);
        MPI_Irecv(array_receive, num_int, MPI_INT, 1, tag, MPI_COMM_WORLD, &request);
        MPI_Wait (&request, &status);
        end_time = MPI_Wtime();
        printf("La emisión ha tardado :%lf\n",end_time-start_time);
    	
        
     	
    }
    else if (yo == 1) //sender
    {
    
    /*
    * El emisor realiza: computo, envio, computo envio.
    */
    	start_time = MPI_Wtime();
    	sleep(1); //computo
    	MPI_Isend(array_int, num_int , MPI_INT, 0, tag, MPI_COMM_WORLD, &request); //mandamos el primer mensaje
    	sleep(6);
    	MPI_Wait (&request, &status);
    	MPI_Isend(array_int, num_int , MPI_INT, 0, tag, MPI_COMM_WORLD, &request); //mandamos el primer mensaje
    	end_time = MPI_Wtime();
    	printf("La emisión ha tardado :%lf\n",end_time-start_time);
    	
    
       
    }

    /* Shut down MPI */
    MPI_Finalize();
    return 0;
} /* main */
