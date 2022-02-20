#include <stdio.h>
#include <string.h>
#include "mpi.h"

/**
* Ejer1_D.c
* Esta versión es la correcta porque cuando uno manda el otro recibe.
*
*
*
**/
int main(int argc, char* argv[]) {
    int         yo;            /* rank of process      */
    int         p;             /* number of processes  */
    int         source;        /* rank of sender       */
    int         dest;          /* rank of receiver     */
    int         tag = 0;       /* tag for messages     */
    char        message[100];  /* storage for message  */
    MPI_Status  status;        /* return status for    */
                               /* receive              */

    /* Start up MPI */
    MPI_Init(&argc, &argv);

    /* Find out process rank  */
    MPI_Comm_rank(MPI_COMM_WORLD, &yo);

    /* Find out number of processes */
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (yo == 0) 
    {
        /* Create message */
        sprintf(message, "Hola desde proceso %d!", yo);
        dest = 1;
        /* Use strlen+1 so that '\0' gets transmitted */
	printf("Mando desde %d \n", yo);
        MPI_Send(message, strlen(message)+1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
        source = 1;
        MPI_Recv(message, 100, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);
	printf("He recibido %s desde %d \n", message, source);
    }
    else if (yo == 1) 
    {
        source = 0;
        MPI_Recv(message, 100, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);
	printf("He recibido %s desde %d \n", message, source);
	printf("Mando desde %d \n", yo);
        /* Create message */
        sprintf(message, "Hola desde proceso %d!", yo);
        dest = 0;
        /* Use strlen+1 so that '\0' gets transmitted */
        MPI_Send(message, strlen(message)+1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
    }

    /* Shut down MPI */
    MPI_Finalize();
    //return 0;
} /* main */
