#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    char        message[100];
    char 	 message_send[100];
    MPI_Status  status;        /* return status for    */
    MPI_Request request;
                         
   
    /* Start up MPI */
    MPI_Init(&argc, &argv);

    /* Find out process rank  */
    MPI_Comm_rank(MPI_COMM_WORLD, &yo);

    /* Find out number of processes */
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (yo == 0) 
    {
        source = 1;
        MPI_Irecv(message, 100, MPI_CHAR, source, tag, MPI_COMM_WORLD, &request);//inicializamos la recepción
     
        /* Create message */
        printf("Mando desde %d \n", yo);
        sprintf(message_send, "Hola desde proceso %d!", yo);
        dest = 1;
        /* Use strlen+1 so that '\0' gets transmitted */
        MPI_Send(message_send, strlen(message_send)+1, MPI_CHAR, dest, tag, MPI_COMM_WORLD); //mientras tanto, mandamos nuestro mensaje
        MPI_Wait(&request, &status); //esperamos a que se completen las operaciones
        
        //imprimimos lo que se ha recibido
        printf("He recibido %s desde %d \n", message, source);
	printf("Mando desde %d \n", yo);
        printf ("El proceso receiver %d ha terminado de recibir y enviar.\n", yo);
    }
    else if (yo == 1) 
    {
        source = 0;
        MPI_Irecv(message, 100, MPI_CHAR, source, tag, MPI_COMM_WORLD, &request);
       
        /* Create message */
        printf("Mando desde %d \n", yo);
        sprintf(message_send, "Hola desde proceso %d!", yo);
        dest = 0;
        
        /* Use strlen+1 so that '\0' gets transmitted */
        MPI_Send(message_send, strlen(message_send)+1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
        MPI_Wait (&request, &status);
        
        printf("He recibido %s desde %d \n", message, source);
        printf ("El proceso sender %d ha terminado de recibir y enviar.\n", yo);
    }

    /* Shut down MPI */
    MPI_Finalize();
    return 0;
} /* main */
