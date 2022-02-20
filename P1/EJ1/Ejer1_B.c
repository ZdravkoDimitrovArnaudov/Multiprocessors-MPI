/*
 * Ejer1_B.c
 * Se mandan menajes desde diferentes fuentes sin seleccionar los mensajes en el receptor.
 * 
 * 
 * 
 */


#include <stdio.h>
#include <string.h>
#include <mpi.h>

#define NUM_MESSAGES 10

int main(int argc, char **argv){
	int me; //rank of process
	int p; //number of processes
	//int source;
	int dest;
	//int tag = 0; //tag for messages
	int number; //number to send 
	int buff_number = 0;
	MPI_Status status; //return status
	
	MPI_Init (&argc, &argv); //start rup MPI
	MPI_Comm_rank (MPI_COMM_WORLD, &me); //get process rank
	MPI_Comm_size (MPI_COMM_WORLD, &p); //get number of processes
	
	if (me == 0){ //receiver
		
		//source = 1;
		printf ("Receiver: %d printing messages received:\n", me);
		for (int i = 0; i < NUM_MESSAGES; i++){
			MPI_Recv(&buff_number, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			printf ("Message %d received from %d: %d\n", status.MPI_TAG, status.MPI_SOURCE, buff_number);
		}

		printf ("Receiver ends printing messages.\n");
		
		
	} else { //sender
	
		dest = 0;
		number = 100;
		
		for (int i = 0; i < NUM_MESSAGES; i++){
			MPI_Send (&number, 1, MPI_INT, dest, i, MPI_COMM_WORLD);
			number++;
		}
		printf ("Sender %d has just finished.\n", me);
		
		
	}
	
	MPI_Finalize(); //shutdown MPI
	
	
	return 0;
}

