/*
 * Ejer2_B.c
 * 
 * 
 * 
 */


#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <stdlib.h>

#define NUM_MESSAGES 10

int main(int argc, char **argv){
	int me; //rank of process
	int p; //number of processes
	int dest;
	int buff_number = 0;
	MPI_Status status; //return status
	//int size = 0;
	int size_received = 0;
	int buffer_size  = 0;
	
	
	//recogemos número de elementos
	if (argc == 2){
		buffer_size = atoi (argv[1]);
	} else {
		buffer_size = 10;
	}
	
	
	int *integer_buffer_send = malloc (buffer_size * sizeof (int));
	int *integer_buffer_receive = malloc (buffer_size * sizeof (int));
	
	for (int i = 0; i < buffer_size ; i++){ //inicializamos buffer de envío
		integer_buffer_send[i] = 100 + i;
	}
	
	
	MPI_Init (&argc, &argv); //start rup MPI
	MPI_Comm_rank (MPI_COMM_WORLD, &me); //get process rank
	MPI_Comm_size (MPI_COMM_WORLD, &p); //get number of processes
	
	
	
	
	// Allocate enough space to issue 2 buffered sends and their messages
        int b_size = (1);
        char* buffer = malloc(b_size);
	MPI_Buffer_attach(buffer, buffer_size); //buffer attach
	
	
	
	
	
	if (me == 0){ //receiver
		
		
		MPI_Recv(integer_buffer_receive, buffer_size, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		printf ("Receiver: %d printing messages received:\n", me);
		for (int i = 0; i < NUM_MESSAGES; i++){	
			printf ("Message %d received from %d: %d\n", status.MPI_TAG, status.MPI_SOURCE, integer_buffer_receive[i]);
			
		}
		
		MPI_Get_count(&status, MPI_BYTE, &size_received);
		printf ("El tamaño del mensaje recibido es: %d\n", size_received);
		printf ("Receiver ends printing messages.\n");
		
	} else { //sender
	
		dest = 0;
		MPI_Bsend (integer_buffer_send, buffer_size, MPI_INT, dest, me, MPI_COMM_WORLD);
		printf ("Sender %d has just finished sending messages.\n", me);
		MPI_Buffer_detach(&buffer, &buffer_size);
		
		
		
	}
	
	
	
	MPI_Finalize(); //shutdown MPI
	
	free (integer_buffer_send);
	free (integer_buffer_receive);
	free (buffer);
	
	return 0;
}

